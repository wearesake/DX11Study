#include "ImguiManager.h"
#include "../Code/Imgui/imgui.h"

ImguiManager::ImguiManager()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
}

ImguiManager::~ImguiManager()
{
    ImGui::DestroyContext();
}
