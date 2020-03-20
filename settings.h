#pragma once

struct SETTINGS
{
	bool Aimbot;
	bool FOV;
	float FOVSize;
	struct {
		bool Players;
	} ESP;
};

extern SETTINGS settings;

namespace Settings {
	bool Initialize();
}
