// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "EditorStyleSet.h"

/**
 * Editor extension commands for the plugin
 */
class FEditorExtensionDemoCommands : public TCommands<FEditorExtensionDemoCommands>
{
public:
	FEditorExtensionDemoCommands();

	// TCommands interface
	virtual void RegisterCommands() override;

public:
	// Menu command
	TSharedPtr<FUICommandInfo> MenuCommand;

	// Toolbar command
	TSharedPtr<FUICommandInfo> ToolbarCommand;

	// Editor mode command
	TSharedPtr<FUICommandInfo> EnterEditorMode;
};
