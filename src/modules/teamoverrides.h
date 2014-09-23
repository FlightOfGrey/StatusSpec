/*
*  teamoverrides.h
*  StatusSpec project
*
*  Copyright (c) 2014 thesupremecommander
*  BSD 2-Clause License
*  http://opensource.org/licenses/BSD-2-Clause
*
*/

#pragma once

#include "../stdafx.h"

#include <string>

#include "convar.h"
#include "igameevents.h"
#include "KeyValues.h"
#include "vgui/VGUI.h"

class TeamOverrides {
public:
	TeamOverrides();

	bool IsEnabled();

	void InterceptMessage(vgui::VPANEL vguiPanel, KeyValues *params, vgui::VPANEL ifromPanel);
private:
	ConVar *enabled;
	ConVar *name_blu;
	ConVar *name_red;
	ConVar *names;
	ConVar *score_blu;
	ConVar *score_red;
	ConVar *scores;
	ConCommand *switch_teams;
	static void SwitchTeams();
};

extern TeamOverrides *g_TeamOverrides;