#pragma once
#define d_toggle_bind 0x2D /* VK_INSERT */

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

	inline float render_distance = -1.f;
	inline int flag_render_distance = 200;
}
#pragma once
