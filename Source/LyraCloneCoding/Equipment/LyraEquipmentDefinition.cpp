// Fill out your copyright notice in the Description page of Project Settings.


#include "LyraEquipmentDefinition.h"

#include "LyraEquipmentInstance.h"

ULyraEquipmentDefinition::ULyraEquipmentDefinition(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	InstanceType = ULyraEquipmentInstance::StaticClass();
}
