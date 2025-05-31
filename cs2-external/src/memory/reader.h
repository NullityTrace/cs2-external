#pragma once
#include <memory>
#include "HandleInspector.h"
#include "pProcess.h"
#include "vector.h"
#include <mutex>

struct view_matrix_t {
	float* operator[ ](int index) {
		return matrix[index];
	}

	float matrix[4][4];
};

class Bones {
public:
	std::map<std::string, Vector3> bonePositions;
};

class Player {
public:
	uintptr_t entity;
	int team;
	uintptr_t pCSPlayerPawn;
	uintptr_t gameSceneNode;
	uintptr_t boneArray;
	uintptr_t spottedState;
	int health;
	int armor;
	std::string name;
	Vector3 origin;
	Vector3 head;
	Bones bones;
	bool is_defusing;
	bool is_spotted;
	int32_t money;
	float flashAlpha;
	std::string weapon;
	void ReadBones();
	void ReadHead();
};


class Reader {
public:
    std::shared_ptr<pProcess> process;
    ProcessModule base_client;
    ProcessModule base_engine;
	bool inGame;
	int localTeam;
	Vector3 localOrigin;
	std::vector<Player> players = {};
	RECT game_bounds;
	Vector3 world_to_screen(Vector3* v);
    void init();
    void loop();

private:
	view_matrix_t view_matrix;
	uintptr_t entity_list;
	uintptr_t localPlayer;
	uintptr_t localpCSPlayerPawn;
	std::uint32_t localPlayerPawn;
	uintptr_t localList_entry2;

};

inline Reader reader;
inline std::mutex reader_mutex;