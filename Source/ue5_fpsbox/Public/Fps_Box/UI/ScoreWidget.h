// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"   
#include "Components/TextBlock.h"
#include "ScoreWidget.generated.h"

/**
 * 
 */
UCLASS()
class UE5_FPSBOX_API UScoreWidget : public UUserWidget
{
	GENERATED_BODY()
public:
    // ScoreWidget
    UPROPERTY(meta = (BindWidget))
    UTextBlock* ScoreText;

    // Function to update the score
    UFUNCTION(BlueprintCallable)
    void UpdateScore(int32 NewScore);
};
