// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LyraInventoryItemDefinition.h"
#include "LyraInventoryFragment_EquippableItem.generated.h"


class ULyraEquipmentDefinition;
/**
 * 
 */
UCLASS()
class ULyraInventoryFragment_EquippableItem : public ULyraInventoryItemFragment
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = Lyra)
	TSubclassOf<ULyraEquipmentDefinition> EquipmentDefinition;
};
