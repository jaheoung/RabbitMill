// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "CustomToolStyle.h"

class FCustomToolCommands : public TCommands<FCustomToolCommands>
{
public:

	FCustomToolCommands()
		: TCommands<FCustomToolCommands>(TEXT("CustomTool"), NSLOCTEXT("Contexts", "CustomTool", "CustomTool Plugin"), NAME_None, FCustomToolStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > PluginAction;
};
