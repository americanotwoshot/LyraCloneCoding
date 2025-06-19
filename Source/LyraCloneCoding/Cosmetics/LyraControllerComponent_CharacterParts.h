// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LyraCharacterPartTypes.h"
#include "Components/ControllerComponent.h"
#include "LyraControllerComponent_CharacterParts.generated.h"

class ULyraPawnComponent_CharacterParts;
/**
 * ControllerComponent가 소유하는 Character Parts
 */
USTRUCT()
struct FLyraControllerCharacterPartEntry
{
	GENERATED_BODY()

	// Character Part에 대한 정의 (메타데이터)
	UPROPERTY(EditAnywhere)
	FLyraCharacterPart Part;

	// Character Part 핸들 (고유값)
	FLyraCharacterPartHandle Handle;
};

/**
 * 
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class ULyraControllerComponent_CharacterParts : public UControllerComponent
{
	GENERATED_BODY()
public:
	ULyraControllerComponent_CharacterParts(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	ULyraPawnComponent_CharacterParts* GetPawnCustomizer() const;
	
	UFUNCTION(BlueprintCallable, Category = Cosmetics)
	void AddCharacterPart(const FLyraCharacterPart& NewPart);
	void AddCharacterPartInternal(const FLyraCharacterPart& NewPart);
	void RemoveAllCharacterParts();
	
	UFUNCTION()
	void OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn);
	
	UPROPERTY(EditAnywhere, Category = Cosmetics)
	TArray<FLyraControllerCharacterPartEntry> CharacterParts;
};
