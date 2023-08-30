// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Bushcraft : ModuleRules
{
	public Bushcraft(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		PrivatePCHHeaderFile = "Precompiled.h";
		MinFilesUsingPrecompiledHeaderOverride = 1;
		bUseUnity = false;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput", "Niagara", "ProceduralMeshComponent" });
	}
}
