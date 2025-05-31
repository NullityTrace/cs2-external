#include <thread>
#include <cmath>
#include "../memory/reader.h"
#include "../memory/pProcess.h"
#include "../gui/gui.h"
#include "../../include/imgui/imgui.h"
#include "../../include/imgui/imgui_internal.h"
#include "overlay.h"

namespace hack {
	std::vector<std::pair<std::string, std::string>> boneConnections = {
						{"neck_0", "spine_1"},
						{"spine_1", "spine_2"},
						{"spine_2", "pelvis"},
						{"spine_1", "arm_upper_L"},
						{"arm_upper_L", "arm_lower_L"},
						{"arm_lower_L", "hand_L"},
						{"spine_1", "arm_upper_R"},
						{"arm_upper_R", "arm_lower_R"},
						{"arm_lower_R", "hand_R"},
						{"pelvis", "leg_upper_L"},
						{"leg_upper_L", "leg_lower_L"},
						{"leg_lower_L", "ankle_L"},
						{"pelvis", "leg_upper_R"},
						{"leg_upper_R", "leg_lower_R"},
						{"leg_lower_R", "ankle_R"}
	};

	bool is_in_bounds(const Vector3& pos, int width, int heigh) {
		return pos.x >= 0 && pos.x <= width && pos.y >= 0 && pos.y <= heigh;

	}
	void loop() {

		std::lock_guard<std::mutex> lock(reader_mutex);

		int playerIndex = 0;
		uintptr_t list_entry;

		for (auto player = reader.players.begin(); player < reader.players.end(); player++) {
			const Vector3 screenPos = reader.world_to_screen(&player->origin);
			const Vector3 screenHead = reader.world_to_screen(&player->head);

			if (
				screenPos.z < 0.01f ||
				!is_in_bounds(screenPos, reader.game_bounds.right, reader.game_bounds.bottom)
				)
				continue;

			const float height = screenPos.y - screenHead.y;
			const float width = height / 2.4f;

			float distance = reader.localOrigin.calculate_distance(player->origin);
			int roundedDistance = std::round(distance / 10.f);
			if (overlay::dotEsp) {
				gui::DrawCircle(
					player->bones.bonePositions["head"].x,
					player->bones.bonePositions["head"].y - width / 12,
					width / 5,
					IM_COL32(225, 75, 75, 255)
				);
			}


			if (roundedDistance > overlay::flag_render_distance)
				continue;


		}
		// std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	
}

