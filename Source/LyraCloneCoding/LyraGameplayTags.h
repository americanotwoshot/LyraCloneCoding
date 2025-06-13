// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Containers/UnrealString.h"
#include "Containers/Map.h"
#include "GameplayTagContainer.h"

class UGameplayTagsManager;

// singleton containing native gameplay tags
struct FLyraGameplayTags
{
	static const FLyraGameplayTags& Get() {return GameplayTags; }
	static void InitializeNativeTags();

	void AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment);
	void AddAllTags(UGameplayTagsManager& Manager);

	FGameplayTag InitState_Spawned;
	FGameplayTag InitState_DataAvailable;
	FGameplayTag InitState_DataInitialized;
	FGameplayTag InitState_GameplayReady;

	/**
	 * Enhanced Input Tags
	 */
	FGameplayTag InputTag_Move;
	FGameplayTag InputTag_Look_Mouse;
	
private:
	static FLyraGameplayTags GameplayTags; 
};