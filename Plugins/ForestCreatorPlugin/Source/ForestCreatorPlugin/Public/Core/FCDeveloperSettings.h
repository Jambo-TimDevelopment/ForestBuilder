

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "FCDeveloperSettings.generated.h"

/** Contains default Game Settings for Project*/
UCLASS(Config = Game, DefaultConfig, Meta = (DisplayName = "Developer FC Settings"))
class FORESTCREATORPLUGIN_API UFCDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	// Begin UDeveloperSettings Override
	virtual FName GetCategoryName() const override { return TEXT("Settings"); }
#if WITH_EDITOR
	// virtual FText GetSectionText() const override { return FText(INVTEXT("Game Settings")); }
	virtual FText GetSectionDescription() const override { return FText(INVTEXT("Default Settings configuration")); }
#endif
	// End UDeveloperSettings Override

	UPROPERTY(Config, EditDefaultsOnly)
	int32 Seed;
};
