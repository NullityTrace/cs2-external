#include <iostream>
#include <windows.h>
#include "src/overlay/overlay.h"
#include "src/updater/offsets.h"
#include "src/memory/reader.h"



bool finish = false;
void read_thread() {
    while (!finish) {
        reader.loop();
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }
}


int main() {
    bool updated = offsets::FetchOffsets();

    if (updated) {
        std::cout << "Offsets updated successfully!" << std::endl;
    }
    else {
        std::cout << "Using existing offsets" << std::endl;
    }

    std::cout << "\n===== OFFSETS =====" << std::hex << std::uppercase << std::endl;
    std::cout << "Build Number: 0x" << offsets::build_number << std::endl;

    std::cout << "\nClient Offsets:" << std::endl;
    std::cout << "  dwLocalPlayerController: 0x" << offsets::client::dwLocalPlayerController << std::endl;
    std::cout << "  dwEntityList: 0x" << offsets::client::dwEntityList << std::endl;
    std::cout << "  dwViewMatrix: 0x" << offsets::client::dwViewMatrix << std::endl;
    std::cout << "  dwPlantedC4: 0x" << offsets::client::dwPlantedC4 << std::endl;

    std::cout << "\nEngine Offsets:" << std::endl;
    std::cout << "  dwBuildNumber: 0x" << offsets::engine::dwBuildNumber << std::endl;

    std::cout << "\nNetvar Offsets:" << std::endl;
    std::cout << "  m_flC4Blow: 0x" << offsets::netvars::m_flC4Blow << std::endl;
    std::cout << "  m_flNextBeep: 0x" << offsets::netvars::m_flNextBeep << std::endl;
    std::cout << "  m_flTimerLength: 0x" << offsets::netvars::m_flTimerLength << std::endl;
    std::cout << "  m_pInGameMoneyServices: 0x" << offsets::netvars::m_pInGameMoneyServices << std::endl;
    std::cout << "  m_iAccount: 0x" << offsets::netvars::m_iAccount << std::endl;
    std::cout << "  m_vecAbsOrigin: 0x" << offsets::netvars::m_vecAbsOrigin << std::endl;
    std::cout << "  m_vOldOrigin: 0x" << offsets::netvars::m_vOldOrigin << std::endl;
    std::cout << "  m_pGameSceneNode: 0x" << offsets::netvars::m_pGameSceneNode << std::endl;
    std::cout << "  m_flFlashOverlayAlpha: 0x" << offsets::netvars::m_flFlashOverlayAlpha << std::endl;
    std::cout << "  m_bIsDefusing: 0x" << offsets::netvars::m_bIsDefusing << std::endl;
    std::cout << "  m_szName: 0x" << offsets::netvars::m_szName << std::endl;
    std::cout << "  m_pClippingWeapon: 0x" << offsets::netvars::m_pClippingWeapon << std::endl;
    std::cout << "  m_ArmorValue: 0x" << offsets::netvars::m_ArmorValue << std::endl;
    std::cout << "  m_iHealth: 0x" << offsets::netvars::m_iHealth << std::endl;
    std::cout << "  m_hPlayerPawn: 0x" << offsets::netvars::m_hPlayerPawn << std::endl;
    std::cout << "  m_sSanitizedPlayerName: 0x" << offsets::netvars::m_sSanitizedPlayerName << std::endl;
    std::cout << "  m_iTeamNum: 0x" << offsets::netvars::m_iTeamNum << std::endl;

    reader.init();

    std::thread read(read_thread);

    read.detach();

    overlay::render();
    return 0;
}