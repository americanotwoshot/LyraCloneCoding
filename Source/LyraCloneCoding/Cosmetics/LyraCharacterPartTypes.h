// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LyraCharacterPartTypes.generated.h"

USTRUCT(BlueprintType)
struct FLyraCharacterPartHandle
{
	GENERATED_BODY()

	void Reset()
	{
		PartHandle = INDEX_NONE;
	}

	bool IsValid() const
	{
		return PartHandle != INDEX_NONE;
	}

	UPROPERTY()
	int32 PartHandle = INDEX_NONE;
};

USTRUCT(BlueprintType)
struct FLyraCharacterPart
{
	GENERATED_BODY()

	// 파츠로 생성된 Actor의 Class 정보를 가지고 있음
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> PartClass;

	// Actor를 소환하여 붙이는데 특정 Bone에 붙이기 위한 정보
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SocketName;
};