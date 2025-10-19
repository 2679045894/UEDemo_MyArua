using UnrealBuildTool;

public class Aura : ModuleRules
{
	public Aura(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
    
		// 公共依赖模块 - 其他模块可以访问这些
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"GameplayAbilities",
			"GameplayTags", 
			"GameplayTasks",
			"UMG",
			"Slate",
			"SlateCore",
			"AIModule",
			"NavigationSystem"
		});

		// 私有依赖模块 - 仅本模块内部使用
		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"NetCore",
			"RHI",
			"ApplicationCore"
		});

		// 如果使用导航系统
		if (Target.bBuildEditor == true)
		{
			PrivateDependencyModuleNames.Add("UnrealEd");
		}
	}
}