// Fill out your copyright notice in the Description page of Project Settings.


#include "LyraCosmeticAnimationTypes.h"

USkeletalMesh* FLyraAnimBodyStyleSelectionSet::SelectBestBodyStyle(const FGameplayTagContainer& CosmeticTags) const
{
	for (const FLyraAnimBodyStyleSelectionEntry& Rule : MeshRules)
	{
		if ((Rule.Mesh) && CosmeticTags.HasAll(Rule.RequiredTags))
		{
			return Rule.Mesh;
		}
	}
	
	return DefaultMesh;
}
