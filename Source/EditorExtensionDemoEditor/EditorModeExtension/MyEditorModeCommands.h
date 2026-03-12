// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "EditorStyleSet.h"

/**
 * Commands for the custom editor mode
 */
class FMyEditorModeCommands : public TCommands<FMyEditorModeCommands>
{
public:
	FMyEditorModeCommands();

	// TCommands interface
	virtual void RegisterCommands() override;

public:
	// Tool commands
	TSharedPtr<FUICommandInfo> SelectTool;
	TSharedPtr<FUICommandInfo> MoveTool;
	TSharedPtr<FUICommandInfo> RotateTool;
	TSharedPtr<FUICommandInfo> ScaleTool;

	// Action commands
	TSharedPtr<FUICommandInfo> ResetMode;
	TSharedPtr<FUICommandInfo> ApplySettings;
};
