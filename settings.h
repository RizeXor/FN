#pragma once

struct SETTINGS
{
	bool Aimbot;
	bool Debug;
	bool FOV;
	bool PlayersAround;
	bool Loop;
	bool Decrypt;
	float FOVSize;
	struct {
		bool Players;
	} ESP;
};

extern SETTINGS settings;

namespace Settings {
	bool Initialize();
}
