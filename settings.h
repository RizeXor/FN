#pragma once

struct SETTINGS
{
	bool Aimbot;
	bool Debug;
	bool FOV;
	bool PlayersAround;
	bool MarkBots;
	float FOVSize;
	struct {
		bool BoneESP;
		bool BoxEsp;
		bool PlayerLines;
		bool PlayerLineToBots;
		bool PlayerLineToPlayers;
	} ESP;
	float NotVisibleColor[4];
	float BotColor[4];
};

extern SETTINGS settings;

namespace Settings {
	bool Initialize();
}
