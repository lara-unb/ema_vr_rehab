// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

using System;
using System.IO;


public class ema_vr_rehab : ModuleRules
{
    private string ModulePath
    {
        get { return ModuleDirectory; }
    }

    private string ThirdPartyPath
    {
        get { return Path.GetFullPath(Path.Combine(ModulePath, "../../ThirdParty/")); }
    }

    public ema_vr_rehab(ReadOnlyTargetRules Target) : base(Target)
	{
        //PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        //PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });

        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        string BSONPath = Path.Combine(ThirdPartyPath, "bson");

        // Console.WriteLine("");
        Console.WriteLine("BSONPath: " + BSONPath);

        // Include std::string functions for rapidjson
        Definitions.Add("RAPIDJSON_HAS_STDSTRING=1");


        PublicIncludePaths.AddRange(
            new string[] {
                "../../Plugins/ROSIntegration/Source/ROSIntegration/Public",
                "../../Plugins/ROSIntegration/Source/ROSIntegration/Classes",
				// ... add public include paths required here ...
			}
            );


        PrivateIncludePaths.AddRange(
            new string[] {
                "../../Plugins/ROSIntegration/Source/ROSIntegration/Private",
                "../../Plugins/ROSIntegration/Source/ROSIntegration/Private/rosbridge2cpp"
				// ... add other private include paths required here ...
			}
            );


        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "InputCore",
                "Sockets",
                "Networking",
                "HeadMountedDisplay",
                "ROSIntegration"
				// ... add other public dependencies that you statically link with here ...
			}
            );


        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "Sockets",
                "Networking"
				// ... add private dependencies that you statically link with here ...	
			}
            );


        DynamicallyLoadedModuleNames.AddRange(
            new string[]
            {
				// ... add any modules that your module loads dynamically here ...
			}
            );



        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            Console.WriteLine("Using Windows BSON files");
            PublicAdditionalLibraries.Add(Path.Combine(BSONPath, "lib", "bson-static-1.0.lib"));
            PublicIncludePaths.Add(Path.Combine(BSONPath, "include", "windows"));
        }
        else if (Target.Platform == UnrealTargetPlatform.Linux)
        {
            Console.WriteLine("Using Linux BSON files");
            PublicAdditionalLibraries.Add(Path.Combine(BSONPath, "lib", "libbson-1.0.a"));
            PublicIncludePaths.Add(Path.Combine(BSONPath, "include", "linux"));
        }
    }
}
