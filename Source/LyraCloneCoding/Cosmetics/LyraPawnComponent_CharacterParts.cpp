// Fill out your copyright notice in the Description page of Project Settings.


#include "LyraPawnComponent_CharacterParts.h"

#include "GameplayTagAssetInterface.h"
#include "GameFramework/Character.h"

bool FLyraCharacterPartList::SpawnActorForEntry(FLyraAppliedCharacterPartEntry& Entry)
{
	bool bCreatedAnyActor = false;
	// Metadata가 비어있다면 그냥 반환
	if (Entry.Part.PartClass != nullptr)
	{
		UWorld* World = OwnerComponent->GetWorld();

		// 해당 ULyraPawnComponent_CharacterParts의 OwnerComponent를 가지는 Actor의 Root Component에 붙이기 위해서
		if (USceneComponent* ComponentToAttachTo = OwnerComponent->GetSceneComponentToAttachTo())
		{
			const FTransform SpawnTransform = ComponentToAttachTo->GetSocketTransform(Entry.Part.SocketName);

			// Actor-Actor의 결합에는 UChildActorComponent를 사용한다.
			UChildActorComponent* PartComponent = NewObject<UChildActorComponent>(OwnerComponent->GetOwner());
			PartComponent->SetupAttachment(ComponentToAttachTo, Entry.Part.SocketName);
			PartComponent->SetChildActorClass(Entry.Part.PartClass);
			// RenderWorld인 FScene에 변경 내용을 전달
			PartComponent->RegisterComponent();

			if (AActor* SpawnedActor = PartComponent->GetChildActor())
			{
				if (USceneComponent* SpawnedRootComponent = SpawnedActor->GetRootComponent())
				{
					SpawnedRootComponent->AddTickPrerequisiteComponent(ComponentToAttachTo);
				}
			}
			
			Entry.SpawnedComponent = PartComponent;
			bCreatedAnyActor = true;
		}
	}

	return bCreatedAnyActor;
}

void FLyraCharacterPartList::DestroyActorForEntry(FLyraAppliedCharacterPartEntry& Entry)
{
	if (Entry.SpawnedComponent)
	{
		Entry.SpawnedComponent->DestroyComponent();
		Entry.SpawnedComponent = nullptr;
	}
}

FLyraCharacterPartHandle FLyraCharacterPartList::AddEntry(FLyraCharacterPart NewPart)
{
	FLyraCharacterPartHandle Result;
	Result.PartHandle = PartHandleCounter++;

	if (ensure(OwnerComponent && OwnerComponent->GetOwner() && OwnerComponent->GetOwner()->HasAuthority()))
	{
		FLyraAppliedCharacterPartEntry& NewEntry = Entries.AddDefaulted_GetRef();
		NewEntry.Part = NewPart;
		NewEntry.PartHandle = Result.PartHandle;
		
		if (SpawnActorForEntry(NewEntry))
		{
			OwnerComponent->BroadcastChanged();
		}
	}

	return Result;
}

void FLyraCharacterPartList::RemoveEntry(FLyraCharacterPartHandle Handle)
{
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		FLyraAppliedCharacterPartEntry& Entry = *EntryIt;

		if (Entry.PartHandle == Handle.PartHandle)
		{
			DestroyActorForEntry(Entry);
		}
	}
}

FGameplayTagContainer FLyraCharacterPartList::CollectCombinedTags() const
{
	FGameplayTagContainer Result;

	for (const FLyraAppliedCharacterPartEntry& Entry : Entries)
	{
		if (Entry.SpawnedComponent)
		{
			// 해당 Actor의 IGameplayTagAssetInterface를 통해 GameplayTag를 가져온다.
			// 후에 각 Part에 대해 GameplayTag를 넣고 싶으면 상속받아서 정의하면 된다.
			if (IGameplayTagAssetInterface* TagInterface = Cast<IGameplayTagAssetInterface>(Entry.SpawnedComponent->GetChildActor()))
			{
				TagInterface->GetOwnedGameplayTags(Result);
			}
		}
	}
	
	return Result;
}

ULyraPawnComponent_CharacterParts::ULyraPawnComponent_CharacterParts(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, CharacterPartList(this)
{
}

USkeletalMeshComponent* ULyraPawnComponent_CharacterParts::GetParentMeshComponent() const
{
	if (AActor* OwnerActor = GetOwner())
	{
		if (ACharacter* OwningCharacter = Cast<ACharacter>(OwnerActor))
		{
			if (USkeletalMeshComponent* MeshComponent = OwningCharacter->GetMesh())
			{
				return MeshComponent;
			}
		}
	}
	return nullptr;
}

USceneComponent* ULyraPawnComponent_CharacterParts::GetSceneComponentToAttachTo() const
{
	if (USkeletalMeshComponent* MeshComponent = GetParentMeshComponent())
	{
		return MeshComponent;
	}
	else if (AActor* OwnerActor = GetOwner())
	{
		return OwnerActor->GetRootComponent();
	}
	return nullptr;
}

FGameplayTagContainer ULyraPawnComponent_CharacterParts::GetCombinedTags(FGameplayTag RequiredPrefix) const
{
	// 현재 장착된 CharacterPartList의 Merge된 Tags를 반환한다.
	FGameplayTagContainer Result = CharacterPartList.CollectCombinedTags();
	if (RequiredPrefix.IsValid())
	{
		// GameplayTag를 통해 필터링할 경우
		return Result.Filter(FGameplayTagContainer(RequiredPrefix));
	}
	else
	{
		// 필터링할 GameplayTag가 없을 경우
		return Result;
	}
	
}

void ULyraPawnComponent_CharacterParts::BroadcastChanged()
{
	const bool bReinitPose = true;

	// 현재 Owner의 SkeletalMeshComponent를 가져온다
	if (USkeletalMeshComponent* MeshComponent = GetParentMeshComponent())
	{
		// BodyMeshes를 통해, GameplayTag를 활용하여 알맞은 SkeletalMesh를 설정한다.
		const FGameplayTagContainer MergedTags = GetCombinedTags(FGameplayTag());
		USkeletalMesh* DesiredMesh = BodyMeshes.SelectBestBodyStyle(MergedTags);

		// SkeletalMesh를 초기화
		MeshComponent->SetSkeletalMesh(DesiredMesh, bReinitPose);

		// PhysicsAsset을 초기화
		if (UPhysicsAsset* PhysicsAsset = BodyMeshes.ForcedPhysicsAsset)
		{
			MeshComponent->SetPhysicsAsset(PhysicsAsset, bReinitPose);
		}
	}
}

FLyraCharacterPartHandle ULyraPawnComponent_CharacterParts::AddCharacterPart(const FLyraCharacterPart& NewPart)
{
	return CharacterPartList.AddEntry(NewPart);
}

void ULyraPawnComponent_CharacterParts::RemoveCharacterPart(FLyraCharacterPartHandle Handle)
{
	CharacterPartList.RemoveEntry(Handle);
}