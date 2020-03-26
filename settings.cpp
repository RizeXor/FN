#include "settings.h"

SETTINGS settings = { 0 };

namespace Settings {
	bool Initialize() {
		settings.Aimbot = true;
		settings.FOV = true;
		settings.PlayersAround = true;
		settings.ESP.BoneESP = true;
		settings.ESP.BoxEsp = true;
		settings.ESP.PlayerLines = false;
		settings.MarkBots = true;

		settings.ESP.PlayerLineToBots = true;
		settings.ESP.PlayerLineToPlayers = true;
		
		settings.FOVSize = 250.0f;

		settings.NotVisibleColor[0] = 0.0f;
		settings.NotVisibleColor[1] = 0.0f;
		settings.NotVisibleColor[2] = 0.0f;
		settings.NotVisibleColor[3] = 0.65f;

		//0.66f, 0.58f, 0.76f, 1.0f
		settings.BotColor[0] = 0.66f;
		settings.BotColor[1] = 0.58f;
		settings.BotColor[2] = 0.76f;
		settings.BotColor[3] = 1.0f;

		return true;
	}
}
