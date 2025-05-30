#pragma once
#include <cstdint>

namespace offsets {
    extern int build_number;

    namespace client {
        extern uintptr_t dwLocalPlayerController;
        extern uintptr_t dwEntityList;
        extern uintptr_t dwViewMatrix;
        extern uintptr_t dwPlantedC4;
    }

    namespace engine {
        extern uintptr_t dwBuildNumber;
    }

    namespace netvars {
        extern uintptr_t m_flC4Blow;
        extern uintptr_t m_flNextBeep;
        extern uintptr_t m_flTimerLength;
        extern uintptr_t m_pInGameMoneyServices;
        extern uintptr_t m_iAccount;
        extern uintptr_t m_vecAbsOrigin;
        extern uintptr_t m_vOldOrigin;
        extern uintptr_t m_pGameSceneNode;
        extern uintptr_t m_flFlashOverlayAlpha;
        extern uintptr_t m_bIsDefusing;
        extern uintptr_t m_szName;
        extern uintptr_t m_pClippingWeapon;
        extern uintptr_t m_ArmorValue;
        extern uintptr_t m_iHealth;
        extern uintptr_t m_hPlayerPawn;
        extern uintptr_t m_sSanitizedPlayerName;
        extern uintptr_t m_iTeamNum;
    }

    bool FetchOffsets();
}