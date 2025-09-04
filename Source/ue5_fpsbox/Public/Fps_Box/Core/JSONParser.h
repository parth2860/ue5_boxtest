// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "JSONParser.generated.h"

// Box type definition (color, health, score)
USTRUCT(BlueprintType)
struct FBoxTypeData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString Name;

    UPROPERTY(BlueprintReadOnly)
    FLinearColor Color;

    UPROPERTY(BlueprintReadOnly)
    int32 Health = 0;

    UPROPERTY(BlueprintReadOnly)
    int32 Score = 0;
};

// Box instance definition (type + transform)
USTRUCT(BlueprintType)
struct FBoxInstanceData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString TypeName;

    UPROPERTY(BlueprintReadOnly)
    FVector Location;

    UPROPERTY(BlueprintReadOnly)
    FRotator Rotation;

    UPROPERTY(BlueprintReadOnly)
    FVector Scale;
};

UCLASS()
class UE5_FPSBOX_API UJSONParser : public UObject
{
	GENERATED_BODY()
	
public:
    // Store parsed data
    UPROPERTY(BlueprintReadOnly)
    TArray<FBoxTypeData> BoxTypes;

    UPROPERTY(BlueprintReadOnly)
    TArray<FBoxInstanceData> BoxInstances;

    // Main entry point
    bool ParseJSON(const FString& JsonString);
    // Debug function
    void DebugParsedData();

private:
    bool ParseTypes(const TSharedPtr<FJsonObject>& RootObj);
    bool ParseObjects(const TSharedPtr<FJsonObject>& RootObj);
};
