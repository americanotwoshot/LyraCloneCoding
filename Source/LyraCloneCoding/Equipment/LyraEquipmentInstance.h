// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LyraEquipmentInstance.generated.h"

struct FLyraEquipmentActorToSpawn;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class LYRACLONECODING_API ULyraEquipmentInstance : public UObject
{
	GENERATED_BODY()
public:
	ULyraEquipmentInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/**
	 * Blueprint 정의를 위한 Equip/Unequip 함수
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = Equipment, meta = (DisplayName = "OnEquipped"))
	void K2_OnEquipped();

	UFUNCTION(BlueprintImplementableEvent, Category = Equipment, meta = (DisplayName = "OnUnequipped"))
	void K2_OnUnequipped();

	UFUNCTION(BlueprintPure, Category = Equipment)
	APawn* GetPawn() const;

	/**
	 * member methods
	 */
	void SpawnEquipmentActors(const TArray<FLyraEquipmentActorToSpawn>& ActorsToSpawn);
	void DestroyEquipmentActors();
	
	/**
	 * interfaces
	 */
	virtual void OnEquipped();
	virtual void OnUnequipped();
	
	// 어떤 InventoryItemInstance에 의해 활성화되었는지
	UPROPERTY()
	TObjectPtr<UObject> Instigator;

	// LyraEquipmentDefinition에 맞게 Spawn된 Actor Instance들 
	UPROPERTY()
	TArray<TObjectPtr<AActor>> SpawnedActors;
};
