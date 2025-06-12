// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LyraCameraMode.h"
#include "LyraCameraMode_ThirdPerson.generated.h"

class UCurveVector;
/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class LYRACLONECODING_API ULyraCameraMode_ThirdPerson : public ULyraCameraMode
{
	GENERATED_BODY()

public:
	ULyraCameraMode_ThirdPerson(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/**
	 * ULyraCameraMode interface
	 */
	virtual void UpdateView(float DeltaTime) override;
	
	/**
	 * member variables
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Third Person")
	TObjectPtr<const UCurveVector> TargetOffsetCurve;
};
