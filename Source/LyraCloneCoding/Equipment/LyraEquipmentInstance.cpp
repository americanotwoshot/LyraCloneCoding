// Fill out your copyright notice in the Description page of Project Settings.


#include "LyraEquipmentInstance.h"

#include "LyraEquipmentDefinition.h"
#include "GameFramework/Character.h"

ULyraEquipmentInstance::ULyraEquipmentInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

APawn* ULyraEquipmentInstance::GetPawn() const
{
	return Cast<APawn>(GetOuter());
}

void ULyraEquipmentInstance::SpawnEquipmentActors(const TArray<FLyraEquipmentActorToSpawn>& ActorsToSpawn)
{
	if (APawn* OwningPawn = GetPawn())
	{
		USceneComponent* AttachTarget = OwningPawn->GetRootComponent();
		if (ACharacter* Char = Cast<ACharacter>(OwningPawn))
		{
			// 만약 캐릭터라면 GetMesh로 반환하여, 메시에 붙인다.
			AttachTarget = Char->GetMesh();
		}

		for (const FLyraEquipmentActorToSpawn& SpawnInfo : ActorsToSpawn)
		{
			// SpawnActorDeferred는 FinishSpawning을 호출해야 스폰이 되고, 데이터만 세팅해 놓는 함수이다.
			AActor* NewActor = GetWorld()->SpawnActorDeferred<AActor>(SpawnInfo.ActorToSpawn, FTransform::Identity, OwningPawn);
			NewActor->FinishSpawning(FTransform::Identity, true);

			NewActor->SetActorRelativeTransform(SpawnInfo.AttachTransform);

			NewActor->AttachToComponent(AttachTarget, FAttachmentTransformRules::KeepRelativeTransform, SpawnInfo.AttachSocket);

			SpawnedActors.Add(NewActor);
		}
	}
}

void ULyraEquipmentInstance::DestroyEquipmentActors()
{
	for (AActor* Actor : SpawnedActors)
	{
		if (Actor)
		{
			Actor->Destroy();
		}
	}
}

void ULyraEquipmentInstance::OnEquipped()
{
	K2_OnEquipped();
}

void ULyraEquipmentInstance::OnUnequipped()
{
	K2_OnUnequipped();
}
