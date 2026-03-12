// Copyright Epic Games, Inc. All Rights Reserved.

#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"

#include "AssetToolsModule.h"
#include "EditorModeRegistry.h"
#include "PropertyEditorModule.h"
#include "ToolMenus.h"

#include "AssetEditorExtension/MyCustomAssetFactory.h"
#include "CustomAsset/MyCustomAsset.h"
#include "CustomAsset/MyCustomActor.h"
#include "MenuToolbarExtension/EditorCommands.h"
#include "EditorModeExtension/MyEditorMode.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FEditorExtensionDemoMenuEntryTest,
	"EditorExtensionDemo.Menu.ToolsEntryExists",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FEditorExtensionDemoMenuEntryTest::RunTest(const FString& Parameters)
{
	UToolMenus* ToolMenus = UToolMenus::Get();
	TestNotNull(TEXT("ToolMenus must be available"), ToolMenus);
	if (!ToolMenus)
	{
		return false;
	}

	UToolMenu* ToolsMenu = ToolMenus->FindMenu("LevelEditor.MainMenu.Tools");
	TestNotNull(TEXT("LevelEditor Tools menu should exist"), ToolsMenu);
	if (!ToolsMenu)
	{
		return false;
	}

	FToolMenuSection* Section = ToolsMenu->FindSection("EditorExtensionDemo");
	TestNotNull(TEXT("EditorExtensionDemo section should be registered"), Section);
	if (!Section)
	{
		return false;
	}

	const FToolMenuEntry* Entry = Section->FindEntry("EditorExtensionDemoMenuEntry");
	TestNotNull(TEXT("EditorExtensionDemo menu entry should exist"), Entry);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FEditorExtensionDemoToolbarCommandTest,
	"EditorExtensionDemo.Toolbar.CommandRegistered",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FEditorExtensionDemoToolbarCommandTest::RunTest(const FString& Parameters)
{
	const TSharedPtr<FUICommandInfo>& ToolbarCommand = FEditorExtensionDemoCommands::Get().ToolbarCommand;
	TestTrue(TEXT("Toolbar command should be registered"), ToolbarCommand.IsValid());
	return ToolbarCommand.IsValid();
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FEditorExtensionDemoModeRegistryTest,
	"EditorExtensionDemo.Mode.RegistryContainsMode",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FEditorExtensionDemoModeRegistryTest::RunTest(const FString& Parameters)
{
	const bool bModeRegistered = FEditorModeRegistry::Get().GetFactoryMap().Contains(FMyEditorMode::EM_EditorModeId);
	TestTrue(TEXT("My Editor Mode should be registered"), bModeRegistered);
	return bModeRegistered;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FEditorExtensionDemoAssetFactoryTest,
	"EditorExtensionDemo.Asset.FactoryCanCreateAsset",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FEditorExtensionDemoAssetFactoryTest::RunTest(const FString& Parameters)
{
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

	UMyCustomAssetFactory* Factory = NewObject<UMyCustomAssetFactory>();
	TestNotNull(TEXT("Factory must be constructed"), Factory);
	if (!Factory)
	{
		return false;
	}

	const FString AssetName = FString::Printf(TEXT("AT_MyCustomAsset_%s"), *FGuid::NewGuid().ToString(EGuidFormats::Digits));
	UObject* Created = AssetTools.CreateAsset(AssetName, TEXT("/Game"), UMyCustomAsset::StaticClass(), Factory);
	UMyCustomAsset* CreatedAsset = Cast<UMyCustomAsset>(Created);

	TestNotNull(TEXT("Factory should create UMyCustomAsset"), CreatedAsset);
	return CreatedAsset != nullptr;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FEditorExtensionDemoDetailsCustomizationTest,
	"EditorExtensionDemo.Details.CustomLayoutRegistered",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FEditorExtensionDemoDetailsCustomizationTest::RunTest(const FString& Parameters)
{
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	const FName ClassName = AMyCustomActor::StaticClass()->GetFName();
	const bool bRegistered = PropertyModule.GetClassNameToDetailLayoutNameMap().Contains(ClassName);
	TestTrue(TEXT("AMyCustomActor detail customization should be registered"), bRegistered);
	return bRegistered;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FEditorExtensionDemoAssetTypeActionsTest,
	"EditorExtensionDemo.Asset.TypeActionsRegistered",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FEditorExtensionDemoAssetTypeActionsTest::RunTest(const FString& Parameters)
{
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	TWeakPtr<IAssetTypeActions> WeakActions = AssetTools.GetAssetTypeActionsForClass(UMyCustomAsset::StaticClass());
	const TSharedPtr<IAssetTypeActions> Actions = WeakActions.Pin();
	if (!TestTrue(TEXT("Asset type actions should be registered for UMyCustomAsset"), Actions.IsValid()))
	{
		return false;
	}

	TestEqual(TEXT("Registered asset type action should target UMyCustomAsset"),
		Actions->GetSupportedClass(),
		UMyCustomAsset::StaticClass());
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
