// Fill out your copyright notice in the Description page of Project Settings.


#include "LyraCameraMode.h"

#include "LyraCameraComponent.h"
#include "LyraPlayerCameraManager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraCameraMode)

FLyraCameraModeView::FLyraCameraModeView()
	: Location(ForceInit)
	, Rotation(ForceInit)
	, ControlRotation(ForceInit)
	, FieldOfView(LYRA_CAMERA_DEFAULT_FOV)
{
}

void FLyraCameraModeView::Blend(const FLyraCameraModeView& Other, float OtherWeight)
{
	if (OtherWeight <= 0.0f)
	{
		return;
	}
	else if (OtherWeight >= 1.0f)
	{
		// Weight가 1.0이면 Other을 덮어쓰면 된다.
		*this = Other;
		return;
	}

	// Location + OtherWeight * (Other.Location - Location)
	Location = FMath::Lerp(Location, Other.Location, OtherWeight);

	const FRotator DeltaRotation = (Other.Rotation - Rotation).GetNormalized();
	Rotation = Rotation + (OtherWeight * DeltaRotation);

	const FRotator DeltaControlRotation = (Other.ControlRotation - ControlRotation).GetNormalized();
	ControlRotation = ControlRotation + (OtherWeight * DeltaControlRotation);

	FieldOfView = FMath::Lerp(FieldOfView, Other.FieldOfView, OtherWeight);
}

ULyraCameraMode::ULyraCameraMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	FieldOfView = LYRA_CAMERA_DEFAULT_FOV;
	ViewPitchMin = LYRA_CAMERA_DEFAULT_PITCH_MIN;
	ViewPitchMax = LYRA_CAMERA_DEFAULT_PITCH_MAX;

	BlendTime = 0.0f;
	BlendAlpha = 1.0f;
	BlendWeight = 1.0f;

	BlendFunction = ELyraCameraModeBlendFunction::EaseOut;
	BlendExponent = 4.0f;
}

void ULyraCameraMode::UpdateCameraMode(float DeltaTime)
{
	// Actor를 활용하여, Pivot[Location|Rotation]을 계산하여 View를 업데이트
	UpdateView(DeltaTime);

	// BlendWeight를 DeltaTime을 활용하여, BlendAlpha 계산 후, BlendFunction에 맞게 다시 매핑하여 최종 계산
	UpdateBlending(DeltaTime);
}

void ULyraCameraMode::UpdateView(float DeltaTime)
{
	// CameraMode를 가지고 있는 CameraComponent의 Owner인 Character를 활용하여, PivotLocation/Rotation을 반환
	FVector PivotLocation = GetPivotLocation();
	FRotator PivotRotation = GetPivotRotation();

	PivotRotation.Pitch = FMath::ClampAngle(PivotRotation.Pitch, ViewPitchMin, ViewPitchMax);

	View.Location = PivotLocation;
	View.Rotation = PivotRotation;

	View.ControlRotation = View.Rotation;
	View.FieldOfView = FieldOfView;
}

void ULyraCameraMode::UpdateBlending(float DeltaTime)
{
	if (BlendTime > 0.f)
	{
		BlendAlpha += (DeltaTime / BlendTime);
	}
	else
	{
		BlendAlpha = 1.0f;
	}

	const float Exponent = (BlendExponent > 0.f) ? BlendExponent : 1.0f;
	switch (BlendFunction)
	{
	case ELyraCameraModeBlendFunction::Linear:
		BlendWeight = BlendAlpha;
		break;
	case ELyraCameraModeBlendFunction::EaseIn:
		BlendWeight = FMath::InterpEaseIn(0.0f, 1.0f, BlendAlpha, Exponent);
		break;
	case ELyraCameraModeBlendFunction::EaseOut:
		BlendWeight = FMath::InterpEaseOut(0.0f, 1.0f, BlendAlpha, Exponent);
		break;
	case ELyraCameraModeBlendFunction::EaseInOut:
		BlendWeight = FMath::InterpEaseInOut(0.0f, 1.0f, BlendAlpha, Exponent);
		break;
	default:
		checkf(false, TEXT("UpdateBlending: Invalid BlendFunction [%d]\n"), (uint8)BlendFunction);
		break;
	}
}

ULyraCameraComponent* ULyraCameraMode::GetLyraCameraComponent() const
{
	return CastChecked<ULyraCameraComponent>(GetOuter());
}

AActor* ULyraCameraMode::GetTargetActor() const
{
	const ULyraCameraComponent* LyraCameraComponent = GetLyraCameraComponent();
	return LyraCameraComponent->GetTargetActor();
}

FVector ULyraCameraMode::GetPivotLocation() const
{
	const AActor* TargetActor = GetTargetActor();
	check(TargetActor);

	if (const APawn* TargetPawn = Cast<APawn>(TargetActor))
	{
		// BaseEyeHeight를 고려하여, ViewLocation을 반환
		return TargetPawn->GetPawnViewLocation();
	}

	return TargetActor->GetActorLocation();
}

FRotator ULyraCameraMode::GetPivotRotation() const
{
	const AActor* TargetActor = GetTargetActor();
	check(TargetActor);

	if (const APawn* TargetPawn = Cast<APawn>(TargetActor))
	{
		// Pawn의 Controller의 Rotation을 반환
		return TargetPawn->GetViewRotation();
	}

	return TargetActor->GetActorRotation();
}

ULyraCameraModeStack::ULyraCameraModeStack(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

ULyraCameraMode* ULyraCameraModeStack::GetCameraModeInstance(TSubclassOf<ULyraCameraMode>& CameraModeClass)
{
	check(CameraModeClass);

	// CameraModeInstances에서 먼저 생성되었는지 확인
	for (ULyraCameraMode* CameraMode : CameraModeInstances)
	{
		if ((CameraMode != nullptr) && (CameraMode->GetClass() == CameraModeClass))
		{
			return CameraMode;
		}
	}

	// CameraModeClass에 알맞는 CameraMode 인스턴스가 없으면 생성
	ULyraCameraMode* NewCameraMode = NewObject<ULyraCameraMode>(GetOuter(), CameraModeClass, NAME_None, RF_NoFlags);
	check(NewCameraMode);

	CameraModeInstances.Add(NewCameraMode);

	return NewCameraMode;
}

void ULyraCameraModeStack::PushCameraMode(TSubclassOf<ULyraCameraMode>& CameraModeClass)
{
	if (!CameraModeClass)
	{
		return;
	}

	ULyraCameraMode* CameraMode = GetCameraModeInstance(CameraModeClass);
	check(CameraMode);

	int32 StackSize = CameraModeStack.Num();
	if ((StackSize > 0) && (CameraModeStack[0] == CameraMode))
	{
		// 가장 최근에 이미 CameraMode가 Stacking 되었으면 return
		return;
	}

	// ExistingStackIndex는 CameraModeStack에서 CameraMode에 맞는 Index를 찾음
	// ExistingStackContribution은 위에서 아래로 최종 BlendWeight 값을 찾음
	int32 ExistingStackIndex = INDEX_NONE;
	float ExistingStackContribution = 1.0f;

	for (int32 StackIndex = 0; StackIndex < StackSize; ++StackIndex)
	{
		if (CameraModeStack[StackIndex] == CameraMode)
		{
			ExistingStackIndex = StackIndex;
			ExistingStackContribution *= CameraMode->BlendWeight;
			break;
		}
		else
		{
			ExistingStackContribution *= (1.0f - CameraMode->BlendWeight);
		}
	}

	if (ExistingStackIndex != INDEX_NONE)
	{
		CameraModeStack.RemoveAt(ExistingStackIndex);
		StackSize--;
	}
	else
	{
		ExistingStackContribution = 0.0f;
	}

	const bool bShouldBlend = ((CameraMode->BlendTime > 0.f) && (StackSize > 0));
	const float BlendWeight = (bShouldBlend ? ExistingStackContribution : 1.0f);
	CameraMode->BlendWeight = BlendWeight;

	CameraModeStack.Insert(CameraMode, 0);

	CameraModeStack.Last()->BlendWeight = 1.0f;
}

void ULyraCameraModeStack::EvaluateStack(float DeltaTime, FLyraCameraModeView& OutCameraModeView)
{
	// Top -> Bottom까지 순차적으로 Stack에 있는 CameraMode 업데이트
	UpdateStack(DeltaTime);

	// Bottom -> Top까지 CameraModeStack에 대해 Blending 진행
	BlendStack(OutCameraModeView);
}

void ULyraCameraModeStack::UpdateStack(float DeltaTime)
{
	const int32 StackSize = CameraModeStack.Num();
	if (StackSize <= 0)
	{
		return;
	}

	int32 RemoveCount = 0;
	int32 RemoveIndex = INDEX_NONE;
	for (int32 StackIndex = 0; StackIndex < StackSize; ++StackIndex)
	{
		ULyraCameraMode* CameraMode = CameraModeStack[StackIndex];
		check(CameraMode);

		CameraMode->UpdateCameraMode(DeltaTime);

		if (CameraMode->BlendWeight >= 1.0f)
		{
			RemoveIndex = (StackIndex + 1);
			RemoveCount = (StackSize - RemoveIndex);
			break;
		}
	}

	if (RemoveCount > 0)
	{
		CameraModeStack.RemoveAt(RemoveIndex, RemoveCount);
	}
}

void ULyraCameraModeStack::BlendStack(FLyraCameraModeView& OutCameraModeView) const
{
	const int32 StackSize = CameraModeStack.Num();
	if (StackSize <= 0)
	{
		return;
	}

	// Bottom -> Top 순서로 Blend 진행
	const ULyraCameraMode* CameraMode = CameraModeStack[StackSize - 1];
	check(CameraMode);

	// 가장 아래는 BlendWeight가 1.0이다
	OutCameraModeView = CameraMode->View;

	// 이미 가장 아래를 지정했기 때문에 StackSize - 2 부터 순회
	for (int32 StackIndex = (StackSize - 2); StackIndex >= 0; --StackIndex)
	{
		CameraMode = CameraModeStack[StackIndex];
		check(CameraMode);

		OutCameraModeView.Blend(CameraMode->View, CameraMode->BlendWeight);
	}
	
}
