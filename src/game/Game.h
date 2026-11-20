#pragma once
#include <string>

#include "game/AppState.h"
#include "scene/SeatGrid.h"

// Scenario state machine MENU -> HALL -> PREVIEW -> CONFIRM -> PURCHASED.
struct Ticket {
    int seatId = -1, row = 0, col = 0;
    std::string session;
};

class Game {
public:
    AppState state() const { return m_state; }
    int selectedSeat() const { return m_seat; }
    const Ticket* ticket() const { return m_hasTicket ? &m_ticket : nullptr; }

    void apply(UiAction a, SeatGrid& seats);  // button pressed
    void seatPicked(int id, SeatGrid& seats); // HALL -> PREVIEW (ray hit a free seat)
    void cameraArrived();                     // PREVIEW -> CONFIRM (camera finished gliding)
    bool back(SeatGrid& seats);               // Esc; false if already in MENU (-> quit)

private:
    AppState m_state = AppState::Menu;
    int m_seat = -1;
    bool m_hasTicket = false;
    Ticket m_ticket;
};
