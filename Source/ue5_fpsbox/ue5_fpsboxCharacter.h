// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "Fps_Box/Core/WebManager.h"
#include "Fps_Box/Core/JSONParser.h"
#include "Fps_Box/Spawners/BoxSpawner.h"
#include "Fps_Box/UI/ScoreWidget.h"
#include "ue5_fpsboxCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

/**
 *  A basic first person character
 */
UCLASS(abstract)
class Aue5_fpsboxCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: first person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* FirstPersonMesh;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

protected:

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	class UInputAction* LookAction;

	/** Mouse Look Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	class UInputAction* MouseLookAction;

	/** Firing a Weapon Input Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* FireWeaponAction;

	/** Spawing Box Input Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* SpawnBoxAction;
	
public:
	Aue5_fpsboxCharacter();

protected:

	virtual void BeginPlay() override;

	/** Called from Input Actions for movement input */
	void MoveInput(const FInputActionValue& Value);

	/** Called from Input Actions for looking input */
	void LookInput(const FInputActionValue& Value);

	/** Handles aim inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoAim(float Yaw, float Pitch);

	/** Handles move inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoMove(float Right, float Forward);

	/** Handles jump start inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpStart();

	/** Handles jump end inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpEnd();

	/* Handles weapon fire input from  controls  */
	UFUNCTION(BlueprintCallable, Category = "Input")
    void FireWeapon();

	/* Handles Spawning Box input from  controls  */
	UFUNCTION(BlueprintCallable, Category = "Input")
	void SpawnBox();

protected:

	/** Set up input action bindings */
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	

public:

	/** Returns the first person mesh **/
	USkeletalMeshComponent* GetFirstPersonMesh() const { return FirstPersonMesh; }

	/** Returns first person camera component **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	//  WebManager 
	UPROPERTY()
	UWebManager* WebManager;

	UFUNCTION()
	void HandleJSONReady(const FString& JsonContent);

	//  JSONParser 
	UPROPERTY()
	UJSONParser* JSONParser;

	// Box spawner 
	UPROPERTY()
	ABoxSpawner* BoxSpawner;

	//Score WWidget ui
	UPROPERTY()
	UScoreWidget* ScoreUi;

	UPROPERTY(BlueprintReadOnly, Category = "Score")
	int32 PlayerScore = 0;

	UFUNCTION()
	void AddScore(int32 Points);

};

