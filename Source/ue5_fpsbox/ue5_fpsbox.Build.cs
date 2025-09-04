// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ue5_fpsbox : ModuleRules
{
	public ue5_fpsbox(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate",
            "HTTP", 
			"Json", 
			"JsonUtilities"
        });

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"ue5_fpsbox",
			"ue5_fpsbox/Variant_Horror",
			"ue5_fpsbox/Variant_Horror/UI",
			"ue5_fpsbox/Variant_Shooter",
			"ue5_fpsbox/Variant_Shooter/AI",
			"ue5_fpsbox/Variant_Shooter/UI",
			"ue5_fpsbox/Variant_Shooter/Weapons"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
