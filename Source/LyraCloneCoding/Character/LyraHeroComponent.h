// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameFrameworkInitStateInterface.h"
#include "Components/PawnComponent.h"
#include "LyraHeroComponent.generated.h"

class ULyraCameraMode;
/**
 * 카메라, 입력 등 플레이어가 제어하는 시스템의 초기화를 처리하는 컴포넌트
 */
UCLASS(Blueprintable, Meta = (BlueprintSpawnableComponent))
class LYRACLONECODING_API ULyraHeroComponent : public UPawnComponent, public IGameFrameworkInitStateInterface
{
	GENERATED_BODY()

public:
	ULyraHeroComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// FeatureName 정의
	static const FName NAME_ActorFeatureName;

	/**
	 * UPawnComponent interfaces
	 */ 
	virtual void OnRegister() final;
	virtual void BeginPlay() final;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) final;

	/**
	 * IGameFrameworkInitStateInterface
	 */ 
	virtual FName GetFeatureName() const final { return NAME_ActorFeatureName; }
	virtual void OnActorInitStateChanged(const FActorInitStateChangedParams& Params) final;
	virtual bool CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const final;
	virtual void HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) final;
	virtual void CheckDefaultInitialization() final;

	/**
	 * member methods
	 */
	TSubclassOf<ULyraCameraMode> DetermineCameraMode() const;
};
