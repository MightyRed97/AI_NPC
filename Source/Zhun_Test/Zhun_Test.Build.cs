// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Zhun_Test : ModuleRules
{
	public Zhun_Test(ReadOnlyTargetRules Target) : base(Target)
	{
		//PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		PCHUsage = PCHUsageMode.UseSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "GameplayTasks", "NavigationSystem", "UMG", "Slate" });
	}
}
