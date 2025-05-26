#include "auto_updater.h"


namespace updater {

	bool file_good(const std::string& name) {
		std::ifstream f(name.c_str());
		return f.good();
	}

	bool read() {
		if (!updater::file_good(file_path)) {
			save();
			return false;
		}

		std::ifstream f(file_path);

		json data;
		try {
			data = json::parse(f);
		}
		catch (const std::exception& e) {
			save();
		}

		if (data.empty())
			return false;

		if (data["build_number"].is_number())
			build_number = data["build_number"];

		if (data["dwLocalPlayer"].is_number())
			offsets::dwLocalPlayerController = data["dwLocalPlayer"];
		if (data["dwLocalPlayerController"].is_number())
			offsets::dwLocalPlayerController = data["dwLocalPlayerController"];
		if (data["dwEntityList"].is_number())
			offsets::dwEntityList = data["dwEntityList"];
		if (data["dwViewMatrix"].is_number())
			offsets::dwViewMatrix = data["dwViewMatrix"];
		if (data["dwBuildNumber"].is_number())
			offsets::dwBuildNumber = data["dwBuildNumber"];
		if (data["dwPlantedC4"].is_number())
			offsets::dwPlantedC4 = data["dwPlantedC4"];

		if (data["m_flC4Blow"].is_number())
			offsets::m_flC4Blow = data["m_flC4Blow"];
		if (data["m_flNextBeep"].is_number())
			offsets::m_flNextBeep = data["m_flNextBeep"];
		if (data["m_flTimerLength"].is_number())
			offsets::m_flTimerLength = data["m_flTimerLength"];

		if (data["m_pInGameMoneyServices"].is_number())
			offsets::m_pInGameMoneyServices = data["m_pInGameMoneyServices"];
		if (data["m_iAccount"].is_number())
			offsets::m_iAccount = data["m_iAccount"];
		if (data["m_vecAbsOrigin"].is_number())
			offsets::m_vecAbsOrigin = data["m_vecAbsOrigin"];
		if (data["m_vOldOrigin"].is_number())
			offsets::m_vOldOrigin = data["m_vOldOrigin"];
		if (data["m_pGameSceneNode"].is_number())
			offsets::m_pGameSceneNode = data["m_pGameSceneNode"];
		if (data["m_flFlashOverlayAlpha"].is_number())
			offsets::m_flFlashOverlayAlpha = data["m_flFlashOverlayAlpha"];
		if (data["m_bIsDefusing"].is_number())
			offsets::m_bIsDefusing = data["m_bIsDefusing"];
		if (data["m_szName"].is_number())
			offsets::m_szName = data["m_szName"];
		if (data["m_pClippingWeapon"].is_number())
			offsets::m_pClippingWeapon = data["m_pClippingWeapon"];
		if (data["m_ArmorValue"].is_number())
			offsets::m_ArmorValue = data["m_ArmorValue"];
		if (data["m_iHealth"].is_number())
			offsets::m_iHealth = data["m_iHealth"];
		if (data["m_hPlayerPawn"].is_number())
			offsets::m_hPlayerPawn = data["m_hPlayerPawn"];
		if (data["dwSanitizedName"].is_number())
			offsets::m_sSanitizedPlayerName = data["m_sSanitizedPlayerName"];
		if (data["m_iTeamNum"].is_number())
			offsets::m_iTeamNum = data["m_iTeamNum"];

		return true;
	}

	void save() {
		json data;

		data["build_number"] = build_number;

		data["dwLocalPlayerController"] = offsets::dwLocalPlayerController;
		data["dwEntityList"] = offsets::dwEntityList;
		data["dwViewMatrix"] = offsets::dwViewMatrix;
		data["dwBuildNumber"] = offsets::dwBuildNumber;
		data["dwPlantedC4"] = offsets::dwPlantedC4;

		data["m_flNextBeep"] = offsets::m_flNextBeep;
		data["m_flC4Blow"] = offsets::m_flC4Blow;
		data["m_flTimerLength"] = offsets::m_flTimerLength;

		data["m_pInGameMoneyServices"] = offsets::m_pInGameMoneyServices;
		data["m_iAccount"] = offsets::m_iAccount;
		data["m_vecAbsOrigin"] = offsets::m_vecAbsOrigin;
		data["m_vOldOrigin"] = offsets::m_vOldOrigin;
		data["m_pGameSceneNode"] = offsets::m_pGameSceneNode;
		data["m_flFlashOverlayAlpha"] = offsets::m_flFlashOverlayAlpha;
		data["m_bIsDefusing"] = offsets::m_bIsDefusing;
		data["m_szName"] = offsets::m_szName;
		data["m_pClippingWeapon"] = offsets::m_pClippingWeapon;
		data["m_ArmorValue"] = offsets::m_ArmorValue;
		data["m_iHealth"] = offsets::m_iHealth;
		data["m_hPlayerPawn"] = offsets::m_hPlayerPawn;
		data["m_sSanitizedPlayerName"] = offsets::m_sSanitizedPlayerName;
		data["m_iTeamNum"] = offsets::m_iTeamNum;

		std::ofstream output(file_path);
		output << std::setw(4) << data << std::endl;
		output.close();
	}
}