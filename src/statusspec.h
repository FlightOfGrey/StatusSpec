/*
 *  statusspec.h
 *  StatusSpec project
 *  
 *  Copyright (c) 2013 thesupremecommander
 *  BSD 2-Clause License
 *  http://opensource.org/licenses/BSD-2-Clause
 *
 */

#ifndef STATUSSPEC_H
#define STATUSSPEC_H

#define CLIENT_DLL

#include "stdafx.h"

#include "tier1/tier1.h"
#include "engine/iserverplugin.h"
#include "igameevents.h"

#include "offsets.h"
#include "vfuncs.h"
#include "icvar.h"

#define PLUGIN_DESC "StatusSpec v0.2"
#define SHORT_DESC "StatusSpec v0.2"

#define SCALED(normalValue) pScheme->GetProportionalScaledValue(normalValue)

#define TEAM_RED 2
#define TEAM_BLU 3

// taken from SourceMod and adapted

enum TFCond
{
	TFCond_Slowed,
	TFCond_Zoomed,
	TFCond_Disguising,
	TFCond_Disguised,
	TFCond_Cloaked,
	TFCond_Ubercharged,
	TFCond_TeleportedGlow,
	TFCond_Taunting,
	TFCond_UberchargeFading,
	TFCond_CloakFlicker,
	TFCond_Teleporting,
	TFCond_Kritzkrieged,
	TFCond_TmpDamageBonus,
	TFCond_DeadRingered,
	TFCond_Bonked,
	TFCond_Dazed,
	TFCond_Buffed,
	TFCond_Charging,
	TFCond_DemoBuff,
	TFCond_CritCola,
	TFCond_InHealRadius,
	TFCond_Healing,
	TFCond_OnFire,
	TFCond_Overhealed,
	TFCond_Jarated,
	TFCond_Bleeding,
	TFCond_DefenseBuffed,
	TFCond_Milked,
	TFCond_MegaHeal,
	TFCond_RegenBuffed,
	TFCond_MarkedForDeath,
	TFCond_NoHealingDamageBuff,
	TFCond_SpeedBuffAlly,
	TFCond_HalloweenCritCandy,
	TFCond_CritCanteen,
	TFCond_CritDemoCharge,
	TFCond_CritHype,
	TFCond_CritOnFirstBlood,
	TFCond_CritOnWin,
	TFCond_CritOnFlagCapture,
	TFCond_CritOnKill,
	TFCond_RestrictToMelee,
	TFCond_DefenseBuffNoCritBlock,
	TFCond_Reprogrammed,
	TFCond_CritMmmph,
	TFCond_DefenseBuffMmmph,
	TFCond_FocusBuff,
	TFCond_DisguiseRemoved,
	TFCond_MarkedForDeathSilent,
	TFCond_DisguisedAsDispenser,
	TFCond_Sapped,
	TFCond_UberchargedHidden,
	TFCond_UberchargedCanteen,
	TFCond_HalloweenBombHead,
	TFCond_HalloweenThriller,
	TFCond_RadiusHealOnDamage,
	TFCond_CritOnDamage,
	TFCond_UberchargedOnTakeDamage,
	TFCond_UberBulletResist,
	TFCond_UberBlastResist,
	TFCond_UberFireResist,
	TFCond_SmallBulletResist,
	TFCond_SmallBlastResist,
	TFCond_SmallFireResist,
	TFCond_Stealthed,
	TFCond_MedigunDebuff,
	TFCond_StealthedUserBuffFade,
	TFCond_BulletImmune,
	TFCond_BlastImmune,
	TFCond_FireImmune,
	TFCond_PreventDeath,
	TFCond_MVMBotRadiowave,
	TFCond_HalloweenSpeedBoost,
	TFCond_HalloweenQuickHeal,
	TFCond_HalloweenGiant,
	TFCond_HalloweenTiny,
	TFCond_HalloweenInHell,
	TFCond_HalloweenGhostMode
};

#define TEXTURE_UBERCHARGEICON "vgui/replay/thumbnails/ubercharge"
#define TEXTURE_CRITBOOSTICON "vgui/replay/thumbnails/critboost"
#define TEXTURE_RESISTSHIELDRED "vgui/replay/thumbnails/resist_shield"
#define TEXTURE_RESISTSHIELDBLU "vgui/replay/thumbnails/resist_shield_blue"
#define TEXTURE_BULLETRESISTRED "vgui/replay/thumbnails/defense_buff_bullet_red"
#define TEXTURE_BLASTRESISTRED "vgui/replay/thumbnails/defense_buff_explosion_red"
#define TEXTURE_FIRERESISTRED "vgui/replay/thumbnails/defense_buff_fire_red"
#define TEXTURE_BULLETRESISTBLU "vgui/replay/thumbnails/defense_buff_bullet_blue"
#define TEXTURE_BLASTRESISTBLU "vgui/replay/thumbnails/defense_buff_explosion_blue"
#define TEXTURE_FIRERESISTBLU "vgui/replay/thumbnails/defense_buff_fire_blue"
#define TEXTURE_BUFFBANNERRED "effects/soldier_buff_offense_red"
#define TEXTURE_BUFFBANNERBLU "effects/soldier_buff_offense_blue"
#define TEXTURE_BATTALIONSBACKUPRED "effects/soldier_buff_defense_red"
#define TEXTURE_BATTALIONSBACKUPBLU "effects/soldier_buff_defense_blue"
#define TEXTURE_CONCHERORRED "effects/soldier_buff_healonhit_red"
#define TEXTURE_CONCHERORBLU "effects/soldier_buff_healonhit_blue"
#define TEXTURE_JARATE "vgui/replay/thumbnails/jarated"
#define TEXTURE_MADMILK "vgui/bleed_drop"
#define TEXTURE_MARKEDFORDEATH "vgui/marked_for_death"
#define TEXTURE_BLEEDING "vgui/bleed_drop"
#define TEXTURE_ONFIRE "hud/leaderboard_class_pyro"

#include "cbase.h"
#include "cdll_int.h"
#include "icliententitylist.h"
#include "icliententity.h"
#include "convar.h"
#include "edict.h"
#include "igameresources.h"
#include "igameevents.h"
#include "vgui/IPanel.h"
#include "vgui/ISurface.h"
#include "vgui/IScheme.h"
#include "ehandle.h"

#include "stdint.h"
#include "vector"

//class CBaseCombatCharacter;
class C_BaseCombatWeapon;
typedef unsigned int VPANEL;
typedef CHandle<C_BaseEntity> EHANDLE;

// client.dll
IBaseClientDLL* pClient;
IClientEntityList* pEntityList;

// engine.dll
IVEngineClient *pEngineClient;

// vgui2.dll
vgui::IPanel* pPanel;
vgui::ISurface* pSurface;
vgui::ISchemeManager* pScheme;
vgui::HFont m_font;

int m_iTextureUbercharged;
int m_iTextureCritBoosted;
int m_iTextureResistShieldRed;
int m_iTextureResistShieldBlu;
int m_iTextureBulletResistRed;
int m_iTextureBlastResistRed;
int m_iTextureFireResistRed;
int m_iTextureBulletResistBlu;
int m_iTextureBlastResistBlu;
int m_iTextureFireResistBlu;
int m_iTextureBuffBannerRed;
int m_iTextureBuffBannerBlu;
int m_iTextureBattalionsBackupRed;
int m_iTextureBattalionsBackupBlu;
int m_iTextureConcherorRed;
int m_iTextureConcherorBlu;
int m_iTextureJarated;
int m_iTextureMilked;
int m_iTextureMarkedForDeath;
int m_iTextureBleeding;
int m_iTextureOnFire;

typedef struct Player {
	int team;
    uint32_t conditions[3];
} Player;

std::vector<Player> playerInfo;

#define Round(x) ((x)>=0?(int)((x)+0.5):(int)((x)-0.5))

class StatusSpecPlugin: public IServerPluginCallbacks, public IGameEventListener
{
public:
	StatusSpecPlugin();
	~StatusSpecPlugin();

	// IServerPluginCallbacks methods
	virtual bool			Load(	CreateInterfaceFn interfaceFactory, CreateInterfaceFn gameServerFactory );
	virtual void			Unload( void );
	virtual void			Pause( void );
	virtual void			UnPause( void );
	virtual const char     *GetPluginDescription( void );      
	virtual void			LevelInit( char const *pMapName );
	virtual void			ServerActivate( edict_t *pEdictList, int edictCount, int clientMax );
	virtual void			GameFrame( bool simulating );
	virtual void			LevelShutdown( void );
	virtual void			ClientActive( edict_t *pEntity );
	virtual void			ClientDisconnect( edict_t *pEntity );
	virtual void			ClientPutInServer( edict_t *pEntity, char const *playername );
	virtual void			SetCommandClient( int index );
	virtual void			ClientSettingsChanged( edict_t *pEdict );
	virtual PLUGIN_RESULT	ClientConnect( bool *bAllowConnect, edict_t *pEntity, const char *pszName, const char *pszAddress, char *reject, int maxrejectlen );
	virtual PLUGIN_RESULT	ClientCommand( edict_t *pEntity, const CCommand &args );
	virtual PLUGIN_RESULT	NetworkIDValidated( const char *pszUserName, const char *pszNetworkID );
	virtual void			OnQueryCvarValueFinished( QueryCvarCookie_t iCookie, edict_t *pPlayerEntity, EQueryCvarValueStatus eStatus, const char *pCvarName, const char *pCvarValue );
	virtual void			OnEdictAllocated( edict_t *edict );
	virtual void			OnEdictFreed( const edict_t *edict );

	// IGameEventListener Interface
	virtual void FireGameEvent( KeyValues * event );
};

#endif
