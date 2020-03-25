#include "settings.h"

SETTINGS settings = { 0 };

namespace Settings {
	bool Initialize() {
		settings.Aimbot = true;
		settings.FOV = true;
		settings.FOVSize = 150.0f;
		settings.PlayersAround = true;
		settings.ESP.BoneESP = true;
		settings.ESP.BOXESP = true;
		settings.ESP.PlayerLines = false;
		settings.NotVisibleColor[0] = 0.0f;
		settings.NotVisibleColor[1] = 0.0f;
		settings.NotVisibleColor[2] = 0.0f;
		settings.NotVisibleColor[3] = 0.80f;

		settings.BotColor[0] = 0.0f;
		settings.BotColor[1] = 0.0f;
		settings.BotColor[2] = 1.0f;
		settings.BotColor[3] = 1.0f;

		return true;
	}
}
