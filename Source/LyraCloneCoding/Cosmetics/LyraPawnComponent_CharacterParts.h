// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LyraCharacterPartTypes.h"
#include "LyraCosmeticAnimationTypes.h"
#include "Components/PawnComponent.h"
#include "LyraPawnComponent_CharacterParts.generated.h"

class ULyraPawnComponent_CharacterParts;

/** 인스턴스화 된 Character Part의 단위 */
USTRUCT()
struct FLyraAppliedCharacterPartEntry
{
	GENERATED_BODY()

	// Character Part의 정의 (메타 데이터)
	UPROPERTY()
	FLyraCharacterPart Part;

	// LyraCharacterPartList에서 할당받은 Part 핸들 값
	UPROPERTY()
	int32 PartHandle = INDEX_NONE;

	// 인스턴스화 된 Character Part용 Actor
	UPROPERTY()
	TObjectPtr<UChildActorComponent> SpawnedComponent = nullptr;
};

/** LyraPawnComponent_CharacterParts에서 실질적 Character Parts를 관리하는 구조체 */
USTRUCT(BlueprintType)
struct FLyraCharacterPartList
{
	GENERATED_BODY()

	FLyraCharacterPartList()
		: OwnerComponent(nullptr)
	{}

	FLyraCharacterPartList(ULyraPawnComponent_CharacterParts* InOwnerComponent)
		: OwnerComponent(InOwnerComponent)
	{}

	bool SpawnActorForEntry(FLyraAppliedCharacterPartEntry& Entry);
	void DestroyActorForEntry(FLyraAppliedCharacterPartEntry& Entry);
	
	FLyraCharacterPartHandle AddEntry(FLyraCharacterPart NewPart);
	void RemoveEntry(FLyraCharacterPartHandle Handle);
	
	FGameplayTagContainer CollectCombinedTags() const;
	
	// 현재 인스턴스화된 Character Part
	UPROPERTY()
	TArray<FLyraAppliedCharacterPartEntry> Entries;
	
	UPROPERTY()
	TObjectPtr<ULyraPawnComponent_CharacterParts> OwnerComponent;

	// PartHandle의 값을 할당 및 관리하는 변수
	int32 PartHandleCounter = 0;
};

/**
 * 
 */
UCLASS()
class LYRACLONECODING_API ULyraPawnComponent_CharacterParts : public UPawnComponent
{
	GENERATED_BODY()
public:
	ULyraPawnComponent_CharacterParts(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	USkeletalMeshComponent* GetParentMeshComponent() const;
	USceneComponent* GetSceneComponentToAttachTo() const;

	FGameplayTagContainer GetCombinedTags(FGameplayTag RequiredPrefix) const;
	void BroadcastChanged();
	
	FLyraCharacterPartHandle AddCharacterPart(const FLyraCharacterPart& NewPart);
	void RemoveCharacterPart(FLyraCharacterPartHandle Handle);
	
	// 인스턴스화 된 Character Parts
	UPROPERTY()
	FLyraCharacterPartList CharacterPartList;

	// 애니메이션 적용을 위한 메시와 연결고리
	UPROPERTY(EditAnywhere, Category = Cosmetics)
	FLyraAnimBodyStyleSelectionSet BodyMeshes;
};
