// Fill out your copyright notice in the Description page of Project Settings.


#include "Fps_Box/Spawners/BoxSpawner.h"
#include "Engine/StaticMeshActor.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
ABoxSpawner::ABoxSpawner()
{
	PrimaryActorTick.bCanEverTick = false;

    // Load the cube mesh 
    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshObj(TEXT("/Game/LevelPrototyping/Meshes/SM_ChamferCube.SM_ChamferCube"));
    if (CubeMeshObj.Succeeded())
    {
        CubeMesh = CubeMeshObj.Object;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("BoxSpawner: Failed to find Cube mesh in constructor!"));
    }
	//load default material
    static ConstructorHelpers::FObjectFinder<UMaterial> MaterialFinder(TEXT("/Game/FirstPerson/Blueprints/M_BoxBase.M_BoxBase"));
    if (MaterialFinder.Succeeded())
    {
        DefaultBoxMaterial = MaterialFinder.Object;
    }
}

// Called when the game starts or when spawned
void ABoxSpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABoxSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void ABoxSpawner::SpawnBoxes(UJSONParser* Parser, UWorld* World)
{

    if (!Parser || !World) return;


    if (!CubeMesh) return;

    for (const FBoxInstanceData& Instance : Parser->BoxInstances)
    {
        const FBoxTypeData* TypeData = FindBoxType(Instance.TypeName, Parser->BoxTypes);
        if (!TypeData) continue;

        FActorSpawnParameters Params;
        Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

        AStaticMeshActor* BoxActor = World->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(),Instance.Location,Instance.Rotation,Params);

        if (!BoxActor) continue;

        UStaticMeshComponent* MeshComp = BoxActor->GetStaticMeshComponent();
        if (!MeshComp) continue;

      
        MeshComp->SetMobility(EComponentMobility::Movable);

        // Assign mesh
        MeshComp->SetStaticMesh(CubeMesh);
        MeshComp->SetWorldScale3D(Instance.Scale);

        //  set Default material 
        if (DefaultBoxMaterial)
        {
            MeshComp->SetMaterial(0, DefaultBoxMaterial);
        }
        else
        {
            MeshComp->SetMaterial(0, UMaterial::GetDefaultMaterial(MD_Surface));
        }

        // Now apply dynamic instance
        CreateBoxMaterial(MeshComp, TypeData->Color);


        // Store spawned box info
        FSpawnedBox Spawned;
        Spawned.BoxActor = BoxActor;
        Spawned.Health = TypeData->Health;
        Spawned.Score = TypeData->Score;
        SpawnedBoxes.Add(Spawned);
    }

    UE_LOG(LogTemp, Log, TEXT("BoxSpawner: Spawned %d boxes"), SpawnedBoxes.Num());
}

bool ABoxSpawner::DamageBox(AActor* HitBox, int32& OutScore)
{
    OutScore = 0; 

    if (!HitBox) return false;

    for (int32 i = 0; i < SpawnedBoxes.Num(); ++i)
    {
        if (SpawnedBoxes[i].BoxActor == HitBox)
        {
            SpawnedBoxes[i].Health -= 1;

            // DamageBox 
            if (SpawnedBoxes[i].Health <= 0)
            {
                OutScore = SpawnedBoxes[i].Score;
                UE_LOG(LogTemp, Log, TEXT("Box destroyed: %s | Awarded %d points"), *HitBox->GetName(), OutScore);

                HitBox->Destroy();
                SpawnedBoxes.RemoveAt(i);
                return true;
            }
            else
            {
                UE_LOG(LogTemp, Log, TEXT("Box damaged: %s | Remaining Health: %d"),*HitBox->GetName(), SpawnedBoxes[i].Health);
                return true;
            }
           
        }

    }

    return false; 
}



const FBoxTypeData* ABoxSpawner::FindBoxType(const FString& TypeName, const TArray<FBoxTypeData>& BoxTypes)
{
    for (const FBoxTypeData& Type : BoxTypes)
    {
        if (Type.Name == TypeName)
            return &Type;
    }
    return nullptr;
}

UMaterialInstanceDynamic* ABoxSpawner::CreateBoxMaterial(UStaticMeshComponent* MeshComp, const FLinearColor& Color)
{
    if (!MeshComp) return nullptr;

    UMaterialInstanceDynamic* DynMat = MeshComp->CreateAndSetMaterialInstanceDynamic(0);
    
    // CreateBoxMaterial
    if (DynMat)
    {
        DynMat->SetVectorParameterValue(FName("BaseColor"), Color);
        UE_LOG(LogTemp, Log, TEXT("BoxSpawner: Applied BaseColor %s to %s"),*Color.ToString(),*MeshComp->GetOwner()->GetName());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("BoxSpawner: Failed to create dynamic material!"));
    }

    return DynMat;
}
