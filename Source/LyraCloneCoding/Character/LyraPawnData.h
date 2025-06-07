// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LyraPawnData.generated.h"

/**
 * 
 */
UCLASS()
class LYRACLONECODING_API ULyraPawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	ULyraPawnData(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Lyra|Pawn")
	TSubclassOf<APawn> PawnClass;
};
