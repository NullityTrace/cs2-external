#pragma once
#include <Windows.h>
#include <WinINet.h>
#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include <vector>
#include <sstream>
#include <filesystem>
#include "json.h"

using json = nlohmann::json;
namespace fs = std::filesystem;


namespace updater {
	const std::string file_path = "offsets.json";

	inline int build_number = 14082;

	namespace offsets {
		inline std::ptrdiff_t dwLocalPlayerController = 27580480;
		inline std::ptrdiff_t dwEntityList = 27258424;
		inline std::ptrdiff_t dwViewMatrix = 27688864;
		inline std::ptrdiff_t dwBuildNumber = 5512164;
		inline std::ptrdiff_t dwPlantedC4 = 27709544;

		inline std::ptrdiff_t m_flC4Blow = 4032;
		inline std::ptrdiff_t m_flNextBeep = 4028;
		inline std::ptrdiff_t m_flTimerLength = 4040;

		inline std::ptrdiff_t m_pInGameMoneyServices = 1824;
		inline std::ptrdiff_t m_iAccount = 64;
		inline std::ptrdiff_t m_vecAbsOrigin = 208;
		inline std::ptrdiff_t m_vOldOrigin = 4900;
		inline std::ptrdiff_t m_pGameSceneNode = 808;
		inline std::ptrdiff_t m_flFlashOverlayAlpha = 5120;
		inline std::ptrdiff_t m_bIsDefusing = 9194;
		inline std::ptrdiff_t m_szName = 3360;
		inline std::ptrdiff_t m_pClippingWeapon = 5024;
		inline std::ptrdiff_t m_ArmorValue = 9244;
		inline std::ptrdiff_t m_iHealth = 836;
		inline std::ptrdiff_t m_hPlayerPawn = 2084;
		inline std::ptrdiff_t m_sSanitizedPlayerName = 1912;
		inline std::ptrdiff_t m_iTeamNum = 995;
	}
}