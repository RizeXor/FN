#include "settings.h"

SETTINGS settings = { 0 };

namespace Settings {
	bool Initialize() {
		settings.Aimbot = false;
		settings.FOV = true;
		settings.FOVSize = 250.0f;

		return true;
	}
}
