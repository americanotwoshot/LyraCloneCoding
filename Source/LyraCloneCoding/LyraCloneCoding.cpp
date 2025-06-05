// Copyright Epic Games, Inc. All Rights Reserved.

#include "LyraCloneCoding.h"
#include "LyraLogChannels.h"
#include "Modules/ModuleManager.h"

class FLyraGameModule : public FDefaultGameModuleImpl
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

void FLyraGameModule::StartupModule()
{
	FDefaultGameModuleImpl::StartupModule();
	UE_LOG(LogLyra, Warning, TEXT("StartupModule!"));
}

void FLyraGameModule::ShutdownModule()
{
	FDefaultGameModuleImpl::ShutdownModule();
}

IMPLEMENT_PRIMARY_GAME_MODULE( FLyraGameModule, LyraCloneCoding, "LyraCloneCoding" );
