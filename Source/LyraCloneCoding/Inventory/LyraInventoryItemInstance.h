// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LyraInventoryItemInstance.generated.h"

class ULyraInventoryItemFragment;
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

	const ULyraInventoryItemFragment* FindFragmentByClass(TSubclassOf<ULyraInventoryItemFragment> FragmentClass) const;

	template <typename ResultClass>
	const ResultClass* FindFragmentByClass() const
	{
		return (ResultClass*)FindFragmentByClass(ResultClass::StaticClass());
	}

	// Inventory Item의 인스턴스에서는 무엇으로 정의되었는지, 메타 클래스를 가지고 있다.
	UPROPERTY()
	TSubclassOf<ULyraInventoryItemDefinition> ItemDef;
};