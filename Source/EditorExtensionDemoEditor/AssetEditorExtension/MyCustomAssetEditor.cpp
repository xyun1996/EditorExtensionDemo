// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyCustomAssetEditor.h"
#include "SMyAssetEditorWidget.h"
#include "CustomAsset/MyCustomAsset.h"

#include "PropertyEditorModule.h"
#include "IDetailsView.h"

#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/SBoxPanel.h"

#include "EditorStyleSet.h"
#include "Framework/Commands/GenericCommands.h"
#include "Framework/Docking/TabManager.h"
#include "ToolMenus.h"

#define LOCTEXT_NAMESPACE "MyCustomAssetEditor"

const FName FMyCustomAssetEditorToolkit::PropertiesTabId(TEXT("MyCustomAssetEditor_Properties"));
const FName FMyCustomAssetEditorToolkit::ViewportTabId(TEXT("MyCustomAssetEditor_Viewport"));
const FName FMyCustomAssetEditorToolkit::SettingsTabId(TEXT("MyCustomAssetEditor_Settings"));
const FName FMyCustomAssetEditorToolkit::ToolkitName(TEXT("MyCustomAssetEditor"));

FMyCustomAssetEditorToolkit::FMyCustomAssetEditorToolkit()
	: EditedAsset(nullptr)
{
}

FMyCustomAssetEditorToolkit::~FMyCustomAssetEditorToolkit()
{
	if (DetailsView.IsValid())
	{
		DetailsView->OnFinishedChangingProperties().RemoveAll(this);
	}
}

void FMyCustomAssetEditorToolkit::Initialize(UMyCustomAsset* InAsset, const TSharedPtr<IToolkitHost>& EditWithinLevelEditor)
{
	EditedAsset = InAsset;

	// Bind commands
	BindCommands();

	// Create the details view
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	FDetailsViewArgs DetailsViewArgs;
	DetailsViewArgs.NotifyHook = nullptr;
	DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
	DetailsViewArgs.bHideSelectionTip = true;
	DetailsViewArgs.bShowPropertyMatrixButton = false;

	DetailsView = PropertyModule.CreateDetailView(DetailsViewArgs);
	DetailsView->SetObject(EditedAsset);

	// Listen for property changes
	DetailsView->OnFinishedChangingProperties().AddRaw(this, &FMyCustomAssetEditorToolkit::OnAssetChanged);

	// Create the editor widget
	EditorWidget = SNew(SMyAssetEditorWidget)
		.Asset(EditedAsset);

	const TSharedRef<FTabManager::FLayout> Layout = FTabManager::NewLayout("MyCustomAssetEditor_Layout_v1")
		->AddArea
		(
			FTabManager::NewPrimaryArea()
			->SetOrientation(Orient_Horizontal)
			->Split
			(
				FTabManager::NewStack()
				->AddTab(ViewportTabId, ETabState::OpenedTab)
				->SetHideTabWell(true)
			)
			->Split
			(
				FTabManager::NewSplitter()
				->SetOrientation(Orient_Vertical)
				->Split
				(
					FTabManager::NewStack()
					->AddTab(PropertiesTabId, ETabState::OpenedTab)
				)
				->Split
				(
					FTabManager::NewStack()
					->AddTab(SettingsTabId, ETabState::OpenedTab)
				)
			)
		);

	// Initialize the toolkit
	FAssetEditorToolkit::InitAssetEditor(
		EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone,
		EditWithinLevelEditor,
		ToolkitName,
		Layout,
		true,
		true,
		InAsset,
		false
	);

	// Extend toolbar
	ExtendToolbar();

	// Regenerate the menu and toolbar
	RegenerateMenusAndToolbars();
}

void FMyCustomAssetEditorToolkit::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	// Call parent implementation
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	// Register our tabs
	InTabManager->RegisterTabSpawner(PropertiesTabId, FOnSpawnTab::CreateSP(this, &FMyCustomAssetEditorToolkit::SpawnTab_AssetDetails))
		.SetDisplayName(LOCTEXT("PropertiesTabLabel", "Properties"));

	InTabManager->RegisterTabSpawner(ViewportTabId, FOnSpawnTab::CreateSP(this, &FMyCustomAssetEditorToolkit::SpawnTab_PreviewViewport))
		.SetDisplayName(LOCTEXT("ViewportTabLabel", "Viewport"));

	InTabManager->RegisterTabSpawner(SettingsTabId, FOnSpawnTab::CreateSP(this, &FMyCustomAssetEditorToolkit::SpawnTab_Settings))
		.SetDisplayName(LOCTEXT("SettingsTabLabel", "Settings"));
}

TSharedRef<SDockTab> FMyCustomAssetEditorToolkit::SpawnTab_AssetDetails(const FSpawnTabArgs& Args)
{
	check(Args.GetTabId().TabType == PropertiesTabId);

	return SNew(SDockTab)
		.Label(LOCTEXT("PropertiesTabTitle", "Properties"))
		.TabColorScale(GetWorldCentricTabColorScale())
		[
			DetailsView.ToSharedRef()
		];
}

TSharedRef<SDockTab> FMyCustomAssetEditorToolkit::SpawnTab_PreviewViewport(const FSpawnTabArgs& Args)
{
	check(Args.GetTabId().TabType == ViewportTabId);

	return SNew(SDockTab)
		.Label(LOCTEXT("ViewportTabTitle", "Preview Viewport"))
		.TabColorScale(GetWorldCentricTabColorScale())
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.FillHeight(1.0f)
			[
				EditorWidget.ToSharedRef()
			]
		];
}

TSharedRef<SDockTab> FMyCustomAssetEditorToolkit::SpawnTab_Settings(const FSpawnTabArgs& Args)
{
	check(Args.GetTabId().TabType == SettingsTabId);

	return SNew(SDockTab)
		.Label(LOCTEXT("SettingsTabTitle", "Settings"))
		.TabColorScale(GetWorldCentricTabColorScale())
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(10.0f)
			[
				SNew(SButton)
				.Text(LOCTEXT("ResetToDefaults", "Reset to Defaults"))
				.ToolTipText(LOCTEXT("ResetToDefaultsTooltip", "Reset all properties to their default values"))
				.OnClicked_Lambda([this]() -> FReply
				{
					OnResetToDefaults();
					return FReply::Handled();
				})
				.IsEnabled_Lambda([this]() -> bool
				{
					return CanResetToDefaults();
				})
			]
		];
}

void FMyCustomAssetEditorToolkit::BindCommands()
{
	// No custom commands are required for this demo toolkit yet.
}

void FMyCustomAssetEditorToolkit::ExtendToolbar()
{
	TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
	AddToolbarExtender(ToolbarExtender);
}

void FMyCustomAssetEditorToolkit::OnResetToDefaults()
{
	if (EditedAsset)
	{
		EditedAsset->AssetName = TEXT("NewCustomAsset");
		EditedAsset->Description = FText::FromString(TEXT("A custom asset for demonstration purposes."));
		EditedAsset->Value = 50.0f;
		EditedAsset->bEnabled = true;
		EditedAsset->AssetColor = FLinearColor::Blue;
		EditedAsset->IntegerArray.Empty();
		EditedAsset->StringProperties.Empty();

		if (DetailsView.IsValid())
		{
			DetailsView->ForceRefresh();
		}
	}
}

bool FMyCustomAssetEditorToolkit::CanResetToDefaults() const
{
	return EditedAsset != nullptr;
}

void FMyCustomAssetEditorToolkit::OnAssetChanged(const FPropertyChangedEvent& PropertyChangedEvent)
{
	(void)PropertyChangedEvent;

	// Can be used to update preview or other UI elements
	if (EditorWidget.IsValid())
	{
		EditorWidget->RefreshAsset();
	}
}

FName FMyCustomAssetEditorToolkit::GetToolkitFName() const
{
	return ToolkitName;
}

FText FMyCustomAssetEditorToolkit::GetBaseToolkitName() const
{
	return LOCTEXT("AppLabel", "Custom Asset Editor");
}

FText FMyCustomAssetEditorToolkit::GetToolkitName() const
{
	if (EditedAsset)
	{
		return EditedAsset->GetAssetDisplayName();
	}
	return GetBaseToolkitName();
}

FText FMyCustomAssetEditorToolkit::GetToolkitToolTipText() const
{
	if (EditedAsset)
	{
		return FText::FromString(EditedAsset->GetPathName());
	}
	return GetBaseToolkitName();
}

FString FMyCustomAssetEditorToolkit::GetWorldCentricTabPrefix() const
{
	return LOCTEXT("WorldCentricTabPrefix", "Custom Asset ").ToString();
}

FLinearColor FMyCustomAssetEditorToolkit::GetWorldCentricTabColorScale() const
{
	return FLinearColor(0.5f, 0.0f, 0.5f, 0.5f); // Purple
}

FString FMyCustomAssetEditorToolkit::GetDocumentationLink() const
{
	return TEXT("Engine/Editor/AssetEditor");
}

void FMyCustomAssetEditorToolkit::SaveAsset_Execute()
{
	if (EditedAsset)
	{
		EditedAsset->MarkPackageDirty();
	}
	FAssetEditorToolkit::SaveAsset_Execute();
}

#undef LOCTEXT_NAMESPACE
