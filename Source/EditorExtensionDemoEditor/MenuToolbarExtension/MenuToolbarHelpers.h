// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "ToolMenus.h"

/**
 * Helper functions for menu and toolbar extensions
 */
namespace MenuToolbarHelpers
{
	// Build a simple menu
	void BuildSimpleMenu(FMenuBuilder& MenuBuilder);

	// Build a simple toolbar
	void BuildSimpleToolbar(FToolBarBuilder& ToolbarBuilder);

	// Register tool menu entries
	void RegisterToolMenuEntries(UToolMenus* ToolMenus);

	// Create menu extender
	TSharedPtr<FExtender> CreateMenuExtender(const TSharedPtr<FUICommandList>& CommandList);

	// Create toolbar extender
	TSharedPtr<FExtender> CreateToolbarExtender(const TSharedPtr<FUICommandList>& CommandList);

	// Helper to add a separator to menu
	void AddMenuSeparator(FMenuBuilder& MenuBuilder, FName ExtensionHook = NAME_None);

	// Helper to add a menu section
	void AddMenuSection(FMenuBuilder& MenuBuilder, const FText& SectionLabel);
}
