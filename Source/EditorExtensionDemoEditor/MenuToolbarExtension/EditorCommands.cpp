// Copyright Epic Games, Inc. All Rights Reserved.

#include "EditorCommands.h"

#define LOCTEXT_NAMESPACE "EditorExtensionDemoCommands"

FEditorExtensionDemoCommands::FEditorExtensionDemoCommands()
	: TCommands<FEditorExtensionDemoCommands>(
		TEXT("EditorExtensionDemo"),
		LOCTEXT("EditorExtensionDemoCommands", "Editor Extension Demo"),
		NAME_None,
		FAppStyle::GetAppStyleSetName()
	)
{
}

void FEditorExtensionDemoCommands::RegisterCommands()
{
	// Menu command
	UI_COMMAND(MenuCommand,
		"Editor Extension Demo Menu",
		"Execute Editor Extension Demo menu action",
		EUserInterfaceActionType::Button,
		FInputChord()
	);

	// Toolbar command
	UI_COMMAND(ToolbarCommand,
		"Editor Extension Demo Toolbar",
		"Execute Editor Extension Demo toolbar action",
		EUserInterfaceActionType::Button,
		FInputChord(EModifierKey::Control, EKeys::E)
	);

	// Editor mode command
	UI_COMMAND(EnterEditorMode,
		"Enter My Editor Mode",
		"Enter the custom editor mode",
		EUserInterfaceActionType::Button,
		FInputChord(EModifierKey::Control | EModifierKey::Shift, EKeys::M)
	);
}

#undef LOCTEXT_NAMESPACE
