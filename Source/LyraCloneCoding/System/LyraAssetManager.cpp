// Fill out your copyright notice in the Description page of Project Settings.

#include "LyraAssetManager.h"

#include "LyraCloneCoding/LyraGameplayTags.h"
#include "LyraCloneCoding/LyraLogChannels.h"

ULyraAssetManager::ULyraAssetManager()
{
}

ULyraAssetManager& ULyraAssetManager::Get()
{
	check(GEngine);

	// GEngine의 AssetManager 클래스를 오버라이드 했기 때문에, GEngine에 Asset Manager를 사용
	if (ULyraAssetManager* Singleton = Cast<ULyraAssetManager>(GEngine->AssetManager))
	{
		return *Singleton;
	}

	UE_LOG(LogLyra, Fatal, TEXT("Invalid AssetManagerClassname in DefaultEngine.ini(project settings); it must be LyraAssetManager"));

	// 위의 Fatal로 인해, Crash나기 때문에 도달하지는 않지만, 컴파일을 위해 더미로 리턴
	return *NewObject<ULyraAssetManager>();
}

void ULyraAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FLyraGameplayTags::InitializeNativeTags();
}

bool ULyraAssetManager::ShouldLogAssetLoads()
{
	// c++에서 args를 받아서 검사하는 함수
	const TCHAR* CommandLineContent = FCommandLine::Get();
	static bool bLogAssetLoads = FParse::Param(CommandLineContent, TEXT("LogAssetLoads"));
	return bLogAssetLoads;
}

UObject* ULyraAssetManager::SynchronousLoadAsset(const FSoftObjectPath& AssetPath)
{
	// 해당 함수를 따로 동작
	if (AssetPath.IsValid())
	{
		TUniquePtr<FScopeLogTime> LogTimePtr;
		if (ShouldLogAssetLoads())
		{
			// 단순히 로깅하면서, 초단위로 로깅 진행
			LogTimePtr = MakeUnique<FScopeLogTime>(*FString::Printf(TEXT("synchronous loaded assets [%s]"), *AssetPath.ToString()), nullptr, FScopeLogTime::ScopeLog_Seconds);
		}

		// 1. AssetManager가 있으면, AssetManager의 StreamableManager를 통해 정적 로딩
		if (UAssetManager::IsValid())
		{
			return UAssetManager::GetStreamableManager().LoadSynchronous(AssetPath);
		}

		// 2. 아니면, FSoftObjectPath를 통해 정적 로딩
		// 항상 StaticLoadObject하기 전에 StaticFindObject를 통해 확인하고
		// 실패하면 진짜 로딩함(매우 느림)
		return AssetPath.TryLoad();
	}

	return nullptr;
}

void ULyraAssetManager::AddLoadedAsset(const UObject* Asset)
{
	if (ensureAlways(Asset))
	{
		FScopeLock Lock(&SyncObject);
		LoadedAssets.Add(Asset);
	}
}
