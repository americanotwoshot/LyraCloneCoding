// Fill out your copyright notice in the Description page of Project Settings.

//
// Deprecated in version 5.5
//
#pragma once

#include "CoreMinimal.h"
#include "LyraMappableConfigPair.generated.h"

class UPlayerMappableInputConfig;

USTRUCT()
struct FLyraMappableConfigPair
{
	GENERATED_BODY()
public:
	FLyraMappableConfigPair() = default;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UPlayerMappableInputConfig> Config;

	UPROPERTY(EditAnywhere)
	bool bShouldActivateAutomatically = true;
};