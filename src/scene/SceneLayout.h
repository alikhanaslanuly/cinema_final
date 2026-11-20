#pragma once
// scene/SceneLayout.h (Куаныш) — общие размеры зала, чтобы Hall и SeatGrid совпадали.
namespace layout {
constexpr int   kRows = 2;
constexpr int   kCols = 5;
constexpr float kColStep = 1.3f;                 // расстояние между креслами по X
constexpr float kRowZ[kRows] = {0.0f, 2.2f};     // центр кресла по Z (ряд 0 ближе к экрану)
constexpr float kRowY[kRows] = {0.0f, 0.6f};     // высота пола под рядом (уступ)

constexpr float kHallHalfW = 4.5f;               // зал по X: [-4.5, 4.5]
constexpr float kHallZFront = -10.0f;            // передняя стена
constexpr float kHallZBack = 6.0f;               // задняя стена
constexpr float kHallHeight = 6.0f;
constexpr float kStepZ = 1.0f;                   // начало уступа (второго ряда) по Z

constexpr float kScreenZ = -9.95f;
constexpr float kScreenHalfW = 4.0f;
constexpr float kScreenY0 = 0.9f, kScreenY1 = 4.3f;
}