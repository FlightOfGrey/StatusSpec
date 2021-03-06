/*
 *  statusspec.h
 *  StatusSpec project
 *  
 *  Copyright (c) 2014 thesupremecommander
 *  BSD 2-Clause License
 *  http://opensource.org/licenses/BSD-2-Clause
 *
 */

#pragma once

#include "stdafx.h"

#include "common.h"
#include "funcs.h"
#include "glows.h"
#include "ifaces.h"
#include "modules.h"

#include "modules/antifreeze.h"
#include "modules/cameratools.h"
#include "modules/custommaterials.h"
#include "modules/custommodels.h"
#include "modules/customtextures.h"
#include "modules/fovoverride.h"
#include "modules/killstreaks.h"
#include "modules/loadouticons.h"
#include "modules/localplayer.h"
#include "modules/mediguninfo.h"
#include "modules/multipanel.h"
#include "modules/playeraliases.h"
#include "modules/playermodels.h"
#include "modules/playeroutlines.h"
#include "modules/projectileoutlines.h"
#include "modules/specguiorder.h"
#include "modules/statusicons.h"
#include "modules/teamhealthcomparison.h"
#include "modules/teamoverrides.h"

class StatusSpecPlugin: public IServerPluginCallbacks
{
public:
	StatusSpecPlugin();
	~StatusSpecPlugin();

	// IServerPluginCallbacks methods
	virtual bool			Load(CreateInterfaceFn interfaceFactory, CreateInterfaceFn gameServerFactory);
	virtual void			Unload(void);
	virtual void			Pause(void);
	virtual void			UnPause(void);
	virtual const char		*GetPluginDescription(void);	  
	virtual void			LevelInit(char const *pMapName);
	virtual void			ServerActivate(edict_t *pEdictList, int edictCount, int clientMax);
	virtual void			GameFrame(bool simulating);
	virtual void			LevelShutdown(void);
	virtual void			ClientActive(edict_t *pEntity);
	virtual void			ClientDisconnect(edict_t *pEntity);
	virtual void			ClientPutInServer(edict_t *pEntity, char const *playername);
	virtual void			SetCommandClient(int index);
	virtual void			ClientSettingsChanged(edict_t *pEdict);
	virtual PLUGIN_RESULT	ClientConnect(bool *bAllowConnect, edict_t *pEntity, const char *pszName, const char *pszAddress, char *reject, int maxrejectlen);
	virtual PLUGIN_RESULT	ClientCommand(edict_t *pEntity, const CCommand &args);
	virtual PLUGIN_RESULT	NetworkIDValidated(const char *pszUserName, const char *pszNetworkID);
	virtual void			OnQueryCvarValueFinished(QueryCvarCookie_t iCookie, edict_t *pPlayerEntity, EQueryCvarValueStatus eStatus, const char *pCvarName, const char *pCvarValue);
	virtual void			OnEdictAllocated(edict_t *edict);
	virtual void			OnEdictFreed(const edict_t *edict);
};