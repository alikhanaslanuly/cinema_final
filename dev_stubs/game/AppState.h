#pragma once

// STUB (real: Arman). Contract: AppState. UiAction is an addition proposed by Alikhan (see CONTRACT.md).
enum class AppState { Menu, Hall, Preview, Confirm, Purchased };

// What the ImGui screens report back to the game logic (buttons).
enum class UiAction { None, BuyTicket, Select, Buy, Back };
