// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameStateComponent.h"
#include "GameFeaturePluginOperationResult.h"
#include "LyraExperienceManagerComponent.generated.h"

class ULyraExperienceDefinition;

enum class ELyraExperienceLoadState
{
	Unloaded,
	Loading,
	LoadingGameFeatures,
	ExecutingActions,
	Loaded,
	Deactivating,
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnLyraExperienceLoaded, const ULyraExperienceDefinition*);

/**
 * game state를 owner로 가지면서, experience 상태 정보를 가지고 있는 component이다.
 * 뿐만 아니라, experience 로딩 상태 업데이트 및 이벤트를 관리한다.
 */
UCLASS()
class LYRACLONECODING_API ULyraExperienceManagerComponent : public UGameStateComponent
{
	GENERATED_BODY()

public:
	bool IsExperienceLoaded() {return (LoadState == ELyraExperienceLoadState::Loaded) && (CurrentExperience != nullptr); }

	// OnExperienceLoaded에 바인딩하거나, 이미 Experience 로딩이 완료되었다면 바로 호출함
	void CallOrRegister_OnExperienceLoaded(FOnLyraExperienceLoaded::FDelegate&& Delegate);

	void ServerSetCurrentExperience(FPrimaryAssetId ExperienceId);
	void StartExperienceLoad();
	void OnExperienceLoadComplete();
	void OnGameFeaturePluginLoadComplete(const UE::GameFeatures::FResult& Result);
	void OnExperienceFullLoadCompleted();
	const ULyraExperienceDefinition* GetCurrentExperienceChecked() const;
	
public:
	UPROPERTY()
	TObjectPtr<const ULyraExperienceDefinition> CurrentExperience;

	// Experience의 로딩 상태를 모니터링
	ELyraExperienceLoadState LoadState = ELyraExperienceLoadState::Unloaded;

	// Experience 로딩이 완료된 이후, Broadcasting Delegate
	FOnLyraExperienceLoaded OnExperienceLoaded;

	// 활성화된 GameFeature Plugin들
	int32 NumGameFeaturePluginsLoading = 0;
	TArray<FString> GameFeaturePluginURLs;
};
