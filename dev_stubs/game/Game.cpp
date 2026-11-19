#include "game/Game.h"

#include <fstream>

void Game::seatPicked(int id, SeatGrid& seats) {
    if (m_state != AppState::Hall) return;
    m_seat = id;
    m_hasTicket = false;
    seats.setSelected(id);
    m_state = AppState::Preview;
}

void Game::cameraArrived() {
    if (m_state == AppState::Preview) m_state = AppState::Confirm;
}

void Game::apply(UiAction a, SeatGrid& seats) {
    switch (a) {
        case UiAction::BuyTicket:
            if (m_state == AppState::Menu) m_state = AppState::Hall;
            break;
        case UiAction::Select:
            if (m_state == AppState::Confirm) m_state = AppState::Purchased;
            break;
        case UiAction::Buy:
            if (m_state == AppState::Purchased && !m_hasTicket && m_seat >= 0) {
                const Seat& s = seats.seats()[m_seat];
                m_ticket.seatId = s.id;
                m_ticket.row = s.row + 1;
                m_ticket.col = s.col + 1;
                m_ticket.session = "20:00";
                m_hasTicket = true;
                seats.setTaken(m_seat, true);
                seats.setSelected(-1);
                std::ofstream f("ticket.txt");
                f << "Cinema ticket\nRow: " << m_ticket.row << "\nSeat: " << m_ticket.col << "\nSession: " << m_ticket.session << "\n";
            }
            break;
        case UiAction::Back:
            back(seats);
            break;
        case UiAction::None:
            break;
    }
}

bool Game::back(SeatGrid& seats) {
    switch (m_state) {
        case AppState::Menu: return false;
        case AppState::Hall: m_state = AppState::Menu; return true;
        case AppState::Preview:
        case AppState::Confirm:
            seats.setSelected(-1);
            m_seat = -1;
            m_state = AppState::Hall;
            return true;
        case AppState::Purchased:
            if (m_hasTicket) { m_seat = -1; m_state = AppState::Hall; }
            else m_state = AppState::Confirm;
            return true;
    }
    return true;
}
