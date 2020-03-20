#pragma once

#include <stdio.h>
#include <windows.h>
#include <string>

#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_internal.h>

#include <MinHook.h>
#pragma comment(lib, "minhook.lib")

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

#include "xorstr.h"

#include "render.h"
#include "settings.h"
#include "offsets.h"
#include "utils.h"
