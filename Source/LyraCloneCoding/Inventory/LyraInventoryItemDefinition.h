// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LyraInventoryItemDefinition.generated.h"

UCLASS(Abstract, DefaultToInstanced, EditInlineNew)
class ULyraInventoryItemFragment : public UObject
{
	GENERATED_BODY()
public:
};

/**
 * 
 */
UCLASS(Blueprintable)
class LYRACLONECODING_API ULyraInventoryItemDefinition : public UObject
{
	GENERATED_BODY()
public:
	ULyraInventoryItemDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// Inventory Item 정의(메타) 이름
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Display)
	FText DisplayName;

	// Inventory Item의 Component를 Fragment로 인식하면 됨
	UPROPERTY(EditDefaultsOnly, Instanced, BlueprintReadOnly, Category = Display)
	TArray<TObjectPtr<ULyraInventoryItemFragment>> Fragments;
};
