// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "LyraCosmeticAnimationTypes.generated.h"

USTRUCT(BlueprintType)
struct FLyraAnimBodyStyleSelectionEntry
{
	GENERATED_BODY()

	// Anim Layer를 적용할 대상 SkeletalMesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMesh> Mesh = nullptr;

	// Cosmetic Tag
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Cosmetic"))
	FGameplayTagContainer RequiredTags;
};

USTRUCT(BlueprintType)
struct FLyraAnimBodyStyleSelectionSet
{
	GENERATED_BODY()

	// GameplayTag를 통해 MeshRules에 알맞은 BodyStyle을 선택한다.
	USkeletalMesh* SelectBestBodyStyle(const FGameplayTagContainer& CosmeticTags) const;
	
	// AnimLayer 적용할 SkeletalMesh를 들고 있음 -> Animation-Mesh 간 Rule
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FLyraAnimBodyStyleSelectionEntry> MeshRules;

	// 디폴트로 적용할 SkeletalMesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMesh> DefaultMesh = nullptr;

	// Physics Asset은 하나로 동일함
	UPROPERTY(EditAnywhere)
	TObjectPtr<UPhysicsAsset> ForcedPhysicsAsset = nullptr;
};