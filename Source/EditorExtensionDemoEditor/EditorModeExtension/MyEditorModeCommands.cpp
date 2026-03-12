// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyEditorModeCommands.h"

#define LOCTEXT_NAMESPACE "MyEditorModeCommands"

FMyEditorModeCommands::FMyEditorModeCommands()
	: TCommands<FMyEditorModeCommands>(
		TEXT("MyEditorMode"),
		LOCTEXT("MyEditorModeCommands", "My Editor Mode Commands"),
		NAME_None,
		FAppStyle::GetAppStyleSetName()
	)
{
}

void FMyEditorModeCommands::RegisterCommands()
{
	// Tool commands
	UI_COMMAND(SelectTool,
		"Select Tool",
		"Activate the selection tool",
		EUserInterfaceActionType::RadioButton,
		FInputChord()
	);

	UI_COMMAND(MoveTool,
		"Move Tool",
		"Activate the move tool",
		EUserInterfaceActionType::RadioButton,
		FInputChord(EModifierKey::None, EKeys::W)
	);

	UI_COMMAND(RotateTool,
		"Rotate Tool",
		"Activate the rotate tool",
		EUserInterfaceActionType::RadioButton,
		FInputChord(EModifierKey::None, EKeys::E)
	);

	UI_COMMAND(ScaleTool,
		"Scale Tool",
		"Activate the scale tool",
		EUserInterfaceActionType::RadioButton,
		FInputChord(EModifierKey::None, EKeys::R)
	);

	// Action commands
	UI_COMMAND(ResetMode,
		"Reset Mode",
		"Reset the editor mode to its default state",
		EUserInterfaceActionType::Button,
		FInputChord()
	);

	UI_COMMAND(ApplySettings,
		"Apply Settings",
		"Apply the current mode settings",
		EUserInterfaceActionType::Button,
		FInputChord(EModifierKey::Control, EKeys::Enter)
	);
}

#undef LOCTEXT_NAMESPACE
