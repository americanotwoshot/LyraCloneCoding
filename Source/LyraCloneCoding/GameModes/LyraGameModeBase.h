// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LyraGameModeBase.generated.h"

class ULyraPawnData;
class ULyraExperienceDefinition;
/**
 * 
 */
UCLASS()
class LYRACLONECODING_API ALyraGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ALyraGameModeBase();

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void InitGameState() final;

	/** GetDefaultPawnClassForController */
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) final;
	
	/** HandleStartingNewPlayer */
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) final;

	/** SpawnDefaultPawnAtTransform */
	virtual APawn* SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform) final;
	
	// Experience 로딩을 진행하기 위한 함수
	void HandleMatchAssignmentIfNotExpectingOne();
	
	void OnMatchAssignmentGiven(FPrimaryAssetId ExperienceId)const;
	bool IsExperienceLoaded() const;
	void OnExperienceLoaded(const ULyraExperienceDefinition* CurrentExperience);
	const ULyraPawnData* GetPawnDataForController(const AController* InController) const;
};