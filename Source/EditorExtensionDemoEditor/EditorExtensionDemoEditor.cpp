// Copyright Epic Games, Inc. All Rights Reserved.

#include "EditorExtensionDemoEditor.h"

#include "MenuToolbarExtension/EditorCommands.h"
#include "MenuToolbarExtension/MenuToolbarHelpers.h"

#include "DetailsPanelExtension/MyCustomActorDetails.h"

#include "AssetEditorExtension/MyCustomAssetTypeActions.h"

#include "CustomAsset/MyCustomActor.h"
#include "EditorModeExtension/MyEditorMode.h"
#include "EditorModeExtension/MyEditorModeCommands.h"

#include "ToolMenus.h"
#include "LevelEditor.h"
#include "EditorModeManager.h"
#include "PropertyEditorModule.h"
#include "AssetToolsModule.h"
#include "IAssetTypeActions.h"

#include "EditorStyleSet.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Framework/Commands/UICommandList.h"
#include "Framework/Notifications/NotificationManager.h"
#include "Widgets/Notifications/SNotificationList.h"

#define LOCTEXT_NAMESPACE "FEditorExtensionDemoEditorModule"

void FEditorExtensionDemoEditorModule::StartupModule()
{
	// Register commands
	RegisterCommands();

	// Setup menu entries
	SetupMenuEntries();

	// Register custom details panel layouts
	RegisterCustomClassLayout();

	// Register custom asset types
	RegisterAssetTypes();

	// Register editor mode
	RegisterEditorMode();

	// Bind commands
	BindCommands();
}

void FEditorExtensionDemoEditorModule::ShutdownModule()
{
	// Unregister editor mode
	UnregisterEditorMode();

	// Unregister asset types
	UnregisterAssetTypes();

	// Unregister custom class layouts
	UnregisterCustomClassLayout();

	// Unregister tool menus
	if (UObjectInitialized())
	{
		if (UToolMenus* ToolMenus = UToolMenus::Get())
		{
			ToolMenus->UnregisterOwnerByName("EditorExtensionDemo");
		}
	}
}

void FEditorExtensionDemoEditorModule::RegisterCommands()
{
	FEditorExtensionDemoCommands::Register();
	PluginCommands = MakeShareable(new FUICommandList);
}

void FEditorExtensionDemoEditorModule::BindCommands()
{
	PluginCommands->MapAction(
		FEditorExtensionDemoCommands::Get().MenuCommand,
		FExecuteAction::CreateRaw(this, &FEditorExtensionDemoEditorModule::OnMenuCommandClicked),
		FCanExecuteAction()
	);

	PluginCommands->MapAction(
		FEditorExtensionDemoCommands::Get().ToolbarCommand,
		FExecuteAction::CreateRaw(this, &FEditorExtensionDemoEditorModule::OnToolbarButtonClicked),
		FCanExecuteAction()
	);
}

void FEditorExtensionDemoEditorModule::SetupMenuEntries()
{
	// Register tool menus
	RegisterMenus();
	RegisterToolbarExtensions();
}

void FEditorExtensionDemoEditorModule::RegisterMenus()
{
	// Owner for tool menus
	UToolMenus* ToolMenus = UToolMenus::Get();
	FToolMenuOwnerScoped OwnerScoped("EditorExtensionDemo");

	// Register menu section and entry
	UToolMenu* Menu = ToolMenus->ExtendMenu("LevelEditor.MainMenu.Tools");
	FToolMenuSection& Section = Menu->AddSection("EditorExtensionDemo", LOCTEXT("EditorExtensionDemoSection", "Editor Extension Demo"));

	// Add menu entry
	Section.AddMenuEntry(
		FName("EditorExtensionDemoMenuEntry"),
		LOCTEXT("EditorExtensionDemoMenuEntryLabel", "Editor Extension Demo"),
		LOCTEXT("EditorExtensionDemoMenuEntryTooltip", "Opens Editor Extension Demo functionality"),
		FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.Settings"),
		FUIAction(
			FExecuteAction::CreateRaw(this, &FEditorExtensionDemoEditorModule::OnMenuCommandClicked)
		)
	);

	Section.AddMenuEntry(
		FName("EditorExtensionDemoActivateModeEntry"),
		LOCTEXT("EditorExtensionDemoActivateModeLabel", "Activate My Editor Mode"),
		LOCTEXT("EditorExtensionDemoActivateModeTooltip", "Activate the custom editor mode"),
		FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.Edit"),
		FUIAction(
			FExecuteAction::CreateLambda([]()
			{
				GLevelEditorModeTools().ActivateMode(FMyEditorMode::EM_EditorModeId);
			})
		)
	);
}

void FEditorExtensionDemoEditorModule::RegisterToolbarExtensions()
{
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");

	// Create toolbar extender
	TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender());

	// Add extension to play toolbar
	ToolbarExtender->AddToolBarExtension(
		"Play",
		EExtensionHook::After,
		PluginCommands,
		FToolBarExtensionDelegate::CreateLambda([](FToolBarBuilder& ToolbarBuilder)
		{
			ToolbarBuilder.AddToolBarButton(
				FEditorExtensionDemoCommands::Get().ToolbarCommand,
				NAME_None,
				LOCTEXT("EditorExtensionDemoToolbarButton", "Ext Demo"),
				LOCTEXT("EditorExtensionDemoToolbarButtonTooltip", "Execute Editor Extension Demo action"),
				FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.Plus")
			);
		})
	);

	LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
}

void FEditorExtensionDemoEditorModule::RegisterCustomClassLayout()
{
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	// Register custom layout for MyCustomActor
	PropertyModule.RegisterCustomClassLayout(
		AMyCustomActor::StaticClass()->GetFName(),
		FOnGetDetailCustomizationInstance::CreateStatic(&FMyCustomActorDetails::MakeInstance)
	);

	PropertyModule.NotifyCustomizationModuleChanged();
}

void FEditorExtensionDemoEditorModule::UnregisterCustomClassLayout()
{
	if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
	{
		FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
		PropertyModule.UnregisterCustomClassLayout(AMyCustomActor::StaticClass()->GetFName());
	}
}

void FEditorExtensionDemoEditorModule::RegisterAssetTypes()
{
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	AssetCategoryBit = static_cast<EAssetTypeCategories::Type>(
		AssetTools.RegisterAdvancedAssetCategory(
			FName(TEXT("EditorExtensionDemo")),
			LOCTEXT("EditorExtensionDemoAssetCategory", "Editor Extension Demo")));

	// Register MyCustomAsset type actions
	TSharedPtr<FMyCustomAssetTypeActions> CustomAssetAction = MakeShareable(new FMyCustomAssetTypeActions(AssetCategoryBit));
	AssetTools.RegisterAssetTypeActions(CustomAssetAction.ToSharedRef());
	CreatedAssetTypeActions.Add(CustomAssetAction);
}

void FEditorExtensionDemoEditorModule::UnregisterAssetTypes()
{
	if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
		for (TSharedPtr<IAssetTypeActions> Action : CreatedAssetTypeActions)
		{
			AssetTools.UnregisterAssetTypeActions(Action.ToSharedRef());
		}
	}
	CreatedAssetTypeActions.Empty();
}

void FEditorExtensionDemoEditorModule::RegisterEditorMode()
{
	FEditorModeRegistry::Get().RegisterMode<FMyEditorMode>(
		FMyEditorMode::EM_EditorModeId,
		LOCTEXT("MyEditorModeName", "My Editor Mode"),
		FSlateIcon(FAppStyle::GetAppStyleSetName(), "EditMode.Actor.Select", "EditMode.Actor.Select.Small"),
		true,
		120
	);
}

void FEditorExtensionDemoEditorModule::UnregisterEditorMode()
{
	FEditorModeRegistry::Get().UnregisterMode(FMyEditorMode::EM_EditorModeId);
}

void FEditorExtensionDemoEditorModule::OnMenuCommandClicked()
{
	UE_LOG(LogTemp, Log, TEXT("EditorExtensionDemo Menu Command Clicked"));
	// Open a notification
	FNotificationInfo Info(LOCTEXT("MenuCommandExecuted", "Editor Extension Demo Menu Command Executed!"));
	Info.ExpireDuration = 3.0f;
	FSlateNotificationManager::Get().AddNotification(Info);
}

void FEditorExtensionDemoEditorModule::OnToolbarButtonClicked()
{
	UE_LOG(LogTemp, Log, TEXT("EditorExtensionDemo Toolbar Button Clicked"));
	// Open a notification
	FNotificationInfo Info(LOCTEXT("ToolbarCommandExecuted", "Editor Extension Demo Toolbar Command Executed!"));
	Info.ExpireDuration = 3.0f;
	FSlateNotificationManager::Get().AddNotification(Info);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FEditorExtensionDemoEditorModule, EditorExtensionDemoEditor)
