// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "ForestCreatorPluginStyle.h"

class FForestCreatorPluginCommands : public TCommands<FForestCreatorPluginCommands>
{
public:
	FForestCreatorPluginCommands()
		: TCommands<FForestCreatorPluginCommands>(
			TEXT("ForestCreatorPlugin"), NSLOCTEXT("Contexts", "ForestCreatorPlugin", "ForestCreatorPlugin Plugin"),
			NAME_None, FForestCreatorPluginStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr<FUICommandInfo> PluginAction;
};
