/*
 *  statusspec.cpp
 *  StatusSpec project
 *  
 *  Copyright (c) 2014 thesupremecommander
 *  BSD 2-Clause License
 *  http://opensource.org/licenses/BSD-2-Clause
 *
 */

#include "statusspec.h"

AntiFreeze *g_AntiFreeze = nullptr;
CameraTools *g_CameraTools = nullptr;
CustomMaterials *g_CustomMaterials = nullptr;
CustomModels *g_CustomModels = nullptr;
CustomTextures *g_CustomTextures = nullptr;
FOVOverride *g_FOVOverride = nullptr;
Killstreaks *g_Killstreaks = nullptr;
LoadoutIcons *g_LoadoutIcons = nullptr;
LocalPlayer *g_LocalPlayer = nullptr;
MedigunInfo *g_MedigunInfo = nullptr;
MultiPanel *g_MultiPanel = nullptr;
PlayerAliases *g_PlayerAliases = nullptr;
PlayerModels *g_PlayerModels = nullptr;
PlayerOutlines *g_PlayerOutlines = nullptr;
ProjectileOutlines *g_ProjectileOutlines = nullptr;
SpecGUIOrder *g_SpecGUIOrder = nullptr;
StatusIcons *g_StatusIcons = nullptr;
TeamHealthComparison *g_TeamHealthComparison = nullptr;
TeamOverrides *g_TeamOverrides = nullptr;
ModuleManager *g_ModuleManager = nullptr;

static int doPostScreenSpaceEffectsHook = 0;
static int frameHook = 0;

void Hook_IBaseClientDLL_FrameStageNotify(ClientFrameStage_t curStage) {
	if (!doPostScreenSpaceEffectsHook) {
		try {
			doPostScreenSpaceEffectsHook = Funcs::AddHook_IClientMode_DoPostScreenSpaceEffects(Interfaces::GetClientMode(), SH_STATIC(Hook_IClientMode_DoPostScreenSpaceEffects), false);
		}
		catch (bad_pointer &e) {
			Warning(e.what());
		}
	}

	if (doPostScreenSpaceEffectsHook) {
		if (Funcs::RemoveHook(frameHook)) {
			frameHook = 0;
		}
	}

	RETURN_META(MRES_IGNORED);
}

bool Hook_IClientMode_DoPostScreenSpaceEffects(const CViewSetup *pSetup) {
	g_GlowObjectManager.RenderGlowEffects(pSetup);

	RETURN_META_VALUE(MRES_OVERRIDE, true);
}

// The plugin is a static singleton that is exported as an interface
StatusSpecPlugin g_StatusSpecPlugin;
EXPOSE_SINGLE_INTERFACE_GLOBALVAR(StatusSpecPlugin, IServerPluginCallbacks, INTERFACEVERSION_ISERVERPLUGINCALLBACKS, g_StatusSpecPlugin);

StatusSpecPlugin::StatusSpecPlugin()
{
}

StatusSpecPlugin::~StatusSpecPlugin()
{
}

bool StatusSpecPlugin::Load(CreateInterfaceFn interfaceFactory, CreateInterfaceFn gameServerFactory)
{
	if (!Interfaces::Load(interfaceFactory, gameServerFactory)) {
		Warning("[%s] Unable to load required libraries!\n", PLUGIN_DESC);
		return false;
	}

	if (!Entities::PrepareOffsets()) {
		Warning("[%s] Unable to determine proper offsets!\n", PLUGIN_DESC);
		return false;
	}

	if (!Funcs::Load()) {
		Warning("[%s] Unable to initialize hooking!", PLUGIN_DESC);
		return false;
	}
	g_ModuleManager = new ModuleManager();

	if (!doPostScreenSpaceEffectsHook) {
		try {
			doPostScreenSpaceEffectsHook = Funcs::AddHook_IClientMode_DoPostScreenSpaceEffects(Interfaces::GetClientMode(), SH_STATIC(Hook_IClientMode_DoPostScreenSpaceEffects), false);
		}
		catch (bad_pointer &e) {
			Warning(e.what());
		}

		if (!doPostScreenSpaceEffectsHook && !frameHook) {
			frameHook = Funcs::AddHook_IBaseClientDLL_FrameStageNotify(Interfaces::pClientDLL, SH_STATIC(Hook_IBaseClientDLL_FrameStageNotify), true);
		}
	}
	
	ConVar_Register();

	g_AntiFreeze = new AntiFreeze();
	g_CameraTools = new CameraTools();
	g_CustomMaterials = new CustomMaterials();
	g_CustomModels = new CustomModels();
	g_CustomTextures = new CustomTextures();
	g_FOVOverride = new FOVOverride();
	g_Killstreaks = new Killstreaks();
	g_LoadoutIcons = new LoadoutIcons();
	g_LocalPlayer = new LocalPlayer();
	g_MedigunInfo = new MedigunInfo();
	g_MultiPanel = new MultiPanel();
	g_PlayerAliases = new PlayerAliases();
	g_PlayerModels = new PlayerModels();
	g_PlayerOutlines = new PlayerOutlines();
	g_ProjectileOutlines = new ProjectileOutlines();
	g_SpecGUIOrder = new SpecGUIOrder();
	g_StatusIcons = new StatusIcons();
	g_TeamHealthComparison = new TeamHealthComparison();
	g_TeamOverrides = new TeamOverrides();
	
	Msg("%s loaded!\n", PLUGIN_DESC);
	return true;
}

void StatusSpecPlugin::Unload(void)
{
	delete g_AntiFreeze;
	delete g_CameraTools;
	delete g_CustomMaterials;
	delete g_CustomModels;
	delete g_CustomTextures;
	delete g_FOVOverride;
	delete g_Killstreaks;
	delete g_LoadoutIcons;
	delete g_LocalPlayer;
	delete g_MedigunInfo;
	delete g_MultiPanel;
	delete g_PlayerAliases;
	delete g_PlayerModels;
	delete g_PlayerOutlines;
	delete g_ProjectileOutlines;
	delete g_SpecGUIOrder;
	delete g_StatusIcons;
	delete g_TeamHealthComparison;
	delete g_TeamOverrides;
	g_ModuleManager->UnloadAllModules();

	Funcs::Unload();

	ConVar_Unregister();
	Interfaces::Unload();
}

void StatusSpecPlugin::Pause(void) {
	Funcs::Pause();
}

void StatusSpecPlugin::UnPause(void) {
	Funcs::Unpause();
}

const char *StatusSpecPlugin::GetPluginDescription(void) {
	return PLUGIN_DESC;
}

void StatusSpecPlugin::LevelInit(char const *pMapName) {}
void StatusSpecPlugin::ServerActivate(edict_t *pEdictList, int edictCount, int clientMax) {}
void StatusSpecPlugin::GameFrame(bool simulating) {}
void StatusSpecPlugin::LevelShutdown(void) {}
void StatusSpecPlugin::ClientActive(edict_t *pEntity) {}
void StatusSpecPlugin::ClientDisconnect(edict_t *pEntity) {}
void StatusSpecPlugin::ClientPutInServer(edict_t *pEntity, char const *playername) {}
void StatusSpecPlugin::SetCommandClient(int index) {}
void StatusSpecPlugin::ClientSettingsChanged(edict_t *pEdict) {}
PLUGIN_RESULT StatusSpecPlugin::ClientConnect(bool *bAllowConnect, edict_t *pEntity, const char *pszName, const char *pszAddress, char *reject, int maxrejectlen) { return PLUGIN_CONTINUE; }
PLUGIN_RESULT StatusSpecPlugin::ClientCommand(edict_t *pEntity, const CCommand &args) { return PLUGIN_CONTINUE; }
PLUGIN_RESULT StatusSpecPlugin::NetworkIDValidated(const char *pszUserName, const char *pszNetworkID) { return PLUGIN_CONTINUE; }
void StatusSpecPlugin::OnQueryCvarValueFinished(QueryCvarCookie_t iCookie, edict_t *pPlayerEntity, EQueryCvarValueStatus eStatus, const char *pCvarName, const char *pCvarValue) {}
void StatusSpecPlugin::OnEdictAllocated(edict_t *edict) {}
void StatusSpecPlugin::OnEdictFreed(const edict_t *edict) {}