// Fill out your copyright notice in the Description page of Project Settings.


#include "LyraHeroComponent.h"
#include "LyraPawnData.h"
#include "LyraPawnExtensionComponent.h"
#include "Components/GameFrameworkComponentManager.h"
#include "LyraCloneCoding/LyraGameplayTags.h"
#include "LyraCloneCoding/LyraLogChannels.h"
#include "LyraCloneCoding/Camera/LyraCameraComponent.h"
#include "LyraCloneCoding/Player/LyraPlayerState.h"

// FeatureName 정의: static member variable 초기화
const FName ULyraHeroComponent::NAME_ActorFeatureName("Hero");

ULyraHeroComponent::ULyraHeroComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
}

void ULyraHeroComponent::OnRegister()
{
	Super::OnRegister();

	// 올바른 Actor에 등록되었는지 확인
	{
		if (!GetPawn<APawn>())
		{
			UE_LOG(LogLyra, Error, TEXT("this component has been added to a BP whose base class is not a Pawn!"));
			return;
		}
	}

	// GameFrameworkComponentManager에 InitState 사용을 위해 등록 진행
	RegisterInitStateFeature();
}

void ULyraHeroComponent::BeginPlay()
{
	Super::BeginPlay();

	// PawnExtensionComponent에 대해서 OnActorInitStateChanged() 관찰하도록
	BindOnActorInitStateChanged(ULyraPawnExtensionComponent::NAME_ActorFeatureName, FGameplayTag(), false);

	// InitState_Spawned로 초기화
	ensure(TryToChangeInitState(FLyraGameplayTags::Get().InitState_Spawned));

	// 강제 업데이트를 진행
	CheckDefaultInitialization();
}

void ULyraHeroComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// OnRegister의 RegisterInitStateFeature()의 쌍을 맞추자
	UnregisterInitStateFeature();
	
	Super::EndPlay(EndPlayReason);
}

void ULyraHeroComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	const FLyraGameplayTags& InitTags = FLyraGameplayTags::Get();

	if (Params.FeatureName == ULyraPawnExtensionComponent::NAME_ActorFeatureName)
	{
		// LyraPawnExtensionComponent의 DataInitialized 상태 변화 관찰 후, LyraHeroComponent도 DataInitialized 상태로 변경
		if (Params.FeatureState == InitTags.InitState_DataInitialized)
		{
			CheckDefaultInitialization();
		}
	}
}

bool ULyraHeroComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState,
	FGameplayTag DesiredState) const
{
	check(Manager);

	const FLyraGameplayTags& InitTags = FLyraGameplayTags::Get();
	APawn* Pawn = GetPawn<APawn>();
	ALyraPlayerState* LyraPS = GetPlayerState<ALyraPlayerState>();

	// InitState_Spawned 초기화
	if (!CurrentState.IsValid() && DesiredState == InitTags.InitState_Spawned)
	{
		// Pawn이 잘 세팅만 되어있으면 바로 넘어감
		if (Pawn)
		{
			return true;
		}
	}

	// Spawned -> DataAvailable
	if (CurrentState == InitTags.InitState_Spawned && DesiredState == InitTags.InitState_DataAvailable)
	{
		if (!LyraPS)
		{
			return false;
		}

		return true;
	}

	// DataAvailable -> DataInitialized
	if (CurrentState == InitTags.InitState_DataAvailable && DesiredState == InitTags.InitState_DataInitialized)
	{
		// PawnExtensionComponent가 DataInitialized될 때까지 기다림 (== 모든 feature component가 InitState_DataAvailable인 상태)
		return LyraPS && Manager->HasFeatureReachedInitState(Pawn, ULyraPawnExtensionComponent::NAME_ActorFeatureName, InitTags.InitState_DataInitialized);
	}

	// DataInitialized -> GameplayReady
	if (CurrentState == InitTags.InitState_DataInitialized && DesiredState == InitTags.InitState_GameplayReady)
	{
		return true;
	}

	// 위의 선형적인 transition이 아니면 false
	return false;	
	
}

void ULyraHeroComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState,
	FGameplayTag DesiredState)
{
	const FLyraGameplayTags& InitTags = FLyraGameplayTags::Get();

	// DataAvailable -> DataInitialized
	if (CurrentState == InitTags.InitState_DataAvailable && DesiredState == InitTags.InitState_DataInitialized)
	{
		APawn* Pawn = GetPawn<APawn>();
		ALyraPlayerState* LyraPS = GetPlayerState<ALyraPlayerState>();
		if (!ensure(Pawn && LyraPS))
		{
			return;
		}

		// Input과 Camera에 대한 핸들링 (TODO)

		const bool bIsLocallyControlled = Pawn->IsLocallyControlled();
		const ULyraPawnData* PawnData = nullptr;
		if (ULyraPawnExtensionComponent* PawnExtComp = ULyraPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
		{
			PawnData = PawnExtComp->GetPawnData<ULyraPawnData>();
		}

		if (bIsLocallyControlled && PawnData)
		{
			// 현재 LyraCharacter에 Attach된 CameraComponent를 찾음
			if (ULyraCameraComponent* CameraComponent = ULyraCameraComponent::FindCameraComponent(Pawn))
			{
				CameraComponent->DetermineCameraModeDelegate.BindUObject(this, &ThisClass::DetermineCameraMode);
			}
		}
	}
}

void ULyraHeroComponent::CheckDefaultInitialization()
{
	const FLyraGameplayTags& InitTags = FLyraGameplayTags::Get();

	static const TArray<FGameplayTag> StateChain = { InitTags.InitState_Spawned, InitTags.InitState_DataAvailable, InitTags.InitState_DataInitialized, InitTags.InitState_GameplayReady };

	// 가능한 InitState 변화를 진행할 수 있는 것까지 InitState Change를 진행한다.
	ContinueInitStateChain(StateChain);
}

PRAGMA_DISABLE_OPTIMIZATION
TSubclassOf<ULyraCameraMode> ULyraHeroComponent::DetermineCameraMode() const
{
	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return nullptr;
	}

	if (ULyraPawnExtensionComponent* PawnExtComp = ULyraPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		if (const ULyraPawnData* PawnData = PawnExtComp->GetPawnData<ULyraPawnData>())
		{
			return PawnData->DefaultCameraMode;
		}
	}

	return nullptr;
}
PRAGMA_ENABLE_OPTIMIZATION