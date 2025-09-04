// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "WebManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnJSONReady, const FString&, JsonContent);

UCLASS()
class UE5_FPSBOX_API UWebManager : public UObject
{
	GENERATED_BODY()
	
public:
    UPROPERTY(BlueprintAssignable)
    FOnJSONReady OnJSONReady;

    // Initialize and start fetching JSON from the URL
    UFUNCTION()
    void Initialize();

    // Check if JSON has been successfully fetched
    UFUNCTION()
    bool IsJSONReady() const { return bIsJSONReady; }

    // Get the fetched JSON string
    UFUNCTION()
    FString GetJSON() const { return FetchedJSON; }

private:
    void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

    FString FetchedJSON;    // Stored JSON string
    bool bIsJSONReady = false;

    const FString URL = TEXT("https://raw.githubusercontent.com/CyrusCHAU/Varadise-Technical-Test/refs/heads/main/data.json");

};
