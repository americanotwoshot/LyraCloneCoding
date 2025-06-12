// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LyraCameraMode.h"
#include "Camera/CameraComponent.h"
#include "LyraCameraComponent.generated.h"

class ULyraCameraModeStack;

template <class TClass> class TSubclassOf;

// (return type, delegate name)
DECLARE_DELEGATE_RetVal(TSubclassOf<ULyraCameraMode>, FLyraCameraModeDelegate);

UCLASS()
class LYRACLONECODING_API ULyraCameraComponent : public UCameraComponent
{
	GENERATED_BODY()
public:
	ULyraCameraComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	static ULyraCameraComponent* FindCameraComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<ULyraCameraComponent>() : nullptr); }
	
	/**
	 * member methods
	 */
	AActor* GetTargetActor() const { return GetOwner(); }
	void UpdateCameraModes();
	
	/**
	 * CameraComponent Interface 
	 */
	virtual void OnRegister() final;
	virtual void GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView) final;
	
	/**
	 * member variables
	 */
	// 카메라의 blending 기능을 지원하는 stack
	UPROPERTY()
	TObjectPtr<ULyraCameraModeStack> CameraModeStack;

	// 현재 CameraMode를 가져오는 Delegate
	FLyraCameraModeDelegate DetermineCameraModeDelegate;
};
