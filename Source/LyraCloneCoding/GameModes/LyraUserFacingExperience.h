// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LyraUserFacingExperience.generated.h"

/**
 *	User가 마주하는 Experience 정보에 대해 저장 및 관리
 */
UCLASS()
class LYRACLONECODING_API ULyraUserFacingExperience : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	/** the specific map to load */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Experience, meta = (AllowedTypes = "Map"))
	FPrimaryAssetId MapID;

	/** the gameplay experience to load */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Experience, meta = (AllowedTypes = "LyraExperienceDefinition"))
	FPrimaryAssetId ExperienceID;
};
