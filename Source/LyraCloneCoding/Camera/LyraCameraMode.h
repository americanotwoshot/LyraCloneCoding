// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LyraCameraMode.generated.h"

class ULyraCameraComponent;
/**
 * FLyraCameraModeView : 화면에 보여질 영역
 */
struct FLyraCameraModeView
{
	FLyraCameraModeView();

	void Blend(const FLyraCameraModeView& Other, float OtherWeight);
	
	FVector Location;
	FRotator Rotation;
	FRotator ControlRotation;
	float FieldOfView;
};

/**
 * [0,1]을 BlendFunction에 맞게 재매핑을 위한 타입
 */
UENUM(BlueprintType)
enum class ELyraCameraModeBlendFunction : uint8
{
	Linear,
	// EaseIn/Out은 exponent 값에 의해 조절된다.
	EaseIn,
	EaseOut,
	EaseInOut,
	COUNT
};

/** Camera Blending 대상 유닛 */
UCLASS(Abstract, NotBlueprintable)
class ULyraCameraMode : public UObject
{
	GENERATED_BODY()
public:
	ULyraCameraMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/**
	 *	member methods
	 */
	void UpdateCameraMode(float DeltaTime);
	virtual void UpdateView(float DeltaTime);
	void UpdateBlending(float DeltaTime);

	ULyraCameraComponent* GetLyraCameraComponent() const;
	AActor* GetTargetActor() const;
	FVector GetPivotLocation() const;
	FRotator GetPivotRotation() const;
	
	/**
	 *	member variables
	 */
	// CameraMode에 의해 생성된 CameraModeView
	FLyraCameraModeView View;

	// CameraMode의 FOV
	UPROPERTY(EditDefaultsOnly, Category = "View", Meta = (UIMin = "5.0", UIMax = "170", ClampMin = "5.0", ClampMax = "170.0"))
	float FieldOfView;

	// View에 대한 Pitch [Min, Max]
	UPROPERTY(EditDefaultsOnly, Category = "View", Meta = (UIMin = "-89.9", UIMax = "89.9", ClampMin = "-89.9", ClampMax = "89.9"))
	float ViewPitchMin;

	UPROPERTY(EditDefaultsOnly, Category = "View", Meta = (UIMin = "-89.9", UIMax = "89.9", ClampMin = "-89.9", ClampMax = "89.9"))
	float ViewPitchMax;
	
	// 얼마동안 Blend를 진행할지, 시간
	UPROPERTY(EditDefaultsOnly, Category = "Blending")
	float BlendTime;

	// 선형적인 Blend 값 [0, 1]
	float BlendAlpha;

	// 해당 CameraMode의 최종 Blend 값
	float BlendWeight;

	// EaseIn/Out에 사용한 Exponent
	UPROPERTY(EditDefaultsOnly, Category = "Blending")
	float BlendExponent;

	// BlendFunction
	ELyraCameraModeBlendFunction BlendFunction;
};

/** Camera Blending을 담당하는 객체 */
UCLASS()
class ULyraCameraModeStack : public UObject
{
	GENERATED_BODY()
public:
	ULyraCameraModeStack(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/**
	 *	member methods
	 */
	ULyraCameraMode* GetCameraModeInstance(TSubclassOf<ULyraCameraMode>& CameraModeClass);
	void PushCameraMode(TSubclassOf<ULyraCameraMode>& CameraModeClass);
	void EvaluateStack(float DeltaTime, FLyraCameraModeView& OutCameraModeView);
	void UpdateStack(float DeltaTime);
	void BlendStack(FLyraCameraModeView& OutCameraModeView) const;
	
	/**
	 *	member variables
	 */
	
	// 생성된 CameraMode를 관리
	UPROPERTY()
	TArray<TObjectPtr<ULyraCameraMode>> CameraModeInstances;

	// Camera Marix Blend 업데이트 진행 큐
	UPROPERTY()
	TArray<TObjectPtr<ULyraCameraMode>> CameraModeStack;
};
