// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LyraInventoryManagerComponent.generated.h"

class ULyraInventoryItemInstance;

USTRUCT()
struct FLyraInventoryEntry
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<ULyraInventoryItemInstance> Instance = nullptr;
};

/** Inventory Item 관리 객체 */
USTRUCT(BlueprintType)
struct FLyraInventoryList
{
	GENERATED_BODY()

	FLyraInventoryList(UActorComponent* InOwnerComponent = nullptr)
		: OwnerComponent(InOwnerComponent)
	{}

	UPROPERTY()
	TArray<FLyraInventoryEntry> Entries;

	UPROPERTY()
	TObjectPtr<UActorComponent> OwnerComponent;
};

/** PlayerController의 Component로서 Inventory를 관리한다. */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LYRACLONECODING_API ULyraInventoryManagerComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	ULyraInventoryManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	UPROPERTY()
	FLyraInventoryList InventoryList;
};
