// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LyraExperienceDefinition.generated.h"

class ULyraPawnData;

/**
 * 
 */
UCLASS()
class LYRACLONECODING_API ULyraExperienceDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	ULyraExperienceDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	TObjectPtr<ULyraPawnData> DefaultPawnData;

	// 게임 모드에 따른 GameFeature plugin을 로딩하는데, 이에 대한 연결고리
	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	TArray<FString> GameFeaturesToEnable;
};
