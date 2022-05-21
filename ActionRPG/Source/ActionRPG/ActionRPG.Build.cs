// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ActionRPG : ModuleRules
{
	public ActionRPG(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG", "NavigationSystem", "GameplayTasks" });

		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");
		PrivateIncludePaths.Add("ActionRPG");
		PrivateIncludePaths.Add("ActionRPG/AI");
		PrivateIncludePaths.Add("ActionRPG/Boss");
		PrivateIncludePaths.Add("ActionRPG/Enemy");
		PrivateIncludePaths.Add("ActionRPG/Environment");
		PrivateIncludePaths.Add("ActionRPG/ETC");
		PrivateIncludePaths.Add("ActionRPG/Item");
		PrivateIncludePaths.Add("ActionRPG/MainCharacter");
		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
