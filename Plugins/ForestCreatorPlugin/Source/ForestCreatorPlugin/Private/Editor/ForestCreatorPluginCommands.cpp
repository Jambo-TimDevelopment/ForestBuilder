// Copyright Epic Games, Inc. All Rights Reserved.

#include "Editor/ForestCreatorPluginCommands.h"

#define LOCTEXT_NAMESPACE "FForestCreatorPluginModule"

void FForestCreatorPluginCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "ForestCreatorPlugin", "Execute ForestCreatorPlugin action", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
