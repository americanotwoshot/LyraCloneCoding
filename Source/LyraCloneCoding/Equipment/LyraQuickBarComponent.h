// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ControllerComponent.h"
#include "LyraQuickBarComponent.generated.h"

class ULyraEquipmentManagerComponent;
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

	/**
	 * ControllerComponent Interface
	 */
	virtual void BeginPlay() override;

	/**
	 * member methods
	 */
	ULyraEquipmentManagerComponent* FindEquipmentManager() const;
	void UnequipItemInSlot();
	void EquipItemInSlot();
	
	// Inventory에 저장되어있는 아이템 인스턴스를 퀵바에 추가하는 함수
	UFUNCTION(BlueprintCallable)
	void AddItemToSlot(int32 SlotIndex, ULyraInventoryItemInstance* Item);

	UFUNCTION(BlueprintCallable, Category = "Lyra")
	void SetActiveSlotIndex(int32 NewIndex);
	
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
