// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LyraCloneCoding/Cosmetics/LyraCosmeticAnimationTypes.h"
#include "LyraCloneCoding/Equipment/LyraEquipmentInstance.h"
#include "LyraWeaponInstance.generated.h"

/**
 * 
 */
UCLASS()
class LYRACLONECODING_API ULyraWeaponInstance : public ULyraEquipmentInstance
{
	GENERATED_BODY()
public:
	ULyraWeaponInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
	FLyraAnimLayerSelectionSet EquippedAnimSet;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
	FLyraAnimLayerSelectionSet UnequippedAnimSet;
};
