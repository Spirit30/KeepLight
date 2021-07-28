// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class BEAR : ModuleRules
{
	public BEAR(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore",
			"LevelSequence",
			"MovieScene",
			"PhysicsCore",
			"CableComponent"
		});
	}
}
