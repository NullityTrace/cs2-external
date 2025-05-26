
#include <thread>
#include <cmath>
#include "reader.h"
#include "../gui/gui.h"
#include "../helpers/globals.h"
#include "../Include/imgui/imgui.h"
#include "../Include/imgui/imgui_internal.h"
#include "../overlay/overlay.h"
#include "../helpers/utils.h"

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

	void loop() {

		std::lock_guard<std::mutex> lock(reader_mutex);

		if (overlay::c4Esp) {

		}

		if (g_game.isC4Planted)
		{
			Vector3 c4Origin = g_game.c4.get_origin();
			const Vector3 c4ScreenPos = g_game.world_to_screen(&c4Origin);

			if (c4ScreenPos.z >= 0.01f) {
				float c4Distance = g_game.localOrigin.calculate_distance(c4Origin);
				float c4RoundedDistance = std::round(c4Distance / 500.f);

				float height = 10 - c4RoundedDistance;
				float width = height * 1.4f;
				
				gui::DrawFilledBox(
					c4ScreenPos.x - (width / 2),
					c4ScreenPos.y - (height / 2),
					width,
					height,
					IM_COL32(225, 75, 75, 255)
				);

				gui::RenderText(
					c4ScreenPos.x + (width / 2 + 5),
					c4ScreenPos.y,
					"C4",
					overlay::esp_name_color,
					10
				);
			}
		}

		int playerIndex = 0;
		uintptr_t list_entry;

		/**
		* Loop through all the players in the entity list
		*
		* (This could have been done by getting a entity list count from the engine, but I'm too lazy to do that)
		**/
		for (auto player = g_game.players.begin(); player < g_game.players.end(); player++) {
			const Vector3 screenPos = g_game.world_to_screen(&player->origin);
			const Vector3 screenHead = g_game.world_to_screen(&player->head);

			if (
				screenPos.z < 0.01f ||
				!utils.is_in_bounds(screenPos, g_game.game_bounds.right, g_game.game_bounds.bottom)
				)
				continue;

			const float height = screenPos.y - screenHead.y;
			const float width = height / 2.4f;

			float distance = g_game.localOrigin.calculate_distance(player->origin);
			int roundedDistance = std::round(distance / 10.f);

			if (overlay::dotEsp) {
				gui::DrawCircle(
					player->bones.bonePositions["head"].x,
					player->bones.bonePositions["head"].y - width / 12,
					width / 5,
					(g_game.localTeam == player->team ? overlay::esp_skeleton_color_team : overlay::esp_skeleton_color_enemy)
				);
			}

			if (overlay::skeletonEsp) {
				for (const auto& connection : boneConnections) {
					const std::string& boneFrom = connection.first;
					const std::string& boneTo = connection.second;

					gui::DrawLine(
						player->bones.bonePositions[boneFrom].x, player->bones.bonePositions[boneFrom].y,
						player->bones.bonePositions[boneTo].x, player->bones.bonePositions[boneTo].y,
						g_game.localTeam == player->team ? overlay::esp_skeleton_color_team : overlay::esp_skeleton_color_enemy
					);
				}
			}

			if (overlay::boxEsp)
			{
				gui::DrawBorderBox(
					screenHead.x - width / 2,
					screenHead.y,
					width,
					height,
					(g_game.localTeam == player->team ? overlay::esp_box_color_team : overlay::esp_box_color_enemy)
				);
			}


			if (overlay::nameEsp) {

				gui::RenderText(
					screenHead.x + (width / 2 + 5),
					screenHead.y,
					player->name.c_str(),
					overlay::esp_name_color,
					10
				);
			}

			
			if (roundedDistance > overlay::flag_render_distance)
				continue;

			if (overlay::hpEsp) {
				gui::DrawBorderBox(
					screenHead.x - (width / 2 + 5),
					screenHead.y + (height * (100 - player->health) / 100),
					2,
					height - (height * (100 - player->health) / 100),
					RGB(
						(255 - player->health),
						(55 + player->health * 2),
						75
					)
				);

				gui::RenderText(
					screenHead.x + (width / 2 + 5),
					screenHead.y + 10,
					(std::to_string(player->health) + "hp").c_str(),
					RGB(
						(255 - player->health),
						(55 + player->health * 2),
						75
					),
					10
				);
			}
			

			if (overlay::armorEsp) {

				gui::DrawBorderBox(
					screenHead.x - (width / 2 + 10),
					screenHead.y + (height * (100 - player->armor) / 100),
					2,
					height - (height * (100 - player->armor) / 100),
					RGB(0, 185, 255)
				);

				gui::RenderText(
					screenHead.x + (width / 2 + 5),
					screenHead.y + 20,
					(std::to_string(player->armor) + "armor").c_str(),
					RGB(
						(255 - player->armor),
						(55 + player->armor * 2),
						75
					),
					10
				);
			}


			gui::RenderText(
				screenHead.x + (width / 2 + 5),
				screenHead.y + 40,
				(std::to_string(roundedDistance) + "m away").c_str(),
				overlay::esp_distance_color,
				10
			);
		
			if (overlay::moneyEsp) {
				gui::RenderText(
					screenHead.x + (width / 2 + 5),
					screenHead.y + 50,
					("$" + std::to_string(player->money)).c_str(),
					RGB(0, 125, 0),
					10
				);
			}
				

			if (overlay::isFlashedEsp) {
				if (player->flashAlpha > 100)
				{
					gui::RenderText(
						screenHead.x + (width / 2 + 5),
						screenHead.y + 60,
						"Player is flashed",
						overlay::esp_distance_color,
						10
					);
				}
			}
				
			if (overlay::isDefusingEsp) {
				if (player->is_defusing)
				{
					const std::string defuText = "Player is defusing";
					gui::RenderText(
						screenHead.x + (width / 2 + 5),
						screenHead.y + 60,
						defuText.c_str(),
						overlay::esp_distance_color,
						10
					);
				}
			}
				
			
		}
		// std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}
