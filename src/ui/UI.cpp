#include "ui/UI.h"

#include <imgui.h>

#include <cstdio>

namespace {
const char* stageName(AppState s) {
    switch (s) {
        case AppState::Menu: return "MENU";
        case AppState::Hall: return "HALL";
        case AppState::Preview: return "PREVIEW";
        case AppState::Confirm: return "CONFIRM";
        case AppState::Purchased: return "PURCHASED";
    }
    return "?";
}
const ImGuiWindowFlags kPanel = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove |
                                ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
}  // namespace

namespace ui {
UiAction draw(const Game& game, const SeatGrid& seats, const Hud& hud) {
    UiAction act = UiAction::None;
    ImVec2 ds = ImGui::GetIO().DisplaySize;
    AppState st = game.state();

    // HUD
    ImGui::SetNextWindowPos(ImVec2(10, 10));
    ImGui::SetNextWindowBgAlpha(0.45f);
    ImGui::Begin("hud", nullptr, kPanel);
    ImGui::Text("%s", hud.team);
    ImGui::Text("%s", hud.topic);
    ImGui::Text("Этап: %s", stageName(st));
    ImGui::Text("Тени: %s (%d)  PCF %dx%d  bias %.2f", hud.shadowsOn ? "ВКЛ" : "ВЫКЛ", hud.shadowRes, hud.pcfKernel, hud.pcfKernel, hud.biasTexels);
    ImGui::Text("FPS: %.0f%s%s", hud.fps, hud.paused ? "  [ПАУЗА]" : "", hud.orbit ? "  [ORBIT]" : "");
    if (hud.benchmark && hud.benchmark[0]) ImGui::Text("%s", hud.benchmark);
    ImGui::End();

    auto centered = [&](const char* id, float yFrac) {
        ImGui::SetNextWindowPos(ImVec2(ds.x * 0.5f, ds.y * yFrac), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
        ImGui::SetNextWindowBgAlpha(0.8f);
        ImGui::Begin(id, nullptr, kPanel);
    };

    if (hud.helpVisible) {
        centered("help", 0.5f);
        ImGui::Text("УПРАВЛЕНИЕ");
        ImGui::Separator();
        ImGui::Text("WASD + мышь   движение и обзор (Shift - быстрее)");
        ImGui::Text("ЛКМ           выбрать кресло в прицеле");
        ImGui::Text("T             тени вкл/выкл");
        ImGui::Text("Q             качество теней 512 / 2048");
        ImGui::Text("Стрелки       двигать свет");
        ImGui::Text("P             пауза анимации");
        ImGui::Text("O             обзор зала (orbit) / от первого лица");
        ImGui::Text("[ ]  K  C     bias, ядро PCF (1x1/3x3/5x5), culling depth-прохода");
        ImGui::Text("V  F5  F6     vsync, замер FPS (4 режима), перезагрузка шейдеров");
        ImGui::Text("F1 / H        помощь");
        ImGui::Text("Esc           назад / выход");
        ImGui::End();
    } else {
        if (st == AppState::Menu) {
            centered("menu", 0.5f);
            ImGui::Text("Cinema Seat Preview");
            if (ImGui::Button("Купить билет", ImVec2(220, 50))) act = UiAction::BuyTicket;
            ImGui::End();
        } else if (st == AppState::Hall) {
            ImDrawList* dl = ImGui::GetForegroundDrawList();
            dl->AddCircle(ImVec2(ds.x * 0.5f, ds.y * 0.5f), 6.0f, IM_COL32(255, 255, 255, 220), 16, 2.0f);
            ImGui::SetNextWindowPos(ImVec2(ds.x * 0.5f, ds.y - 20), ImGuiCond_Always, ImVec2(0.5f, 1.0f));
            ImGui::SetNextWindowBgAlpha(0.45f);
            ImGui::Begin("hint", nullptr, kPanel);
            ImGui::Text("WASD + мышь: ходить | ЛКМ: выбрать кресло в прицеле | F1: помощь");
            ImGui::End();
        } else if (st == AppState::Preview) {
            centered("preview", 0.9f);
            ImGui::Text("Вид с места...");
            ImGui::End();
        } else if (st == AppState::Confirm) {
            centered("confirm", 0.88f);
            int id = game.selectedSeat();
            if (id >= 0) ImGui::Text("Ряд %d, место %d", seats.seats()[id].row + 1, seats.seats()[id].col + 1);
            if (ImGui::Button("Выбрать", ImVec2(140, 40))) act = UiAction::Select;
            ImGui::SameLine();
            if (ImGui::Button("Назад", ImVec2(140, 40))) act = UiAction::Back;
            ImGui::End();
        } else if (st == AppState::Purchased) {
            centered("purchased", 0.5f);
            if (const Ticket* t = game.ticket()) {
                ImGui::Text("БИЛЕТ");
                ImGui::Separator();
                ImGui::Text("Ряд: %d   Место: %d   Сеанс: %s", t->row, t->col, t->session.c_str());
                ImGui::Text("Сохранено в ticket.txt");
                if (ImGui::Button("В зал", ImVec2(140, 40))) act = UiAction::Back;
            } else {
                if (const Seat* s = (game.selectedSeat() >= 0 ? &seats.seats()[game.selectedSeat()] : nullptr)) {
                    ImGui::Text("Ряд %d, место %d, сеанс: %s", s->row + 1, s->col + 1, "20:00");
                }
                if (ImGui::Button("Купить", ImVec2(140, 40))) act = UiAction::Buy;
                ImGui::SameLine();
                if (ImGui::Button("Назад", ImVec2(140, 40))) act = UiAction::Back;
            }
            ImGui::End();
        }
    }
    return act;
}
}  // namespace ui
