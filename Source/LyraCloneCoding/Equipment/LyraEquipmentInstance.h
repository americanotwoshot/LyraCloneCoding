// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LyraEquipmentInstance.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class LYRACLONECODING_API ULyraEquipmentInstance : public UObject
{
	GENERATED_BODY()
public:
	ULyraEquipmentInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// 어떤 InventoryItemInstance에 의해 활성화되었는지
	UPROPERTY()
	TObjectPtr<UObject> Instigator;

	// LyraEquipmentDefinition에 맞게 Spawn된 Actor Instance들 
	UPROPERTY()
	TArray<TObjectPtr<AActor>> SpawnedActors;
};
