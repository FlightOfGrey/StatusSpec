/*
 *  custommaterials.cpp
 *  StatusSpec project
 *
 *  Copyright (c) 2014 thesupremecommander
 *  BSD 2-Clause License
 *  http://opensource.org/licenses/BSD-2-Clause
 *
 */

#include "custommaterials.h"

CustomMaterials::CustomMaterials(std::string name) : Module(name) {
	findMaterialHook = 0;
	materialConfig = new KeyValues("materials");
	materialConfig->LoadFromFile(Interfaces::pFileSystem, "resource/custommaterials.res", "mod");

	enabled = new ConVar("statusspec_custommaterials_enabled", "0", FCVAR_NONE, "enable custom materials", [](IConVar *var, const char *pOldValue, float flOldValue) { g_ModuleManager->GetModule<CustomMaterials>("Custom Materials")->ToggleEnabled(var, pOldValue, flOldValue); });
	load_replacement_group = new ConCommand("statusspec_custommaterials_load_replacement_group", [](const CCommand &command) { g_ModuleManager->GetModule<CustomMaterials>("Custom Materials")->LoadReplacementGroup(command); }, "load a material replacement group", FCVAR_NONE);
	unload_replacement_group = new ConCommand("statusspec_custommaterials_unload_replacement_group", [](const CCommand &command) { g_ModuleManager->GetModule<CustomMaterials>("Custom Materials")->UnloadReplacementGroup(command); }, "unload a material replacement group", FCVAR_NONE);
}

bool CustomMaterials::CheckDependencies(std::string name) {
	bool ready = true;

	if (!Interfaces::pFileSystem) {
		PRINT_TAG();
		Warning("Required interface IFileSystem for module %s not available!\n", name.c_str());

		ready = false;
	}

	if (!g_pMaterialSystem) {
		PRINT_TAG();
		Warning("Required interface IMaterialSystem for module %s not available!\n", name.c_str());

		ready = false;
	}

	return ready;
}

IMaterial *CustomMaterials::FindMaterialOverride(char const *pMaterialName, const char *pTextureGroupName, bool complain, const char *pComplainPrefix) {
	if (materialReplacements.find(pMaterialName) != materialReplacements.end()) {
		RETURN_META_VALUE_NEWPARAMS(MRES_HANDLED, nullptr, &IMaterialSystem::FindMaterial, (materialReplacements[pMaterialName].replacement.c_str(), pTextureGroupName, complain, pComplainPrefix));
	}

	RETURN_META_VALUE(MRES_IGNORED, nullptr);
}

void CustomMaterials::LoadReplacementGroup(const CCommand &command) {
	if (command.ArgC() < 2) {
		Warning("Must specify a replacement group to load!\n");
		return;
	}

	const char *group = command.Arg(1);

	KeyValues *replacementsConfig = materialConfig->FindKey(group);

	if (replacementsConfig) {
		FOR_EACH_VALUE(replacementsConfig, materialReplacement) {
			std::string original = materialReplacement->GetName();

			if (materialReplacements.find(original) != materialReplacements.end()) {
				materialReplacements.erase(original);
			}

			materialReplacements[original].group = group;
			materialReplacements[original].replacement = materialReplacement->GetString();
		}
	}
	else {
		Warning("Must specify a valid replacement group to load!\n");
	}
}

void CustomMaterials::ToggleEnabled(IConVar *var, const char *pOldValue, float flOldValue) {
	if (enabled->GetBool()) {
		if (!findMaterialHook) {
			findMaterialHook = Funcs::AddHook_IMaterialSystem_FindMaterial(g_pMaterialSystem, SH_MEMBER(this, &CustomMaterials::FindMaterialOverride), false);
		}
	}
	else {
		if (findMaterialHook) {
			if (Funcs::RemoveHook(findMaterialHook)) {
				findMaterialHook = 0;
			}
		}
	}
}

void CustomMaterials::UnloadReplacementGroup(const CCommand &command) {
	if (command.ArgC() < 2) {
		Warning("Must specify a replacement group to load!\n");
		return;
	}

	const char *group = command.Arg(1);

	KeyValues *replacementsConfig = materialConfig->FindKey(group);

	if (replacementsConfig) {
		auto iterator = materialReplacements.begin();

		while (iterator != materialReplacements.end()) {
			if (iterator->second.group.compare(group) == 0) {
				materialReplacements.erase(iterator++);
			}
			else {
				++iterator;
			}
		}
	}
	else {
		Warning("Must specify a valid replacement group to unload!\n");
	}
}