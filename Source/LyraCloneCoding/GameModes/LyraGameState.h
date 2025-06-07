// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "LyraGameState.generated.h"

class ULyraExperienceManagerComponent;
/**
 * 
 */
UCLASS()
class LYRACLONECODING_API ALyraGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	ALyraGameState();

	UPROPERTY()
	TObjectPtr<ULyraExperienceManagerComponent> ExperienceManagerComponent;
};
