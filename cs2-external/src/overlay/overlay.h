#pragma once
#define d_toggle_bind 0x2D /* VK_INSERT */
#include "../../include/imgui/imgui.h"
#include "../../include/imgui/imgui_internal.h"

namespace overlay
{
	void render();

	inline bool enabled = true;
	inline bool teamEsp = false;
	inline bool dotEsp = false;
	inline bool boxEsp = false;
	inline bool skeletonEsp = false;
	inline bool hpEsp = false;
	inline bool armorEsp = false;
	inline bool nameEsp = false;
	inline bool isFlashedEsp = false;
	inline bool isDefusingEsp = false;
	inline bool moneyEsp = false;
	inline bool c4Esp = false;

	inline float render_distance = -1.f;
	inline int flag_render_distance = 200;


	inline ImColor esp_box_color_team = ImColor(75, 175, 75);       
	inline ImColor esp_box_color_enemy = ImColor(225, 75, 75);      
	inline ImColor esp_skeleton_color_team = ImColor(75, 175, 75);  
	inline ImColor esp_skeleton_color_enemy = ImColor(225, 75, 75);
	inline ImColor esp_name_color = ImColor(75, 75, 175);           
	inline ImColor esp_distance_color = ImColor(75, 75, 175);       
}
#pragma once
