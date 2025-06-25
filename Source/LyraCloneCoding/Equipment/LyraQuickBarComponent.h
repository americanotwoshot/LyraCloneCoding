// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ControllerComponent.h"
#include "LyraQuickBarComponent.generated.h"

class ULyraEquipmentInstance;
class ULyraInventoryItemInstance;
/**
 * HUD의 Quick Bar 역할
 * Inventory와 Equipment의 다리 역할하는 Component
 */
UCLASS()
class LYRACLONECODING_API ULyraQuickBarComponent : public UControllerComponent
{
	GENERATED_BODY()
public:
	ULyraQuickBarComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// HUD QuickBar Slot 개수
	UPROPERTY()
	int32 NumSlots = 3;

	// HUD QuickBar Slot 리스트
	TArray<TObjectPtr<ULyraInventoryItemInstance>> Slots;

	// 현재 활성화된 Slot Index
	UPROPERTY()
	int32 ActiveSlotIndex = -1;

	// 현재 장착한 장비 정보
	UPROPERTY()
	TObjectPtr<ULyraEquipmentInstance> EquippedItem;
};
