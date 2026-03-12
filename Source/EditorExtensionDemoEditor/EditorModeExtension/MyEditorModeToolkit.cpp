// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyEditorModeToolkit.h"
#include "MyEditorMode.h"
#include "MyEditorModeCommands.h"

#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Docking/SDockTab.h"

#include "EditorStyleSet.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Framework/Commands/UICommandList.h"

#define LOCTEXT_NAMESPACE "MyEditorModeToolkit"

const FName FMyEditorModeToolkit::ModePanelTabId(TEXT("MyEditorModePanel"));

FMyEditorModeToolkit::FMyEditorModeToolkit()
{
	// Define available tools
	AvailableTools.Add(FName("Tool_Select"));
	AvailableTools.Add(FName("Tool_Move"));
	AvailableTools.Add(FName("Tool_Rotate"));
	AvailableTools.Add(FName("Tool_Scale"));
}

FMyEditorModeToolkit::~FMyEditorModeToolkit()
{
}

void FMyEditorModeToolkit::Init(const TSharedPtr<IToolkitHost>& InitToolkitHost)
{
	// Register commands
	FMyEditorModeCommands::Register();

	// Create command list
	TSharedRef<FUICommandList> CommandList = MakeShareable(new FUICommandList);

	// Setup mode commands
	TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender);

	// Initialize the toolkit
	FModeToolkit::Init(InitToolkitHost);
}

TSharedPtr<SWidget> FMyEditorModeToolkit::CreateModePanel()
{
	return SNew(SVerticalBox)
		// Header
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(5.0f)
		[
			SNew(SBorder)
			.BorderImage(FAppStyle::GetBrush("ToolPanel.GroupBorder"))
			.Padding(5.0f)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("ModeTitle", "My Editor Mode"))
				.Font(FAppStyle::GetFontStyle("PropertyWindow.BoldFont"))
			]
		]

		// Tool Buttons
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(5.0f, 10.0f)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(2.0f)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("ToolsLabel", "Tools:"))
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(2.0f)
			[
				SNew(SButton)
				.Text(LOCTEXT("SelectTool", "Select Tool"))
				.ToolTipText(LOCTEXT("SelectToolTooltip", "Selection tool for picking objects"))
				.HAlign(HAlign_Center)
				.OnClicked(this, &FMyEditorModeToolkit::OnResetMode)
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(2.0f)
			[
				SNew(SButton)
				.Text(LOCTEXT("MoveTool", "Move Tool"))
				.ToolTipText(LOCTEXT("MoveToolTooltip", "Tool for moving objects"))
				.HAlign(HAlign_Center)
				.OnClicked_Lambda([]() -> FReply
				{
					UE_LOG(LogTemp, Log, TEXT("Move Tool Selected"));
					return FReply::Handled();
				})
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(2.0f)
			[
				SNew(SButton)
				.Text(LOCTEXT("RotateTool", "Rotate Tool"))
				.ToolTipText(LOCTEXT("RotateToolTooltip", "Tool for rotating objects"))
				.HAlign(HAlign_Center)
				.OnClicked_Lambda([]() -> FReply
				{
					UE_LOG(LogTemp, Log, TEXT("Rotate Tool Selected"));
					return FReply::Handled();
				})
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(2.0f)
			[
				SNew(SButton)
				.Text(LOCTEXT("ScaleTool", "Scale Tool"))
				.ToolTipText(LOCTEXT("ScaleToolTooltip", "Tool for scaling objects"))
				.HAlign(HAlign_Center)
				.OnClicked_Lambda([]() -> FReply
				{
					UE_LOG(LogTemp, Log, TEXT("Scale Tool Selected"));
					return FReply::Handled();
				})
			]
		]

		// Info section
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(5.0f, 10.0f)
		[
			SNew(SBorder)
			.BorderImage(FAppStyle::GetBrush("ToolPanel.DarkGroupBorder"))
			.Padding(5.0f)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("ModeInfoText", "Click in the viewport to place markers.\nPress ESC to reset.\n\nThis demonstrates a custom editor mode with viewport interaction."))
				.AutoWrapText(true)
			]
		]

		// Reset button
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(5.0f, 10.0f)
		.HAlign(HAlign_Center)
		[
			SNew(SButton)
			.Text(LOCTEXT("ResetMode", "Reset Mode"))
			.ToolTipText(LOCTEXT("ResetModeTooltip", "Reset the mode to its initial state"))
			.OnClicked(this, &FMyEditorModeToolkit::OnResetMode)
		];
}

TSharedPtr<SWidget> FMyEditorModeToolkit::GetInlineContent() const
{
	return const_cast<FMyEditorModeToolkit*>(this)->CreateModePanel();
}

void FMyEditorModeToolkit::OnToolButtonClick(int32 ToolIndex)
{
	if (TSharedPtr<FMyEditorMode> Mode = EditorMode.Pin())
	{
		if (AvailableTools.IsValidIndex(ToolIndex))
		{
			Mode->SetCurrentTool(AvailableTools[ToolIndex]);
		}
	}
}

bool FMyEditorModeToolkit::IsToolButtonSelected(int32 ToolIndex) const
{
	if (TSharedPtr<FMyEditorMode> Mode = EditorMode.Pin())
	{
		if (AvailableTools.IsValidIndex(ToolIndex))
		{
			return Mode->GetCurrentTool() == AvailableTools[ToolIndex];
		}
	}
	return false;
}

FReply FMyEditorModeToolkit::OnResetMode()
{
	UE_LOG(LogTemp, Log, TEXT("Reset MyEditorMode"));
	return FReply::Handled();
}

FName FMyEditorModeToolkit::GetToolkitFName() const
{
	return FName("MyEditorMode");
}

FText FMyEditorModeToolkit::GetBaseToolkitName() const
{
	return LOCTEXT("ToolkitName", "My Editor Mode");
}

FString FMyEditorModeToolkit::GetWorldCentricTabPrefix() const
{
	return LOCTEXT("WorldCentricTabPrefix", "MyEditorMode").ToString();
}

FLinearColor FMyEditorModeToolkit::GetWorldCentricTabColorScale() const
{
	return FLinearColor(0.3f, 0.5f, 0.7f, 0.5f);
}

#undef LOCTEXT_NAMESPACE
