// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class HeroShooter : ModuleRules
{
	public HeroShooter(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG" }); // Core Modules

        PrivateDependencyModuleNames.AddRange(new string[] { "Json" });

        PrivateDependencyModuleNames.AddRange(new string[] { "OnlineSubsystem", "OnlineSubsystemSteam" });

        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" }); // Cpp UI

        PublicDependencyModuleNames.AddRange(new string[] { "AIModule" });

        PublicDependencyModuleNames.AddRange(new string[] { "GameplayAbilities", "GameplayTags", "GameplayTasks" }); // Gameplay ability system
    }
}
