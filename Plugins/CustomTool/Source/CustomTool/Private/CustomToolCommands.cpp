// Copyright Epic Games, Inc. All Rights Reserved.

#include "CustomToolCommands.h"

#define LOCTEXT_NAMESPACE "FCustomToolModule"

void FCustomToolCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "CustomTool", "Execute CustomTool action", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
