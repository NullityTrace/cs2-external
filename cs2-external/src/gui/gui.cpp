#include "../overlay/overlay.h"
#include "../../include/imgui/imgui.h"
#include "../../include/imgui/imgui_internal.h"
#include <string>


namespace gui {

    enum class MenuTab {
        Visuals,
        Config
    };

    inline MenuTab activeTab = MenuTab::Visuals;

    void ApplyFlatDarkPurpleTheme() {
        ImGuiStyle& style = ImGui::GetStyle();
        ImVec4* colors = style.Colors;

        colors[ImGuiCol_TitleBg] = ImVec4(0.08f, 0.06f, 0.12f, 1.0f);
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.15f, 0.1f, 0.2f, 1.0f);
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.05f, 0.04f, 0.07f, 1.0f);

        colors[ImGuiCol_WindowBg] = ImVec4(0.05f, 0.04f, 0.07f, 1.0f);
        colors[ImGuiCol_Text] = ImVec4(0.8f, 0.7f, 0.9f, 1.0f);
        colors[ImGuiCol_Border] = ImVec4(0.3f, 0.2f, 0.4f, 0.7f);
        colors[ImGuiCol_FrameBg] = ImVec4(0.08f, 0.06f, 0.12f, 1.0f);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.2f, 0.15f, 0.3f, 1.0f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.3f, 0.25f, 0.4f, 1.0f);
        colors[ImGuiCol_Button] = ImVec4(0.15f, 0.1f, 0.2f, 1.0f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.25f, 0.2f, 0.3f, 1.0f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.35f, 0.25f, 0.45f, 1.0f);
        colors[ImGuiCol_CheckMark] = ImVec4(0.8f, 0.5f, 1.0f, 1.0f);

        style.WindowRounding = 0.0f;
        style.FrameRounding = 2.0f;
        style.GrabRounding = 2.0f;
        style.ScrollbarRounding = 2.0f;
    }

    void drawmenu() {
        ApplyFlatDarkPurpleTheme();

        ImGui::SetNextWindowSize(ImVec2(600, 400), ImGuiCond_Once);
        ImGui::Begin("NullityTraceEsp", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

        ImGui::BeginChild("Sidebar", ImVec2(120, 0), true);
        if (ImGui::Button("Visuals", ImVec2(-1, 40))) activeTab = MenuTab::Visuals;
        if (ImGui::Button("Config", ImVec2(-1, 40))) activeTab = MenuTab::Config;
        ImGui::EndChild();

        ImGui::SameLine();

        ImGui::BeginChild("MainContent", ImVec2(0, 0), true);

        if (activeTab == MenuTab::Visuals) {
            ImGui::Text("Visual Options");
            ImGui::Checkbox("Team ESP", &overlay::teamEsp);
            ImGui::Checkbox("Dot ESP", &overlay::dotEsp);
            ImGui::Checkbox("Box ESP", &overlay::boxEsp);
            ImGui::Checkbox("Skeleton ESP", &overlay::skeletonEsp);
            ImGui::Checkbox("Health", &overlay::hpEsp);
            ImGui::Checkbox("Armor", &overlay::armorEsp);
            ImGui::Checkbox("Name", &overlay::nameEsp);
            ImGui::Checkbox("Flashed", &overlay::isFlashedEsp);
            ImGui::Checkbox("Defusing", &overlay::isDefusingEsp);
            ImGui::Checkbox("Money", &overlay::moneyEsp);
        }

        else if (activeTab == MenuTab::Config) {
            ImGui::Spacing();
            ImGui::TextColored(ImVec4(0.8f, 0.5f, 1.0f, 1.0f), "Config tab is under development...");
            ImGui::Spacing();
            ImGui::Text("You will be able to save and load presets here soon!");
        }


        ImGui::EndChild();
        ImGui::End();
    }

    void DrawLine(float x1, float y1, float x2, float y2, ImColor color)
    {
        ImDrawList* draw_list = ImGui::GetForegroundDrawList();
        draw_list->AddLine(ImVec2(x1, y1), ImVec2(x2, y2), color, 2.0f); 
    }

    void DrawCircle(float x, float y, float radius, ImColor color)
    {
        ImDrawList* draw_list = ImGui::GetForegroundDrawList();
        draw_list->AddCircle(ImVec2(x, y), radius, color, 0, 2.0f); 
    }

    void DrawBorderBox(float x, float y, float w, float h, ImColor borderColor)
    {
        ImDrawList* draw_list = ImGui::GetForegroundDrawList();
        draw_list->AddRect(ImVec2(x, y), ImVec2(x + w, y + h), borderColor, 0.0f, 0, 2.0f); 
    }

    void DrawFilledBox(float x, float y, float width, float height, ImColor color)
    {
        ImDrawList* draw_list = ImGui::GetForegroundDrawList();
        draw_list->AddRectFilled(ImVec2(x, y), ImVec2(x + width, y + height), color);
    }


    void RenderText(float x, float y, const char* text, ImColor textColor, float fontSize)
    {
        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]); 
        ImGui::GetForegroundDrawList()->AddText(ImVec2(x, y), textColor, text);
        ImGui::PopFont();
    }


}
