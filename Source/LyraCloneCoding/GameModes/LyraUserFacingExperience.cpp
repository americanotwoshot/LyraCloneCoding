// Fill out your copyright notice in the Description page of Project Settings.


#include "LyraUserFacingExperience.h"

#include "CommonSessionSubsystem.h"

UCommonSession_HostSessionRequest* ULyraUserFacingExperience::CreateHostingRequest() const
{
	const FString ExperienceName = ExperienceID.PrimaryAssetName.ToString();

	UCommonSession_HostSessionRequest* Result = NewObject<UCommonSession_HostSessionRequest>();
	Result->MapID = MapID;
	Result->ExtraArgs.Add(TEXT("Experience"), ExperienceName);
	
	return Result;
}
