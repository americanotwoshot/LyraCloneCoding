// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "LyraEquipmentManagerComponent.generated.h"

class ULyraEquipmentInstance;
class ULyraEquipmentDefinition;

USTRUCT()
struct FLyraAppliedEquipmentEntry
{
	GENERATED_BODY()

	// 장착물에 대한 메타 데이터
	UPROPERTY()
	TSubclassOf<ULyraEquipmentDefinition> EquipmentDefinition;

	// EquipmentDefinition을 통해 생성된 인스턴스
	UPROPERTY()
	TObjectPtr<ULyraEquipmentInstance> Instance = nullptr;
};

USTRUCT()
struct FLyraEquipmentList
{
	GENERATED_BODY()
	
	FLyraEquipmentList(UActorComponent* InOwnerComponent = nullptr)
		: OwnerComponent(InOwnerComponent)
	{}

	ULyraEquipmentInstance* AddEntry(TSubclassOf<ULyraEquipmentDefinition> EquipmentDefinition);
	void RemoveEntry(ULyraEquipmentInstance* Instance);

	// 장착물에 대한 관리 리스트
	UPROPERTY()
	TArray<FLyraAppliedEquipmentEntry> Entries;
	
	UPROPERTY()
	TObjectPtr<UActorComponent> OwnerComponent;
};

/**
 * 
 */
UCLASS()
class LYRACLONECODING_API ULyraEquipmentManagerComponent : public UPawnComponent
{
	GENERATED_BODY()
public:
	ULyraEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	ULyraEquipmentInstance* EquipItem(TSubclassOf<ULyraEquipmentDefinition> EquipmentDefinition);
	void UnequipItem(ULyraEquipmentInstance* ItemInstance);
	
	UPROPERTY()
	FLyraEquipmentList EquipmentList;
};
