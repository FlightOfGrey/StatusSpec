/*
*  multipanel.cpp
*  StatusSpec project
*
*  Copyright (c) 2014 thesupremecommander
*  BSD 2-Clause License
*  http://opensource.org/licenses/BSD-2-Clause
*
*/

#include "multipanel.h"

MultiPanel::MultiPanel() {
	consoleDialog = nullptr;
	scoreboardPanel = vgui::INVALID_PANEL;

	console = new ConVar("statusspec_multipanel_console", "0", FCVAR_NONE, "displays a console in the HUD", MultiPanel::ToggleConsole);
	scoreboard = new ConVar("statusspec_multipanel_scoreboard", "0", FCVAR_NONE, "displays the scoreboard", MultiPanel::ToggleScoreboard);
}

void MultiPanel::InitHud() {
	if (!consoleDialog) {
		consoleDialog = new CGameConsoleDialog();
	}

	if (scoreboardPanel == vgui::INVALID_PANEL) {
		IClientMode *clientMode = Interfaces::GetClientMode();

		if (clientMode) {
			vgui::Panel *viewport = Interfaces::GetClientMode()->GetViewport();

			if (viewport) {
				vgui::VPANEL vpanel = viewport->GetVPanel();
				int children = g_pVGuiPanel->GetChildCount(vpanel);

				for (int i = 0; i < children; i++) {
					vgui::VPANEL child = g_pVGuiPanel->GetChild(vpanel, i);

					if (strcmp(g_pVGuiPanel->GetName(child), SCOREBOARD_PANEL_NAME) == 0) {
						scoreboardPanel = g_pVGui->PanelToHandle(child);

						break;
					}
				}
			}
		}
	}
}

void MultiPanel::ToggleConsole(IConVar *var, const char *pOldValue, float flOldValue) {
	bool enabled = g_MultiPanel->console->GetBool();

	if (!g_MultiPanel->consoleDialog) {
		g_MultiPanel->InitHud();
	}

	if (g_MultiPanel->consoleDialog) {
		if (enabled) {
			g_MultiPanel->consoleDialog->Activate();
		}
		else {
			g_MultiPanel->consoleDialog->Hide();
		}
	}
}

void MultiPanel::ToggleScoreboard(IConVar *var, const char *pOldValue, float flOldValue) {
	if (g_MultiPanel->scoreboardPanel == vgui::INVALID_PANEL) {
		g_MultiPanel->InitHud();
	}

	if (g_MultiPanel->scoreboardPanel != vgui::INVALID_PANEL) {
		IClientMode *clientMode = Interfaces::GetClientMode();

		if (clientMode) {
			IViewPort *viewport = dynamic_cast<IViewPort *>(Interfaces::GetClientMode()->GetViewport());

			if (viewport) {
				viewport->ShowPanel(SCOREBOARD_PANEL_NAME, true);
				viewport->ShowPanel(SCOREBOARD_PANEL_NAME, false);
			}
		}

		g_pVGuiPanel->SetVisible(g_pVGui->HandleToPanel(g_MultiPanel->scoreboardPanel), g_MultiPanel->scoreboard->GetBool());
	}
}