#pragma once
#include "game/Game.h"
#include "scene/SeatGrid.h"

// ImGui screens + HUD + F1 help. Called between ImGuiLayer::beginFrame() and render().
namespace ui {
struct Hud {
    const char* team;
    const char* topic;
    float fps;
    bool shadowsOn;
    int shadowRes;
    bool paused;
    bool helpVisible;
    bool orbit;
    bool vsync;
    float biasTexels;
    int pcfKernel;  // 1, 3 or 5 (kernel side)
    const char* benchmark;  // "" when idle
};
UiAction draw(const Game& game, const SeatGrid& seats, const Hud& hud);
}  // namespace ui
