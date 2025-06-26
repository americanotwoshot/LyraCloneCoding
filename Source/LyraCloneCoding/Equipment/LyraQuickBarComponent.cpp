// Fill out your copyright notice in the Description page of Project Settings.


#include "LyraQuickBarComponent.h"

#include "LyraEquipmentInstance.h"
#include "LyraEquipmentManagerComponent.h"
#include "LyraEquipmentDefinition.h"
#include "LyraCloneCoding/Inventory/LyraInventoryFragment_EquippableItem.h"
#include "LyraCloneCoding/Inventory/LyraInventoryItemInstance.h"

ULyraQuickBarComponent::ULyraQuickBarComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void ULyraQuickBarComponent::BeginPlay()
{
	// NumSlots에 따라 미리 Slot을 할당
	if (Slots.Num() < NumSlots)
	{
		Slots.AddDefaulted(NumSlots - Slots.Num());
	}
	
	Super::BeginPlay();
}

ULyraEquipmentManagerComponent* ULyraQuickBarComponent::FindEquipmentManager() const
{
	if (AController* OwnerController = Cast<AController>(GetOwner()))
	{
		if (APawn* Pawn = OwnerController->GetPawn())
		{
			return Pawn->FindComponentByClass<ULyraEquipmentManagerComponent>();
		}
	}
	
	return nullptr;
}

void ULyraQuickBarComponent::UnequipItemInSlot()
{
	if (ULyraEquipmentManagerComponent* EquipmentManager = FindEquipmentManager())
	{
		if (EquippedItem)
		{
			// EquipmentManager를 통해 Pawn의 장비를 해제한다.
			EquipmentManager->UnequipItem(EquippedItem);

			EquippedItem = nullptr;
		}
	}
}

void ULyraQuickBarComponent::EquipItemInSlot()
{
	check(Slots.IsValidIndex(ActiveSlotIndex));
	check(EquippedItem == nullptr);

	if (ULyraInventoryItemInstance* SlotItem = Slots[ActiveSlotIndex])
	{
		if (const ULyraInventoryFragment_EquippableItem* EquipInfo = SlotItem->FindFragmentByClass<ULyraInventoryFragment_EquippableItem>())
		{
			TSubclassOf<ULyraEquipmentDefinition> EquipDef = EquipInfo->EquipmentDefinition;
			if (EquipDef)
			{
				if (ULyraEquipmentManagerComponent* EquipmentManager = FindEquipmentManager())
				{
					EquippedItem = EquipmentManager->EquipItem(EquipDef);

					if (EquippedItem)
					{
						EquippedItem->Instigator = SlotItem;
					}
				}
			}
		}
	}
}

void ULyraQuickBarComponent::AddItemToSlot(int32 SlotIndex, ULyraInventoryItemInstance* Item)
{
	// 동적 추가가 아니라 인덱스로 직접 넣는다.
	if (Slots.IsValidIndex(SlotIndex) && (Item != nullptr))
	{
		if (Slots[SlotIndex] == nullptr)
		{
			Slots[SlotIndex] = Item;
		}
	}
}

void ULyraQuickBarComponent::SetActiveSlotIndex(int32 NewIndex)
{
	if (Slots.IsValidIndex(NewIndex) && (ActiveSlotIndex != NewIndex))
	{
		UnequipItemInSlot();
		ActiveSlotIndex = NewIndex;
		EquipItemInSlot();
	}
}
