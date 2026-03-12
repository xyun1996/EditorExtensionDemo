// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"

class UMyCustomAsset;
class SImage;
class STextBlock;

/**
 * Slate widget for displaying and editing custom asset properties
 */
class SMyAssetEditorWidget : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SMyAssetEditorWidget)
		: _Asset(nullptr)
	{}
		SLATE_ARGUMENT(UMyCustomAsset*, Asset)
	SLATE_END_ARGS()

public:
	// Construct the widget
	void Construct(const FArguments& InArgs);

	// Refresh the asset display
	void RefreshAsset();

	// Get the asset being edited
	UMyCustomAsset* GetAsset() const { return Asset; }

private:
	// Handle button click
	FReply OnRandomizeValues();

	// Handle import button
	FReply OnImportFromClipboard();

	// Handle export button
	FReply OnExportToClipboard();

	// Get color for display
	FSlateColor GetDisplayColor() const;

	// Get value text
	FText GetValueText() const;

	// Get status text
	FText GetStatusText() const;

private:
	// The asset being edited
	TObjectPtr<UMyCustomAsset> Asset;

	// Cached widgets
	TSharedPtr<STextBlock> NameTextBlock;
	TSharedPtr<STextBlock> ValueTextBlock;
	TSharedPtr<STextBlock> StatusTextBlock;
	TSharedPtr<SImage> ColorPreviewImage;
};
