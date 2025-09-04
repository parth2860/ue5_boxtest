// Fill out your copyright notice in the Description page of Project Settings.


#include "Fps_Box/Core/JSONParser.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

bool UJSONParser::ParseJSON(const FString& JsonString)
{
    BoxTypes.Empty();
    BoxInstances.Empty();

    TSharedPtr<FJsonObject> RootObj;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

    if (!FJsonSerializer::Deserialize(Reader, RootObj) || !RootObj.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("JsonParser: Failed to parse JSON"));
        return false;
    }

    bool bTypesOk = ParseTypes(RootObj);
    bool bObjectsOk = ParseObjects(RootObj);

    return bTypesOk && bObjectsOk;
}

bool UJSONParser::ParseTypes(const TSharedPtr<FJsonObject>& RootObj)
{
    const TArray<TSharedPtr<FJsonValue>>* TypesArray;
    if (!RootObj->TryGetArrayField(TEXT("types"), TypesArray)) return false;

    for (const TSharedPtr<FJsonValue>& Value : *TypesArray)
    {
        const TSharedPtr<FJsonObject>* Obj;
        if (Value->TryGetObject(Obj))
        {
            FBoxTypeData TypeData;
            TypeData.Name = (*Obj)->GetStringField(TEXT("name"));

            const TArray<TSharedPtr<FJsonValue>>* ColorArray;
            if ((*Obj)->TryGetArrayField(TEXT("color"), ColorArray) && ColorArray->Num() == 3)
            {
                TypeData.Color = FLinearColor(
                    (*ColorArray)[0]->AsNumber() / 255.f,
                    (*ColorArray)[1]->AsNumber() / 255.f,
                    (*ColorArray)[2]->AsNumber() / 255.f
                );
            }

            TypeData.Health = (*Obj)->GetIntegerField(TEXT("health"));
            TypeData.Score = (*Obj)->GetIntegerField(TEXT("score"));

            BoxTypes.Add(TypeData);
        }
    }

    return true;
}

bool UJSONParser::ParseObjects(const TSharedPtr<FJsonObject>& RootObj)
{
    const TArray<TSharedPtr<FJsonValue>>* ObjArray;
    if (!RootObj->TryGetArrayField(TEXT("objects"), ObjArray)) return false;

    for (const TSharedPtr<FJsonValue>& Value : *ObjArray)
    {
        const TSharedPtr<FJsonObject>* Obj;
        if (Value->TryGetObject(Obj))
        {
            FBoxInstanceData Instance;
            Instance.TypeName = (*Obj)->GetStringField(TEXT("type"));

            const TSharedPtr<FJsonObject>* TransformObj;
            if ((*Obj)->TryGetObjectField(TEXT("transform"), TransformObj))
            {
                auto ToVector = [](const TArray<TSharedPtr<FJsonValue>>& Arr) -> FVector
                    {
                        return FVector(
                            Arr[0]->AsNumber(),
                            Arr[1]->AsNumber(),
                            Arr[2]->AsNumber()
                        );
                    };

                const TArray<TSharedPtr<FJsonValue>>* LocArray;
                if ((*TransformObj)->TryGetArrayField(TEXT("location"), LocArray) && LocArray->Num() == 3)
                {
                    Instance.Location = ToVector(*LocArray);
                }

                const TArray<TSharedPtr<FJsonValue>>* RotArray;
                if ((*TransformObj)->TryGetArrayField(TEXT("rotation"), RotArray) && RotArray->Num() == 3)
                {
                    Instance.Rotation = FRotator(
                        (*RotArray)[0]->AsNumber(), // Pitch
                        (*RotArray)[1]->AsNumber(), // Yaw
                        (*RotArray)[2]->AsNumber()  // Roll
                    );
                }

                const TArray<TSharedPtr<FJsonValue>>* ScaleArray;
                if ((*TransformObj)->TryGetArrayField(TEXT("scale"), ScaleArray) && ScaleArray->Num() == 3)
                {
                    Instance.Scale = ToVector(*ScaleArray);
                }
            }

            BoxInstances.Add(Instance);
        }
    }

    return true;
}
void UJSONParser::DebugParsedData()
{
    UE_LOG(LogTemp, Log, TEXT("---- Parsed Box Types ----"));
    for (const FBoxTypeData& Type : BoxTypes)
    {
        UE_LOG(LogTemp, Log, TEXT("Type=%s, Color=(%f,%f,%f), Health=%d, Score=%d"),*Type.Name, Type.Color.R, Type.Color.G, Type.Color.B, Type.Health, Type.Score);
    }

    UE_LOG(LogTemp, Log, TEXT("---- Parsed Box Instances ----"));
    for (const FBoxInstanceData& Instance : BoxInstances)
    {
        UE_LOG(LogTemp, Log, TEXT("Type=%s, Loc=(%s), Rot=(%s), Scale=(%s)"),*Instance.TypeName,*Instance.Location.ToString(),*Instance.Rotation.ToString(),*Instance.Scale.ToString());
    }
}
