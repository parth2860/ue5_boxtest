// Fill out your copyright notice in the Description page of Project Settings.


#include "Fps_Box/Core/WebManager.h"
#include "Engine/Engine.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

void UWebManager::Initialize()
{
    if (URL.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("WebManager: Assignment URL is empty."));
        return;
    }

    // Create HTTP request
    TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
    HttpRequest->SetURL(URL);
    HttpRequest->SetVerb(TEXT("GET"));
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &UWebManager::OnResponseReceived);
    HttpRequest->ProcessRequest();

    UE_LOG(LogTemp, Log, TEXT("WebManager: HTTP request sent to %s"), *URL);
}

void UWebManager::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    if (!bWasSuccessful || !Response.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("WebManager: Failed to fetch JSON."));
        return;
    }

    FetchedJSON = Response->GetContentAsString();

    UE_LOG(LogTemp, Log, TEXT("WebManager: JSON successfully fetched. Length: %d"), FetchedJSON.Len());

    // Broadcast event
    OnJSONReady.Broadcast(FetchedJSON);
}