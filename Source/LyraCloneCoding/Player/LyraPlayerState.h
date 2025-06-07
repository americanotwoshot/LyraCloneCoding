// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "LyraPlayerState.generated.h"

class ULyraPawnData;
class ULyraExperienceDefinition;
/**
 * 
 */
UCLASS()
class LYRACLONECODING_API ALyraPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	// AActor's interface
	virtual void PostInitializeComponents() final;

	void OnExperienceLoaded(const ULyraExperienceDefinition* CurrentExperience);

	template <class T>
	const T* GetPawnData() const { return Cast<T>(PawnData); }
	void SetPawnData(const ULyraPawnData* InPawnData);
	
	UPROPERTY()
	TObjectPtr<const ULyraPawnData> PawnData;
};
