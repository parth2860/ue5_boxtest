// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Fps_Box/Core/JSONParser.h"
#include "BoxSpawner.generated.h"

USTRUCT()
struct FSpawnedBox
{
	GENERATED_BODY()

	UPROPERTY()
	AActor* BoxActor = nullptr;

	UPROPERTY()
	int32 Health = 0;

	UPROPERTY()
	int32 Score = 0;
};

UCLASS()
class UE5_FPSBOX_API ABoxSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoxSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite)
	UStaticMesh* CubeMesh;

	// Default base material 
	UPROPERTY()
	UMaterial* DefaultBoxMaterial;

	// Spawn boxes based on parser data
	UFUNCTION()
	void SpawnBoxes(UJSONParser* Parser, UWorld* World);

	// Apply damage to box
	UFUNCTION()
	bool DamageBox(AActor* HitBox, int32& OutScore);

	// Access all spawned boxes
	UPROPERTY()
	TArray<FSpawnedBox> SpawnedBoxes;

	// find box type data
	const FBoxTypeData* FindBoxType(const FString& TypeName, const TArray<FBoxTypeData>& BoxTypes);

	// create dynamic material
	UMaterialInstanceDynamic* CreateBoxMaterial(UStaticMeshComponent* MeshComp, const FLinearColor& Color);
	

};
