#include <thread>
#include <map>
#include <cmath>
#include "../updater/offsets.h"
#include "reader.h"
#include "../overlay/overlay.h"

void Reader::init() {
    HandleInspector inspector;
    if (inspector.FindHandlesToProcess(L"cs2.exe")) {
        HANDLE hCs2 = inspector.GetDuplicatedHandle();
        DWORD pid = inspector.GetDuplicatedPid();

        if (hCs2 && pid) {
            std::wcout << L"[cs2] Handle duplicado com sucesso! Handle: " << hCs2 << std::endl;

            process = std::make_shared<pProcess>(pid, hCs2);

            do {
                base_client = process->GetModule("client.dll");
                base_engine = process->GetModule("engine2.dll");

                if (base_client.base == 0 || base_engine.base == 0) {
                    std::cout << "[cs2] Aguardando client.dll/engine2.dll serem carregadas..." << std::endl;
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                }
            } while (base_client.base == 0 || base_engine.base == 0);

            std::cout << "[cs2] base_engine.base: 0x" << std::hex << base_engine.base << std::endl;
            std::cout << "[cs2] dwBuildNumber offset: 0x" << std::hex << offsets::engine::dwBuildNumber << std::endl;

            uintptr_t address = base_engine.base + offsets::engine::dwBuildNumber;
            std::cout << "[cs2] Final read address: 0x" << std::hex << address << std::endl;

            uintptr_t buildNum = process->read<uintptr_t>(address);
            std::cout << "[cs2] BuildNumber: " << std::dec << buildNum << std::endl;
        }
    }
}

std::string Reader::readString(uintptr_t address, size_t maxSize) {
	char buffer[128]{};

	if (address && address > 0x10000 && address < 0x7FFFFFFFFFFF) {
		if (process->read_raw(address, buffer, maxSize)) {
			return std::string(buffer);
		}
	}
	return {};
}



void Reader::loop() {
    std::lock_guard<std::mutex> lock(reader_mutex);
    inGame = false;

    localPlayer = process->read<uintptr_t>(base_client.base + offsets::client::dwLocalPlayerController);
    if (!localPlayer) return;

    localPlayerPawn = process->read<std::uint32_t>(localPlayer + offsets::netvars::m_hPlayerPawn);
    if (!localPlayerPawn) return;

    entity_list = process->read<uintptr_t>(base_client.base + offsets::client::dwEntityList);

    localList_entry2 = process->read<uintptr_t>(entity_list + 0x8 * ((localPlayerPawn & 0x7FFF) >> 9) + 16);
    localpCSPlayerPawn = process->read<uintptr_t>(localList_entry2 + 120 * (localPlayerPawn & 0x1FF));
    if (!localpCSPlayerPawn) return;

    view_matrix = process->read<view_matrix_t>(base_client.base + offsets::client::dwViewMatrix);

    localTeam = process->read<int>(localPlayer + offsets::netvars::m_iTeamNum);
    localOrigin = process->read<Vector3>(localpCSPlayerPawn + offsets::netvars::m_vOldOrigin);

    inGame = true;
    int playerIndex = 0;
    std::vector<Player> list;
    Player player;
	uintptr_t list_entry, list_entry2, playerPawn, playerMoneyServices, clippingWeapon, weaponData, playerNameData;

	while (true) {
		playerIndex++;
		list_entry = process->read<uintptr_t>(entity_list + (8 * (playerIndex & 0x7FFF) >> 9) + 16);
		if (!list_entry) break;
		
		player.entity = process->read<uintptr_t>(list_entry + 120 * (playerIndex & 0x1FF));
		if (!player.entity) continue;
		/*
		player.team = process->read<int>(player.entity + offsets::netvars::m_iTeamNum);
		if (overlay::teamEsp && (player.team == localTeam)) continue;

		playerPawn = process->read<std::uint32_t>(player.entity + offsets::netvars::m_hPlayerPawn);

		list_entry2 = process->read<uintptr_t>(entity_list + 0x8 * ((playerPawn & 0x7FFF) >> 9) + 16);
		if (!list_entry2) continue;

		player.pCSPlayerPawn = process->read<uintptr_t>(list_entry2 + 120 * (playerPawn & 0x1FF));
		if (!player.pCSPlayerPawn) continue;

		player.health = process->read<int>(player.pCSPlayerPawn + offsets::netvars::m_iHealth);
		player.armor = process->read<int>(player.pCSPlayerPawn + offsets::netvars::m_ArmorValue);
		if (player.health <= 0 || player.health > 100) continue;

		if (overlay::teamEsp && (player.pCSPlayerPawn == localPlayer)) continue;


		playerNameData = process->read<uintptr_t>(player.entity + offsets::netvars::m_sSanitizedPlayerName);
		player.name = readString(playerNameData);

		player.gameSceneNode = process->read<uintptr_t>(player.pCSPlayerPawn + offsets::netvars::m_pGameSceneNode);
		player.origin = process->read<Vector3>(player.pCSPlayerPawn + offsets::netvars::m_vOldOrigin);
		player.head = { player.origin.x, player.origin.y, player.origin.z + 75.f };

		if (player.origin.x == localOrigin.x && player.origin.y == localOrigin.y && player.origin.z == localOrigin.z)
			continue;

		if (overlay::render_distance != -1 && (localOrigin - player.origin).length2d() > overlay::render_distance) continue;
		if (player.origin.x == 0 && player.origin.y == 0) continue;

		if (overlay::skeletonEsp) {
			player.gameSceneNode = process->read<uintptr_t>(player.pCSPlayerPawn + offsets::netvars::m_pGameSceneNode);
			player.boneArray = process->read<uintptr_t>(player.gameSceneNode + 0x1F0);
			player.ReadBones();
		}

		if (overlay::dotEsp && !overlay::skeletonEsp) {
			player.gameSceneNode = process->read<uintptr_t>(player.pCSPlayerPawn + offsets::netvars::m_pGameSceneNode);
			player.boneArray = process->read<uintptr_t>(player.gameSceneNode + 0x1F0);
			player.ReadHead();
		}
		*/
		list.push_back(player);
	}

	players.clear();
	players.assign(list.begin(), list.end());
}


std::map<std::string, int> boneMap = {
	{"head", 6},
	{"neck_0", 5},
	{"spine_1", 4},
	{"spine_2", 2},
	{"pelvis", 0},
	{"arm_upper_L", 8},
	{"arm_lower_L", 9},
	{"hand_L", 10},
	{"arm_upper_R", 13},
	{"arm_lower_R", 14},
	{"hand_R", 15},
	{"leg_upper_L", 22},
	{"leg_lower_L", 23},
	{"ankle_L", 24},
	{"leg_upper_R", 25},
	{"leg_lower_R", 26},
	{"ankle_R", 27}
};

uintptr_t boneAddress;
Vector3 bonePosition;
int boneIndex;
void Player::ReadHead() {
	boneAddress = boneArray + 6 * 32;
	bonePosition = reader.process->read<Vector3>(boneAddress);
	bones.bonePositions["head"] = reader.world_to_screen(&bonePosition);

}

void Player::ReadBones() {
	for (const auto& entry : boneMap) {
		const std::string& boneName = entry.first;
		boneIndex = entry.second;
		boneAddress = boneArray + boneIndex * 32;
		bonePosition = reader.process->read<Vector3>(boneAddress);
		bones.bonePositions[boneName] = reader.world_to_screen(&bonePosition);
	}
}

Vector3 Reader::world_to_screen(Vector3* v) {
	float _x = view_matrix[0][0] * v->x + view_matrix[0][1] * v->y + view_matrix[0][2] * v->z + view_matrix[0][3];
	float _y = view_matrix[1][0] * v->x + view_matrix[1][1] * v->y + view_matrix[1][2] * v->z + view_matrix[1][3];

	float w = view_matrix[3][0] * v->x + view_matrix[3][1] * v->y + view_matrix[3][2] * v->z + view_matrix[3][3];

	float inv_w = 1.f / w;
	_x *= inv_w;
	_y *= inv_w;

	float x = game_bounds.right * .5f;
	float y = game_bounds.bottom * .5f;

	x += 0.5f * _x * game_bounds.right + 0.5f;
	y -= 0.5f * _y * game_bounds.bottom + 0.5f;

	return { x, y, w };
}


ProcessModule pProcess::GetModule(const char* lModule)
{
    std::wstring wideModule;
    int wideCharLength = MultiByteToWideChar(CP_UTF8, 0, lModule, -1, nullptr, 0);
    if (wideCharLength > 0)
    {
        wideModule.resize(wideCharLength);
        MultiByteToWideChar(CP_UTF8, 0, lModule, -1, &wideModule[0], wideCharLength);
    }

    HANDLE handle_module = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid_);
    MODULEENTRY32W module_entry_{};
    module_entry_.dwSize = sizeof(MODULEENTRY32W);

    if (Module32FirstW(handle_module, &module_entry_)) {
        do {
            if (!wcscmp(module_entry_.szModule, wideModule.c_str())) {
                CloseHandle(handle_module);
                return { (uintptr_t)module_entry_.modBaseAddr, module_entry_.dwSize };
            }
        } while (Module32NextW(handle_module, &module_entry_));
    }

    CloseHandle(handle_module);
    return { 0, 0 };
}
