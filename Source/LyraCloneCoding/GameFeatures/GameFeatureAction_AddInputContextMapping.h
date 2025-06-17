#pragma once

#include "CoreMinimal.h"
#include "GameFeatureAction_WorldActionBase.h"
#include "GameFeatureAction_AddInputContextMapping.generated.h"

class UInputMappingContext;
struct FComponentRequestHandle;
class APawn;

USTRUCT()
struct FInputMappingContextAndPriority
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Input")
	TSoftObjectPtr<UInputMappingContext> InputMapping;

	UPROPERTY(EditAnywhere, Category = "Input")
	int32 Priority = 0;

	UPROPERTY(EditAnywhere, Category = "Input")
	bool bRegisterWithSettings = true;
};

UCLASS()
class UGameFeatureAction_AddInputContextMapping : public UGameFeatureAction_WorldActionBase
{
	GENERATED_BODY()
public:
	/**
	 * UGameFeatureAction's Interface
	 */
	virtual void OnGameFeatureRegistering() override;
	virtual void OnGameFeatureActivating(FGameFeatureActivatingContext& Context) override;
	virtual void OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context) override;
	virtual void OnGameFeatureUnregistering() override;

private:
	struct FPerContextData
	{
		TArray<TSharedPtr<FComponentRequestHandle>> ExtensionRequestHandles;
		TArray<TWeakObjectPtr<APlayerController>> ControllersAddedTo;
	};

	/** Delegate for when the game instance is changed to register IMC's */
	FDelegateHandle RegisterInputContextMappingsForGameInstanceHandle;
	
	/**
	 * member functions
	 */
	void RegisterInputMappingContexts();
	void RegisterInputMappingsContextForGameInstance(UGameInstance* GameInstance);
	void RegisterInputMappingContextsForLocalPlayer(ULocalPlayer* LocalPlayer);
	void UnregisterInputMappingContexts();
	void UnregisterInputMappingContextsForGameInstance(UGameInstance* GameInstance);
	void UnregisterInputMappingContextsForLocalPlayer(ULocalPlayer* LocalPlayer);
	
	/**
	 * UGameFeatureAction_WorldActionBase's Interface
	 */
	virtual void AddToWorld(const FWorldContext& WorldContext, const FGameFeatureStateChangeContext& ChangeContext) override;
	
	// GameFeatureState가 변경될 때, 콜백 함수
	void HandlePawnExtension(AActor* Actor, FName EventName, FGameFeatureStateChangeContext ChangeContext);
	
	// InputConfig를 EnhancedInputLocalPlayerSubsystem에 추가 및 제거
	void AddInputMapping(UPlayer* Player, FPerContextData& ActiveData);
	void RemoveInputMapping(APlayerController* PlayerController, FPerContextData& ActiveData);
	
	// ActiveData를 초기화
	void Reset(FPerContextData& ActiveData);

	/**
	 * member variables
	 */
	TMap<FGameFeatureStateChangeContext, FPerContextData> ContextData;

	UPROPERTY(EditAnywhere, Category = "Input")
	TArray<FInputMappingContextAndPriority> InputMappings;
};