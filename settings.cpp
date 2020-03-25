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

		return true;
	}
}
