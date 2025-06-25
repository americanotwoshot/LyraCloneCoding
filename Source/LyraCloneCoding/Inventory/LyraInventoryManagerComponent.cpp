// Fill out your copyright notice in the Description page of Project Settings.


#include "LyraInventoryManagerComponent.h"


ULyraInventoryManagerComponent::ULyraInventoryManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, InventoryList(this)
{
}
