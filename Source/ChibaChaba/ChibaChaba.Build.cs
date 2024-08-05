// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ChibaChaba : ModuleRules
{
	public ChibaChaba(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "Niagara",
			"GameplayTasks", "UMG", "NavigationSystem" });

		PrivateDependencyModuleNames.AddRange(new string[] {});

		PublicIncludePaths.AddRange(new string[] {
			"ChibaChaba",
			"ChibaChaba/Public",
			"ChibaChaba/Public/Characters/",
			"ChibaChaba/Public/Components/",
			"ChibaChaba/Public/Items/",
			"ChibaChaba/Public/Tests/",
			"ChibaChaba/Public/Weapons/",
			"ChibaChaba/Public/UI/",
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
