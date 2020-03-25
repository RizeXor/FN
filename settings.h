#pragma once

struct SETTINGS
{
	bool Aimbot;
	bool Debug;
	bool FOV;
	bool PlayersAround;
	float FOVSize;
	struct {
		bool BoneESP;
		bool BOXESP;
		bool PlayerLines;
	} ESP;
	float NotVisibleColor[4];
	float BotColor[4];
};

extern SETTINGS settings;

namespace Settings {
	bool Initialize();
}
