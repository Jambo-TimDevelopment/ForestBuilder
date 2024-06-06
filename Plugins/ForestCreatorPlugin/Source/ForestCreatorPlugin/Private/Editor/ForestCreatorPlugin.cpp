// Copyright Epic Games, Inc. All Rights Reserved.

#include "Editor/ForestCreatorPlugin.h"
#include "Editor/ForestCreatorPluginStyle.h"
#include "Editor/ForestCreatorPluginCommands.h"
#include "Landscape.h"
#include "Misc/MessageDialog.h"
#include "ToolMenus.h"
#include "Core/FCFunctionLibrary.h"
#include "Core/ForestCreator.h"
#include "Kismet/GameplayStatics.h"


static const FName ForestCreatorPluginTabName("ForestCreatorPlugin");

#define LOCTEXT_NAMESPACE "FForestCreatorPluginModule"

void FForestCreatorPluginModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	FForestCreatorPluginStyle::Initialize();
	FForestCreatorPluginStyle::ReloadTextures();

	FForestCreatorPluginCommands::Register();

	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FForestCreatorPluginCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FForestCreatorPluginModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FForestCreatorPluginModule::RegisterMenus));
}

void FForestCreatorPluginModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FForestCreatorPluginStyle::Shutdown();

	FForestCreatorPluginCommands::Unregister();
}

void FForestCreatorPluginModule::PluginButtonClicked()
{
	// Put your "OnButtonClicked" stuff here

	if (AActor* FoundActor = UFCFunctionLibrary::FindObjectByClass(AForestCreator::StaticClass()))
	{
		AForestCreator* ForestCreator = StaticCast<AForestCreator*>(FoundActor);
		ForestCreator->BuildForest();
		return;
	}

	FMessageDialog::Open(EAppMsgType::Ok, FText::FromString("ForestCreator not found! Add ForestCreator on your map and try again."));
}

void FForestCreatorPluginModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FForestCreatorPluginCommands::Get().PluginAction, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginTools");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FForestCreatorPluginCommands::Get().PluginAction));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FForestCreatorPluginModule, ForestCreatorPlugin)
