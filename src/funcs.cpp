/*
 *  funcs.cpp
 *  StatusSpec project
 *
 *  Copyright (c) 2014 thesupremecommander
 *  BSD 2-Clause License
 *  http://opensource.org/licenses/BSD-2-Clause
 *
 */

#include "funcs.h"

void StatusSpecUnloader::ReadyToUnload(SourceHook::Plugin plug) {};

SourceHook::Impl::CSourceHookImpl g_SourceHook;
SourceHook::ISourceHook *g_SHPtr = &g_SourceHook;
int g_PLID = 0;

SH_DECL_MANUALHOOK5_void(C_TFPlayer_CalcView, OFFSET_CALCVIEW, 0, 0, Vector &, QAngle &, float &, float &, float &);
SH_DECL_MANUALHOOK0(C_TFPlayer_GetFOV, OFFSET_GETFOV, 0, 0, float);
SH_DECL_MANUALHOOK3_void(C_TFPlayer_GetGlowEffectColor, OFFSET_GETGLOWEFFECTCOLOR, 0, 0, float *, float *, float *);
SH_DECL_MANUALHOOK0(C_TFPlayer_GetHealth, OFFSET_GETHEALTH, 0, 0, int);
SH_DECL_MANUALHOOK0(C_TFPlayer_GetMaxHealth, OFFSET_GETMAXHEALTH, 0, 0, int);
SH_DECL_MANUALHOOK0(C_TFPlayer_GetObserverMode, OFFSET_GETOBSERVERMODE, 0, 0, int);
SH_DECL_MANUALHOOK0(C_TFPlayer_GetObserverTarget, OFFSET_GETOBSERVERTARGET, 0, 0, C_BaseEntity *);
SH_DECL_HOOK1_void(IBaseClientDLL, FrameStageNotify, SH_NOATTRIB, 0, ClientFrameStage_t);
SH_DECL_HOOK1(IClientMode, DoPostScreenSpaceEffects, SH_NOATTRIB, 0, bool, const CViewSetup *);
SH_DECL_HOOK1(IGameEventManager2, FireEventClientSide, SH_NOATTRIB, 0, bool, IGameEvent *);
SH_DECL_HOOK4(IMaterialSystem, FindMaterial, SH_NOATTRIB, 0, IMaterial *, char const *, const char *, bool, const char *);
SH_DECL_HOOK3_void(IPanel, SendMessage, SH_NOATTRIB, 0, VPANEL, KeyValues *, VPANEL);
SH_DECL_HOOK3_void(IPanel, SetPos, SH_NOATTRIB, 0, VPANEL, int, int);
SH_DECL_HOOK2(IVEngineClient, GetPlayerInfo, SH_NOATTRIB, 0, bool, int, player_info_t *);

int Funcs::setModelLastHookRegistered = 0;
std::map<int, std::function<void(C_BaseEntity *, const model_t *&)>> Funcs::setModelHooks;

GLPI_t Funcs::getLocalPlayerIndexOriginal = nullptr;
SMI_t Funcs::setModelIndexOriginal = nullptr;
SMP_t Funcs::setModelPointerOriginal = nullptr;

bool Funcs::AddDetour(void *target, void *detour, void *&original) {
	MH_STATUS addHookResult = MH_CreateHook(target, detour, &original);

	if (addHookResult != MH_OK) {
		return false;
	}

	MH_STATUS enableHookResult = MH_EnableHook(target);

	return (enableHookResult == MH_OK);
}

bool Funcs::AddDetour_GetLocalPlayerIndex(GLPI_t detour) {
	void *original;

	if (AddDetour(GetFunc_GetLocalPlayerIndex(), detour, original)) {
		getLocalPlayerIndexOriginal = reinterpret_cast<GLPI_t>(original);
		return true;
	}

	return false;
}

bool Funcs::AddDetour_C_BaseEntity_SetModelIndex(SMIH_t detour) {
	void *original;

	if (AddDetour(GetFunc_C_BaseEntity_SetModelIndex(), detour, original)) {
		setModelIndexOriginal = reinterpret_cast<SMI_t>(original);
		return true;
	}

	return false;
}

bool Funcs::AddDetour_C_BaseEntity_SetModelPointer(SMPH_t detour) {
	void *original;

	if (AddDetour(GetFunc_C_BaseEntity_SetModelPointer(), detour, original)) {
		setModelPointerOriginal = reinterpret_cast<SMP_t>(original);
		return true;
	}

	return false;
}

int Funcs::AddGlobalHook_C_TFPlayer_GetFOV(C_TFPlayer *instance, fastdelegate::FastDelegate0<float> hook, bool post) {
	return SH_ADD_MANUALHOOK(C_TFPlayer_GetFOV, instance, hook, post);
}

int Funcs::AddHook_C_BaseEntity_SetModel(std::function<void(C_BaseEntity *, const model_t *&)> hook) {
	if (setModelHooks.size() == 0) {
		AddDetour_C_BaseEntity_SetModelIndex(Detour_C_BaseEntity_SetModelIndex);
		AddDetour_C_BaseEntity_SetModelPointer(Detour_C_BaseEntity_SetModelPointer);
	}

	setModelHooks[++setModelLastHookRegistered] = hook;

	return setModelLastHookRegistered;
}

int Funcs::AddHook_IBaseClientDLL_FrameStageNotify(IBaseClientDLL *instance, fastdelegate::FastDelegate1<ClientFrameStage_t> hook, bool post) {
	return SH_ADD_HOOK(IBaseClientDLL, FrameStageNotify, instance, hook, post);
}

int Funcs::AddHook_IClientMode_DoPostScreenSpaceEffects(IClientMode *instance, fastdelegate::FastDelegate1<const CViewSetup *, bool> hook, bool post) {
	return SH_ADD_HOOK(IClientMode, DoPostScreenSpaceEffects, instance, hook, post);
}

int Funcs::AddHook_IGameEventManager2_FireEventClientSide(IGameEventManager2 *instance, fastdelegate::FastDelegate1<IGameEvent *, bool> hook, bool post) {
	return SH_ADD_HOOK(IGameEventManager2, FireEventClientSide, instance, hook, post);
}

int Funcs::AddHook_IMaterialSystem_FindMaterial(IMaterialSystem *instance, fastdelegate::FastDelegate4<char const *, const char *, bool, const char *, IMaterial *> hook, bool post) {
	return SH_ADD_HOOK(IMaterialSystem, FindMaterial, instance, hook, post);
}

int Funcs::AddHook_IPanel_SendMessage(vgui::IPanel *instance, fastdelegate::FastDelegate3<vgui::VPANEL, KeyValues *, vgui::VPANEL> hook, bool post) {
	return SH_ADD_HOOK(IPanel, SendMessage, instance, hook, post);
}

int Funcs::AddHook_IPanel_SetPos(vgui::IPanel *instance, fastdelegate::FastDelegate3<vgui::VPANEL, int, int> hook, bool post) {
	return SH_ADD_HOOK(IPanel, SetPos, instance, hook, post);
}

int Funcs::AddHook_IVEngineClient_GetPlayerInfo(IVEngineClient *instance, fastdelegate::FastDelegate2<int, player_info_t *, bool> hook, bool post) {
	return SH_ADD_HOOK(IVEngineClient, GetPlayerInfo, instance, hook, post);
}

int Funcs::CallFunc_GetLocalPlayerIndex() {
	if (getLocalPlayerIndexOriginal) {
		return getLocalPlayerIndexOriginal();
	}
	else {
		return GetFunc_GetLocalPlayerIndex()();
	}
}

void Funcs::CallFunc_C_BaseEntity_SetModelIndex(C_BaseEntity *instance, int index) {
	if (setModelIndexOriginal) {
		setModelIndexOriginal(instance, index);
	}
	else {
		GetFunc_C_BaseEntity_SetModelIndex()(instance, index);
	}
}

void Funcs::CallFunc_C_BaseEntity_SetModelPointer(C_BaseEntity *instance, const model_t *pModel) {
	if (setModelPointerOriginal) {
		setModelPointerOriginal(instance, pModel);
	}
	else {
		GetFunc_C_BaseEntity_SetModelPointer()(instance, pModel);
	}
}

void Funcs::CallFunc_C_HLTVCamera_SetPrimaryTarget(C_HLTVCamera *instance, int nEntity) {
	GetFunc_C_HLTVCamera_SetPrimaryTarget()(instance, nEntity);
}

float Funcs::CallFunc_C_TFPlayer_GetFOV(C_TFPlayer *instance) {
	return SH_MCALL(instance, C_TFPlayer_GetFOV)();
}

void Funcs::CallFunc_C_TFPlayer_GetGlowEffectColor(C_TFPlayer *instance, float *r, float *g, float *b) {
	SH_MCALL(instance, C_TFPlayer_GetGlowEffectColor)(r, g, b);
}

int Funcs::CallFunc_C_TFPlayer_GetHealth(C_TFPlayer *instance) {
	return SH_MCALL(instance, C_TFPlayer_GetHealth)();
}

int Funcs::CallFunc_C_TFPlayer_GetMaxHealth(C_TFPlayer *instance) {
	return SH_MCALL(instance, C_TFPlayer_GetMaxHealth)();
}

int Funcs::CallFunc_C_TFPlayer_GetObserverMode(C_TFPlayer *instance) {
	return SH_MCALL(instance, C_TFPlayer_GetObserverMode)();
}

C_BaseEntity *Funcs::CallFunc_C_TFPlayer_GetObserverTarget(C_TFPlayer *instance) {
	return SH_MCALL(instance, C_TFPlayer_GetObserverTarget)();
}

bool Funcs::CallFunc_IVEngineClient_GetPlayerInfo(IVEngineClient *instance, int ent_num, player_info_t *pinfo) {
	return SH_CALL(instance, &IVEngineClient::GetPlayerInfo)(ent_num, pinfo);
}

void Funcs::Detour_C_BaseEntity_SetModelIndex(C_BaseEntity *instance, void *, int index) {
	const model_t *model = Interfaces::pModelInfoClient->GetModel(index);

	for (auto iterator = setModelHooks.begin(); iterator != setModelHooks.end(); ++iterator) {
		iterator->second(instance, model);
	}

	int newIndex = Interfaces::pModelInfoClient->GetModelIndex(Interfaces::pModelInfoClient->GetModelName(model));

	Funcs::CallFunc_C_BaseEntity_SetModelIndex(instance, newIndex);
}

void Funcs::Detour_C_BaseEntity_SetModelPointer(C_BaseEntity *instance, void *, const model_t *pModel) {
	for (auto iterator = setModelHooks.begin(); iterator != setModelHooks.end(); ++iterator) {
		iterator->second(instance, pModel);
	}

	Funcs::CallFunc_C_BaseEntity_SetModelPointer(instance, pModel);
}

GLPI_t Funcs::GetFunc_GetLocalPlayerIndex() {
#if defined _WIN32
	static DWORD pointer = NULL;

	if (!pointer) {
		pointer = SignatureScan("client", GETLOCALPLAYERINDEX_SIG, GETLOCALPLAYERINDEX_MASK);

		if (!pointer) {
			throw bad_pointer("GetLocalPlayerIndex");
		}
	}

	return (GLPI_t)(pointer);
#else
	throw bad_pointer("GetLocalPlayerIndex");

	return nullptr;
#endif
}

SMI_t Funcs::GetFunc_C_BaseEntity_SetModelIndex() {
#if defined _WIN32
	static DWORD pointer = NULL;

	if (!pointer) {
		pointer = SignatureScan("client", SETMODELINDEX_SIG, SETMODELINDEX_MASK);

		if (!pointer) {
			throw bad_pointer("C_BaseEntity::SetModelIndex");
		}
	}

	return (SMI_t)(pointer);
#else
	throw bad_pointer("C_BaseEntity::SetModelIndex");

	return nullptr;
#endif
}

SMP_t Funcs::GetFunc_C_BaseEntity_SetModelPointer() {
#if defined _WIN32
	static DWORD pointer = NULL;

	if (!pointer) {
		pointer = SignatureScan("client", SETMODELPOINTER_SIG, SETMODELPOINTER_MASK);

		if (!pointer) {
			throw bad_pointer("C_BaseEntity::SetModelPointer");
		}
	}

	return (SMP_t)(pointer);
#else
	throw bad_pointer("C_BaseEntity::SetModelPointer");

	return nullptr;
#endif
}

SPT_t Funcs::GetFunc_C_HLTVCamera_SetPrimaryTarget() {
#if defined _WIN32
	static DWORD pointer = NULL;

	if (!pointer) {
		pointer = SignatureScan("client", SETPRIMARYTARGET_SIG, SETPRIMARYTARGET_MASK);

		if (!pointer) {
			throw bad_pointer("C_HLTVCamera::SetPrimaryTarget");
		}
	}

	return (SPT_t)(pointer);
#else
	throw bad_pointer("C_HLTVCamera::SetPrimaryTarget");

	return nullptr;
#endif
}

bool Funcs::RemoveDetour_GetLocalPlayerIndex() {
	if (RemoveDetour(GetFunc_GetLocalPlayerIndex())) {
		getLocalPlayerIndexOriginal = nullptr;
		return true;
	}

	return false;
}

bool Funcs::RemoveDetour_C_BaseEntity_SetModelIndex() {
	if (RemoveDetour(GetFunc_C_BaseEntity_SetModelIndex())) {
		setModelIndexOriginal = nullptr;
		return true;
	}

	return false;
}

bool Funcs::RemoveDetour_C_BaseEntity_SetModelPointer() {
	if (RemoveDetour(GetFunc_C_BaseEntity_SetModelPointer())) {
		setModelPointerOriginal = nullptr;
		return true;
	}

	return false;
}

bool Funcs::RemoveDetour(void *target) {
	MH_STATUS disableHookResult = MH_DisableHook(target);

	if (disableHookResult != MH_OK) {
		return false;
	}

	MH_STATUS removeHookResult = MH_RemoveHook(target);

	return (removeHookResult == MH_OK);
}

bool Funcs::RemoveHook(int hookID) {
	return SH_REMOVE_HOOK_ID(hookID);
}

void Funcs::RemoveHook_C_BaseEntity_SetModel(int hookID) {
	setModelHooks.erase(hookID);

	if (setModelHooks.size() == 0) {
		RemoveDetour_C_BaseEntity_SetModelIndex();
		RemoveDetour_C_BaseEntity_SetModelPointer();
	}
}

bool Funcs::Load() {
	MH_STATUS minHookResult = MH_Initialize();

	return (minHookResult == MH_OK || minHookResult == MH_ERROR_ALREADY_INITIALIZED);
}

bool Funcs::Unload() {
	g_SourceHook.UnloadPlugin(g_PLID, new StatusSpecUnloader());
	MH_STATUS minHookResult = MH_Uninitialize();

	return (minHookResult == MH_OK || minHookResult == MH_ERROR_NOT_INITIALIZED);
}

bool Funcs::Pause() {
	g_SourceHook.PausePlugin(g_PLID);
	MH_STATUS minHookResult = MH_DisableHook(MH_ALL_HOOKS);

	return (minHookResult == MH_OK || minHookResult == MH_ERROR_DISABLED);
}

bool Funcs::Unpause() {
	g_SourceHook.UnpausePlugin(g_PLID);
	MH_STATUS minHookResult = MH_EnableHook(MH_ALL_HOOKS);

	return (minHookResult == MH_OK || minHookResult == MH_ERROR_ENABLED);
}