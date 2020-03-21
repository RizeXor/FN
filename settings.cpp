#include "settings.h"

SETTINGS settings = { 0 };

namespace Settings {
	bool Initialize() {
		settings.Aimbot = false;
		settings.FOV = true;
		settings.FOVSize = 250.0f;
		settings.PlayersAround = true;
		settings.Loop = true;
		settings.Decrypt = true;

		return true;
	}
}
