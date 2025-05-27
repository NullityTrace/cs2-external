// offsets.h
#pragma once
#include <cstdint>

namespace offsets {
    extern int build_number;

    namespace client {
        extern std::uintptr_t dwLocalPlayerController;
        extern std::uintptr_t dwEntityList;
        extern std::uintptr_t dwViewMatrix;
        extern std::uintptr_t dwPlantedC4;
    }

    namespace engine {
        extern std::uintptr_t dwBuildNumber;
    }

    namespace netvars {
        extern std::uintptr_t m_flC4Blow;
        extern std::uintptr_t m_flNextBeep;
        extern std::uintptr_t m_flTimerLength;
        extern std::uintptr_t m_pInGameMoneyServices;
        extern std::uintptr_t m_iAccount;
        extern std::uintptr_t m_vecAbsOrigin;
        extern std::uintptr_t m_vOldOrigin;
        extern std::uintptr_t m_pGameSceneNode;
        extern std::uintptr_t m_flFlashOverlayAlpha;
        extern std::uintptr_t m_bIsDefusing;
        extern std::uintptr_t m_szName;
        extern std::uintptr_t m_pClippingWeapon;
        extern std::uintptr_t m_ArmorValue;
        extern std::uintptr_t m_iHealth;
        extern std::uintptr_t m_hPlayerPawn;
        extern std::uintptr_t m_sSanitizedPlayerName;
        extern std::uintptr_t m_iTeamNum;
    }

    bool FetchOffsets();
}