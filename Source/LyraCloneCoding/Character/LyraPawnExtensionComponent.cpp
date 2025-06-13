// Fill out your copyright notice in the Description page of Project Settings.


#include "LyraPawnExtensionComponent.h"

#include "Components/GameFrameworkComponentManager.h"
#include "LyraCloneCoding/LyraGameplayTags.h"
#include "LyraCloneCoding/LyraLogChannels.h"

// feature name은 component마다 있기 때문에 component라는 이름은 빼고 이름 선언
const FName ULyraPawnExtensionComponent::NAME_ActorFeatureName("PawnExtension");

ULyraPawnExtensionComponent::ULyraPawnExtensionComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
}

void ULyraPawnExtensionComponent::SetPawnData(const ULyraPawnData* InPawnData)
{
	// Pawn에 대해 Authority가 없을 경우, SetPawnData는 진행하지 않음
	APawn* Pawn = GetPawnChecked<APawn>();
	if (Pawn->GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	if (PawnData)
	{
		return;
	}

	PawnData = InPawnData;
	
}

void ULyraPawnExtensionComponent::SetupPlayerInputComponent()
{
	// ForceUpdate로 다시 InitState 상태 변환 시작 
	CheckDefaultInitialization();
}

void ULyraPawnExtensionComponent::OnRegister()
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

	// 디버깅을 위한 함수
	UGameFrameworkComponentManager* Manager = UGameFrameworkComponentManager::GetForActor(GetOwningActor());
}

void ULyraPawnExtensionComponent::BeginPlay()
{
	Super::BeginPlay();

	// Actor에 등록된 모든 Feature Component의 InitState 상태를 관찰하겠다는 의미
	BindOnActorInitStateChanged(NAME_None, FGameplayTag(), false);

	// InitState_Spawned로 상태 변환을 시도
	// 1. CanChangeInitState로 상태 변화 가능성 유무를 판단
	// 2. HandleChangeInitState로 내부 상태를 변경 (Feature Component)
	// 3. BindOnActorInitStateChanged로 bind된 Delegate를 조건에 맞게 호출해줌
	//		-> 모든 Actor의 Feature 상태 변화에 대해 OnActorInitStateChanged()가 호출됨
	ensure(TryToChangeInitState(FLyraGameplayTags::Get().InitState_Spawned));

	// 강제 업데이트를 진행
	CheckDefaultInitialization();
}

void ULyraPawnExtensionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// OnRegister의 RegisterInitStateFeature()의 쌍을 맞추자
	UnregisterInitStateFeature();
	
	Super::EndPlay(EndPlayReason);
}

void ULyraPawnExtensionComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	// 나 자신의 InitState 변화가 아니라 다른 component의 변화만
	if (Params.FeatureName != NAME_ActorFeatureName)
	{
		// 다른 Feature Component들의 상태를 계속 관찰하여 Sync를 맞추기 위해
		// OnActorInitStateChanged에서는 지속적으로 CheckDefaultInitialization()을 호출하여 상태를 확인한다.
		const FLyraGameplayTags& InitTags = FLyraGameplayTags::Get();
		if (Params.FeatureState == InitTags.InitState_DataAvailable)
		{
			CheckDefaultInitialization();
		}
	}
}

bool ULyraPawnExtensionComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager,
	FGameplayTag CurrentState, FGameplayTag DesiredState) const
{
	check(Manager);

	APawn* Pawn = GetPawn<APawn>();
	const FLyraGameplayTags& InitTags = FLyraGameplayTags::Get();

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
		// PawnData를 설정하기 전이라면
		if (!PawnData)
		{
			return false;
		}

		// Pawn이 Controller가 없으면 false 반환
		const bool bIsLocallyControlled = Pawn->IsLocallyControlled();
		if (bIsLocallyControlled)
		{
			if (!GetController<AController>())
			{
				return false;
			}
		}
		
		return true;
	}

	// DataAvailable -> DataInitialized
	if (CurrentState == InitTags.InitState_DataAvailable && DesiredState == InitTags.InitState_DataInitialized)
	{
		// Actor에 바인드된 모든 Feature들이 DataAvailable 상태일 때, true 반환
		return Manager->HaveAllFeaturesReachedInitState(Pawn, InitTags.InitState_DataAvailable);
	}

	// DataInitialized -> GameplayReady
	if (CurrentState == InitTags.InitState_DataInitialized && DesiredState == InitTags.InitState_GameplayReady)
	{
		return true;
	}

	// 위의 선형적인 transition이 아니면 false
	return false;	
}

void ULyraPawnExtensionComponent::CheckDefaultInitialization()
{
	// PawnExtensionComponent는 Feature Component들의 초기화를 관장하는 Component이다.
	// 따라서, Actor에 바인딩된 Feature Component들에 대해 CheckDefaultInitialization을 호출해준다.(나를 제외하고)
	CheckDefaultInitializationForImplementers();

	const FLyraGameplayTags& InitTags = FLyraGameplayTags::Get();

	static const TArray<FGameplayTag> StateChain = { InitTags.InitState_Spawned, InitTags.InitState_DataAvailable, InitTags.InitState_DataInitialized, InitTags.InitState_GameplayReady };

	// 가능한 InitState 변화를 진행할 수 있는 것까지 InitState Change를 진행한다.
	ContinueInitStateChain(StateChain);
}