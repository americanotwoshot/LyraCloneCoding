// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "LyraAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class LYRACLONECODING_API ULyraAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "Character State Data")
	float GroundDistance = -1.0f;
};
