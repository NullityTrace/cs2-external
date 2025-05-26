#pragma once
#include "../../include/imgui/imgui.h"
#include "../../include/imgui/imgui_internal.h"

namespace gui {
    void drawmenu();
    void RenderText(float x, float y, const char* text, ImColor textColor, float fontSize);
    void DrawFilledBox(float x, float y, float width, float height, ImColor color);
    void DrawBorderBox(float x, float y, float w, float h, ImColor borderColor);
    void DrawCircle(float x, float y, float radius, ImColor color);
    void DrawLine(float x1, float y1, float x2, float y2, ImColor color);
}
