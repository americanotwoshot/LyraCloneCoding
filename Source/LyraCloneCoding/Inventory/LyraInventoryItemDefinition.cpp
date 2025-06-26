// Fill out your copyright notice in the Description page of Project Settings.


#include "LyraInventoryItemDefinition.h"

ULyraInventoryItemDefinition::ULyraInventoryItemDefinition(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

const ULyraInventoryItemFragment* ULyraInventoryItemDefinition::FindFragmentByClass(
	TSubclassOf<ULyraInventoryItemFragment> FragmentClass) const
{
	if (FragmentClass)
	{
		for (ULyraInventoryItemFragment* Fragment : Fragments)
		{
			if (Fragment && Fragment->IsA(FragmentClass))
			{
				return Fragment;
			}
		}
	}
	return nullptr;
}
