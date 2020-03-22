#include "settings.h"

SETTINGS settings = { 0 };

namespace Settings {
	bool Initialize() {
		settings.Aimbot = true;
		settings.FOV = true;
		settings.FOVSize = 150.0f;
		settings.PlayersAround = true;
		settings.Loop = true;
		settings.Decrypt = true;
		settings.ESP.Players = true;
		settings.ESP.PlayerLines = true;

		return true;
	}
}
