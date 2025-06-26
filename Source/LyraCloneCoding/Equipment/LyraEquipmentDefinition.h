// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LyraEquipmentDefinition.generated.h"

class ULyraEquipmentInstance;

USTRUCT()
struct FLyraEquipmentActorToSpawn
{
	GENERATED_BODY()

	// Spawn할 대상 Actor 클래스
	UPROPERTY(EditAnywhere, Category = Equipment)
	TSubclassOf<AActor> ActorToSpawn;

	// 어느 Bone Socket에 붙일지 결정
	UPROPERTY(EditAnywhere, Category = Equipment)
	FName AttachSocket;

	// Socket에서 어느정도 Transform을 더해줄지 결정
	UPROPERTY(EditAnywhere, Category = Equipment)
	FTransform AttachTransform;
};

/**
 * 
 */
UCLASS(Blueprintable)
class LYRACLONECODING_API ULyraEquipmentDefinition : public UObject
{
	GENERATED_BODY()
public:
	ULyraEquipmentDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(EditDefaultsOnly, Category = Equipment)
	TSubclassOf<ULyraEquipmentInstance> InstanceType;
	
	UPROPERTY(EditDefaultsOnly, Category = Equipment)
	TArray<FLyraEquipmentActorToSpawn> ActorsToSpawn;
};
