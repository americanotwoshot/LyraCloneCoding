// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LyraExperienceDefinition.generated.h"

class UGameFeatureAction;
class ULyraExperienceActionSet;
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

	/**
	 * member variables
	 */
	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	TObjectPtr<ULyraPawnData> DefaultPawnData;

	// 게임 모드에 따른 GameFeature plugin을 로딩하는데, 이에 대한 연결고리
	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	TArray<FString> GameFeaturesToEnable;

	// GameFeatureAction의 Set이며, Gameplay 용도에 맞게 분류의 목적으로 사용
	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	TArray<TObjectPtr<ULyraExperienceActionSet>> ActionSets;

	// 일반적인 GameFeatureAction으로 추가
	UPROPERTY(EditDefaultsOnly, Category = "Actions")
	TArray<TObjectPtr<UGameFeatureAction>> Actions;
};
