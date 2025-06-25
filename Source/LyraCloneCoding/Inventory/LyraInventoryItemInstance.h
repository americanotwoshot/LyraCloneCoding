// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LyraInventoryItemInstance.generated.h"

class ULyraInventoryItemDefinition;
/**
 * 
 */
UCLASS()
class LYRACLONECODING_API ULyraInventoryItemInstance : public UObject
{
	GENERATED_BODY()
public:
	ULyraInventoryItemInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY()
	TSubclassOf<ULyraInventoryItemDefinition> ItemDef;
};
