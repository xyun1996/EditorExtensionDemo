// Copyright Epic Games, Inc. All Rights Reserved.

#include "SMyAssetEditorWidget.h"
#include "CustomAsset/MyCustomAsset.h"

#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SSpinBox.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SGridPanel.h"

#include "EditorStyleSet.h"
#include "HAL/PlatformApplicationMisc.h"

#define LOCTEXT_NAMESPACE "SMyAssetEditorWidget"

void SMyAssetEditorWidget::Construct(const FArguments& InArgs)
{
	Asset = InArgs._Asset;

	// Build the widget layout
	ChildSlot
	[
		SNew(SVerticalBox)
		// Header section
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(10.0f, 5.0f)
		[
			SNew(SBorder)
			.BorderImage(FAppStyle::GetBrush("ToolPanel.GroupBorder"))
			.Padding(5.0f)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.FillWidth(1.0f)
				.VAlign(VAlign_Center)
				[
					SNew(STextBlock)
					.Text(LOCTEXT("AssetInfoHeader", "Custom Asset Preview"))
					.Font(FAppStyle::GetFontStyle("PropertyWindow.BoldFont"))
				]
				+ SHorizontalBox::Slot()
				.AutoWidth()
				[
					SNew(SButton)
					.Text(LOCTEXT("Refresh", "Refresh"))
					.ToolTipText(LOCTEXT("RefreshTooltip", "Refresh the asset display"))
					.OnClicked_Lambda([this]() -> FReply
					{
						RefreshAsset();
						return FReply::Handled();
					})
				]
			]
		]

		// Asset Name
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(10.0f, 5.0f)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("AssetNameLabel", "Asset Name: "))
				.Font(FAppStyle::GetFontStyle("PropertyWindow.NormalFont"))
			]
			+ SHorizontalBox::Slot()
			.FillWidth(1.0f)
			.VAlign(VAlign_Center)
			[
				SAssignNew(NameTextBlock, STextBlock)
				.Text(this, &SMyAssetEditorWidget::GetValueText)
				.Font(FAppStyle::GetFontStyle("PropertyWindow.BoldFont"))
			]
		]

		// Value display
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(10.0f, 5.0f)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("ValueLabel", "Value: "))
			]
			+ SHorizontalBox::Slot()
			.FillWidth(1.0f)
			.VAlign(VAlign_Center)
			[
				SAssignNew(ValueTextBlock, STextBlock)
				.Text(this, &SMyAssetEditorWidget::GetValueText)
			]
		]

		// Color preview
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(10.0f, 5.0f)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("ColorLabel", "Asset Color: "))
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(5.0f, 0.0f)
			[
				SAssignNew(ColorPreviewImage, SImage)
				.Image(FAppStyle::GetBrush("WhiteBrush"))
				.ColorAndOpacity(this, &SMyAssetEditorWidget::GetDisplayColor)
			]
		]

		// Status
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(10.0f, 5.0f)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("StatusLabel", "Status: "))
			]
			+ SHorizontalBox::Slot()
			.FillWidth(1.0f)
			.VAlign(VAlign_Center)
			[
				SAssignNew(StatusTextBlock, STextBlock)
				.Text(this, &SMyAssetEditorWidget::GetStatusText)
			]
		]

		// Action buttons
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(10.0f, 10.0f)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(0.0f, 0.0f, 5.0f, 0.0f)
			[
				SNew(SButton)
				.Text(LOCTEXT("Randomize", "Randomize Values"))
				.ToolTipText(LOCTEXT("RandomizeTooltip", "Randomize the asset values"))
				.OnClicked(this, &SMyAssetEditorWidget::OnRandomizeValues)
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(5.0f, 0.0f)
			[
				SNew(SButton)
				.Text(LOCTEXT("ImportClipboard", "Import from Clipboard"))
				.ToolTipText(LOCTEXT("ImportClipboardTooltip", "Import asset data from clipboard"))
				.OnClicked(this, &SMyAssetEditorWidget::OnImportFromClipboard)
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(5.0f, 0.0f)
			[
				SNew(SButton)
				.Text(LOCTEXT("ExportClipboard", "Export to Clipboard"))
				.ToolTipText(LOCTEXT("ExportClipboardTooltip", "Export asset data to clipboard"))
				.OnClicked(this, &SMyAssetEditorWidget::OnExportToClipboard)
			]
		]
	];
}

void SMyAssetEditorWidget::RefreshAsset()
{
	// Force refresh of bound properties
	if (NameTextBlock.IsValid())
	{
		NameTextBlock->Invalidate(EInvalidateWidget::Layout);
	}
	if (ValueTextBlock.IsValid())
	{
		ValueTextBlock->Invalidate(EInvalidateWidget::Layout);
	}
	if (StatusTextBlock.IsValid())
	{
		StatusTextBlock->Invalidate(EInvalidateWidget::Layout);
	}
	if (ColorPreviewImage.IsValid())
	{
		ColorPreviewImage->Invalidate(EInvalidateWidget::Layout);
	}
}

FReply SMyAssetEditorWidget::OnRandomizeValues()
{
	if (Asset)
	{
		// Randomize some values
		Asset->Value = FMath::FRandRange(0.0f, 100.0f);

		// Random color
		Asset->AssetColor = FLinearColor(
			FMath::FRand(),
			FMath::FRand(),
			FMath::FRand(),
			1.0f
		);

		// Random boolean
		Asset->bEnabled = FMath::RandBool();

		// Random integer array
		Asset->IntegerArray.Empty();
		int32 ArraySize = FMath::RandRange(0, 5);
		for (int32 i = 0; i < ArraySize; ++i)
		{
			Asset->IntegerArray.Add(FMath::RandRange(1, 100));
		}

		RefreshAsset();
	}
	return FReply::Handled();
}

FReply SMyAssetEditorWidget::OnImportFromClipboard()
{
	if (Asset)
	{
		FString ClipboardContent;
		FPlatformApplicationMisc::ClipboardPaste(ClipboardContent);

		// Try to parse comma-separated values
		TArray<FString> Values;
		ClipboardContent.ParseIntoArray(Values, TEXT(","));

		if (Values.Num() >= 1)
		{
			Asset->AssetName = Values[0];
		}
		if (Values.Num() >= 2)
		{
			Asset->Value = FCString::Atof(*Values[1]);
		}
		if (Values.Num() >= 5)
		{
			float R = FCString::Atof(*Values[2]);
			float G = FCString::Atof(*Values[3]);
			float B = FCString::Atof(*Values[4]);
			Asset->AssetColor = FLinearColor(R, G, B, 1.0f);
		}

		RefreshAsset();
	}
	return FReply::Handled();
}

FReply SMyAssetEditorWidget::OnExportToClipboard()
{
	if (Asset)
	{
		FString ExportString = FString::Printf(TEXT("%s,%.2f,%.2f,%.2f,%.2f"),
			*Asset->AssetName,
			Asset->Value,
			Asset->AssetColor.R,
			Asset->AssetColor.G,
			Asset->AssetColor.B
		);

		FPlatformApplicationMisc::ClipboardCopy(*ExportString);
	}
	return FReply::Handled();
}

FSlateColor SMyAssetEditorWidget::GetDisplayColor() const
{
	if (Asset)
	{
		return Asset->AssetColor;
	}
	return FLinearColor::Gray;
}

FText SMyAssetEditorWidget::GetValueText() const
{
	if (Asset)
	{
		return FText::AsNumber(Asset->Value);
	}
	return FText::GetEmpty();
}

FText SMyAssetEditorWidget::GetStatusText() const
{
	if (Asset)
	{
		if (Asset->bEnabled)
		{
			return LOCTEXT("StatusEnabled", "Enabled");
		}
		else
		{
			return LOCTEXT("StatusDisabled", "Disabled");
		}
	}
	return LOCTEXT("StatusNoAsset", "No Asset");
}

#undef LOCTEXT_NAMESPACE
