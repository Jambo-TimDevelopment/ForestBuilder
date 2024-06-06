// Copyright Epic Games, Inc. All Rights Reserved.

#include "Editor/ForestCreatorPluginStyle.h"
#include "Editor/ForestCreatorPlugin.h"
#include "Framework/Application/SlateApplication.h"
#include "Styling/SlateStyleRegistry.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FForestCreatorPluginStyle::StyleInstance = nullptr;

void FForestCreatorPluginStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FForestCreatorPluginStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FForestCreatorPluginStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("ForestCreatorPluginStyle"));
	return StyleSetName;
}


const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);

TSharedRef< FSlateStyleSet > FForestCreatorPluginStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("ForestCreatorPluginStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("ForestCreatorPlugin")->GetBaseDir() / TEXT("Resources"));

	Style->Set("ForestCreatorPlugin.PluginAction", new IMAGE_BRUSH_SVG(TEXT("PlaceholderButtonIcon"), Icon20x20));
	return Style;
}

void FForestCreatorPluginStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FForestCreatorPluginStyle::Get()
{
	return *StyleInstance;
}
