// Copyright Epic Games, Inc. All Rights Reserved.

#include "ue5_fpsboxCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ue5_fpsbox.h"
#include "Blueprint/UserWidget.h"   
#include "Components/TextBlock.h"   

Aue5_fpsboxCharacter::Aue5_fpsboxCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	
	// Create the first person mesh that will be viewed only by this character's owner
	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("First Person Mesh"));

	FirstPersonMesh->SetupAttachment(GetMesh());
	FirstPersonMesh->SetOnlyOwnerSee(true);
	FirstPersonMesh->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::FirstPerson;
	FirstPersonMesh->SetCollisionProfileName(FName("NoCollision"));

	// Create the Camera Component	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Camera"));
	FirstPersonCameraComponent->SetupAttachment(FirstPersonMesh, FName("head"));
	FirstPersonCameraComponent->SetRelativeLocationAndRotation(FVector(-2.8f, 5.89f, 0.0f), FRotator(0.0f, 90.0f, -90.0f));
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	FirstPersonCameraComponent->bEnableFirstPersonFieldOfView = true;
	FirstPersonCameraComponent->bEnableFirstPersonScale = true;
	FirstPersonCameraComponent->FirstPersonFieldOfView = 70.0f;
	FirstPersonCameraComponent->FirstPersonScale = 0.6f;

	// configure the character comps
	GetMesh()->SetOwnerNoSee(true);
	GetMesh()->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::WorldSpaceRepresentation;

	GetCapsuleComponent()->SetCapsuleSize(34.0f, 96.0f);

	// Configure character movement
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	GetCharacterMovement()->AirControl = 0.5f;
}

void Aue5_fpsboxCharacter::BeginPlay()
{
	Super::BeginPlay();

	WebManager = NewObject<UWebManager>(this, UWebManager::StaticClass());
	if (WebManager)
	{
		// Bind delegate correctly
		WebManager->OnJSONReady.AddDynamic(this, &Aue5_fpsboxCharacter::HandleJSONReady);

		WebManager->Initialize();
	}

	if (TSubclassOf<UScoreWidget> WidgetClass = LoadClass<UScoreWidget>(
		nullptr,
		TEXT("/Game/FirstPerson/Blueprints/WBP_ScoreWidget.WBP_ScoreWidget_C")))
	{
		ScoreUi = CreateWidget<UScoreWidget>(GetWorld(), WidgetClass); 

		if (ScoreUi)
		{
			ScoreUi->AddToViewport();
			ScoreUi->UpdateScore(0); 
		}
	}

}
void Aue5_fpsboxCharacter::HandleJSONReady(const FString& JsonContent)
{
	UE_LOG(LogTemp, Log, TEXT("Character: JSON received, length=%d"), JsonContent.Len());

	JSONParser = NewObject<UJSONParser>(this);
	if (JSONParser && JSONParser->ParseJSON(JsonContent))
	{
		UE_LOG(LogTemp, Log, TEXT("Parser: Found %d types, %d objects"),
			JSONParser->BoxTypes.Num(), JSONParser->BoxInstances.Num());

		JSONParser->DebugParsedData();
	}
}
void Aue5_fpsboxCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &Aue5_fpsboxCharacter::DoJumpStart);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &Aue5_fpsboxCharacter::DoJumpEnd);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &Aue5_fpsboxCharacter::MoveInput);

		// Looking/Aiming
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &Aue5_fpsboxCharacter::LookInput);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &Aue5_fpsboxCharacter::LookInput);
		//Fire weapon
		EnhancedInputComponent->BindAction(FireWeaponAction, ETriggerEvent::Started, this, &Aue5_fpsboxCharacter::FireWeapon);
		//Spawn box
		EnhancedInputComponent->BindAction(SpawnBoxAction, ETriggerEvent::Started, this, &Aue5_fpsboxCharacter::SpawnBox);
	}
	else
	{
		UE_LOG(Logue5_fpsbox, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


void Aue5_fpsboxCharacter::MoveInput(const FInputActionValue& Value)
{
	// get the Vector2D move axis
	FVector2D MovementVector = Value.Get<FVector2D>();

	// pass the axis values to the move input
	DoMove(MovementVector.X, MovementVector.Y);

}

void Aue5_fpsboxCharacter::LookInput(const FInputActionValue& Value)
{
	// get the Vector2D look axis
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// pass the axis values to the aim input
	DoAim(LookAxisVector.X, LookAxisVector.Y);

}

void Aue5_fpsboxCharacter::DoAim(float Yaw, float Pitch)
{
	if (GetController())
	{
		// pass the rotation inputs
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void Aue5_fpsboxCharacter::DoMove(float Right, float Forward)
{
	if (GetController())
	{
		// pass the move inputs
		AddMovementInput(GetActorRightVector(), Right);
		AddMovementInput(GetActorForwardVector(), Forward);
	}
}

void Aue5_fpsboxCharacter::DoJumpStart()
{
	// pass Jump to the character
	Jump();
}

void Aue5_fpsboxCharacter::DoJumpEnd()
{
	// pass StopJumping to the character
	StopJumping();
}

void Aue5_fpsboxCharacter::FireWeapon()
{
	FVector CameraLoc;
	FRotator CameraRot;
	GetActorEyesViewPoint(CameraLoc, CameraRot);

	FVector End = CameraLoc + (CameraRot.Vector() * 10000.f); // range

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	
	// Damagae the box
	if (GetWorld()->LineTraceSingleByChannel(Hit, CameraLoc, End, ECC_Visibility, Params))
	{
		if (AActor* HitActor = Hit.GetActor())
		{
			UE_LOG(LogTemp, Log, TEXT("Hit: %s"), *HitActor->GetName());

			if (BoxSpawner)
			{
				int32 Points = 0;
				if (BoxSpawner->DamageBox(HitActor, Points) && Points > 0)
				{
					AddScore(Points);
				}
			}
		}
	}

	// Debug line
	DrawDebugLine(GetWorld(), CameraLoc, End, FColor::Red, false, 1.0f, 0, 1.0f);
}


void Aue5_fpsboxCharacter::SpawnBox()
{
	if (!JSONParser || !GetWorld()) return;

	// Spawn BoxSpawner if it doesn't exist
	if (!BoxSpawner || !BoxSpawner->IsValidLowLevel())
	{
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		BoxSpawner = GetWorld()->SpawnActor<ABoxSpawner>(ABoxSpawner::StaticClass(),FVector::ZeroVector,FRotator::ZeroRotator,Params);

		if (!BoxSpawner)
		{
			UE_LOG(LogTemp, Error, TEXT("Character: Failed to spawn BoxSpawner!"));
			return;
		}
	}

	// Spawn the boxes
	BoxSpawner->SpawnBoxes(JSONParser, GetWorld());
}
void Aue5_fpsboxCharacter::AddScore(int32 Points)
{
	PlayerScore += Points;
	UE_LOG(LogTemp, Log, TEXT("New Score: %d"), PlayerScore);

	if (ScoreUi) 
	{
		ScoreUi->UpdateScore(PlayerScore); 
	}

}
