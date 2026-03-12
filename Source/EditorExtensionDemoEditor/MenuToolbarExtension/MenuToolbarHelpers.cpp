// Copyright Epic Games, Inc. All Rights Reserved.

#include "MenuToolbarHelpers.h"
#include "EditorCommands.h"
#include "EditorStyleSet.h"
#include "Framework/Commands/UICommandList.h"

#define LOCTEXT_NAMESPACE "MenuToolbarHelpers"

void MenuToolbarHelpers::BuildSimpleMenu(FMenuBuilder& MenuBuilder)
{
	MenuBuilder.BeginSection("EditorExtensionDemoSection", LOCTEXT("EditorExtensionDemoSection", "Editor Extension Demo"));
	{
		MenuBuilder.AddMenuEntry(FEditorExtensionDemoCommands::Get().MenuCommand);
	}
	MenuBuilder.EndSection();
}

void MenuToolbarHelpers::BuildSimpleToolbar(FToolBarBuilder& ToolbarBuilder)
{
	ToolbarBuilder.BeginSection("EditorExtensionDemoSection");
	{
		ToolbarBuilder.AddToolBarButton(FEditorExtensionDemoCommands::Get().ToolbarCommand);
	}
	ToolbarBuilder.EndSection();
}

void MenuToolbarHelpers::RegisterToolMenuEntries(UToolMenus* ToolMenus)
{
	// Register in the Tools menu
	UToolMenu* ToolsMenu = ToolMenus->ExtendMenu("LevelEditor.MainMenu.Tools");

	if (ToolsMenu)
	{
		FToolMenuSection& Section = ToolsMenu->FindOrAddSection("EditorExtensionDemo");

		// Add a submenu
		FToolMenuEntry SubMenuEntry = FToolMenuEntry::InitSubMenu(
			"EditorExtensionDemoSubMenu",
			LOCTEXT("EditorExtensionDemoSubMenu", "Editor Extension Demo"),
			LOCTEXT("EditorExtensionDemoSubMenuTooltip", "Contains Editor Extension Demo actions"),
			FNewToolMenuDelegate::CreateLambda([](UToolMenu* SubMenu)
			{
				FToolMenuSection& SubSection = SubMenu->AddSection("Actions");
				SubSection.AddMenuEntry(
					"MenuCommand",
					LOCTEXT("ExecuteMenuCommand", "Execute Menu Command"),
					LOCTEXT("ExecuteMenuCommandTooltip", "Execute a sample menu command"),
					FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.Settings"),
					FUIAction(FExecuteAction::CreateLambda([]()
					{
						UE_LOG(LogTemp, Log, TEXT("Editor Extension Demo submenu command executed"));
					}))
				);
			}),
			false,
			FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.Settings")
		);
		Section.AddEntry(SubMenuEntry);
	}
}

TSharedPtr<FExtender> MenuToolbarHelpers::CreateMenuExtender(const TSharedPtr<FUICommandList>& CommandList)
{
	TSharedPtr<FExtender> Extender = MakeShareable(new FExtender());

	Extender->AddMenuExtension(
		"FileProject",
		EExtensionHook::After,
		CommandList,
		FMenuExtensionDelegate::CreateLambda([](FMenuBuilder& MenuBuilder)
		{
			MenuBuilder.BeginSection("EditorExtensionDemoProjectSection", LOCTEXT("ProjectSection", "Editor Extension Demo"));
			{
				MenuBuilder.AddMenuEntry(FEditorExtensionDemoCommands::Get().MenuCommand);
			}
			MenuBuilder.EndSection();
		})
	);

	return Extender;
}

TSharedPtr<FExtender> MenuToolbarHelpers::CreateToolbarExtender(const TSharedPtr<FUICommandList>& CommandList)
{
	TSharedPtr<FExtender> Extender = MakeShareable(new FExtender());

	Extender->AddToolBarExtension(
		"Play",
		EExtensionHook::After,
		CommandList,
		FToolBarExtensionDelegate::CreateLambda([](FToolBarBuilder& ToolbarBuilder)
		{
			ToolbarBuilder.BeginSection("EditorExtensionDemoSection");
			{
				ToolbarBuilder.AddToolBarButton(FEditorExtensionDemoCommands::Get().ToolbarCommand);
			}
			ToolbarBuilder.EndSection();
		})
	);

	return Extender;
}

void MenuToolbarHelpers::AddMenuSeparator(FMenuBuilder& MenuBuilder, FName ExtensionHook)
{
	MenuBuilder.AddMenuSeparator(ExtensionHook);
}

void MenuToolbarHelpers::AddMenuSection(FMenuBuilder& MenuBuilder, const FText& SectionLabel)
{
	MenuBuilder.BeginSection(NAME_None, SectionLabel);
	MenuBuilder.EndSection();
}

#undef LOCTEXT_NAMESPACE
