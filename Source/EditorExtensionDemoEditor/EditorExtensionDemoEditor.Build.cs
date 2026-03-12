// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class EditorExtensionDemoEditor : ModuleRules
{
	public EditorExtensionDemoEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EditorExtensionDemo",
		});

		PrivateDependencyModuleNames.AddRange(new string[] {
			"ApplicationCore",
			"Slate",
			"SlateCore",
			"UnrealEd",
			"EditorFramework",
			"LevelEditor",
			"PropertyEditor",
			"ToolMenus",
			"EditorStyle",
			"EditorSubsystem",
			"AssetTools",
			"AssetRegistry",
			"WorkspaceMenuStructure",
		});
	}
}
