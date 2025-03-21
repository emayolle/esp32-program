#pragma once
#ifndef SIM7080G_FSM_H
#define SIM7080G_FSM_H
#include "STATE.hpp"
#include "SIM7080G_SERIAL.hpp"
#include "SIM7080G_CATM1.hpp"
#include "SIM7080G_GNSS.hpp"
#include "SIM7080G_SOCKET.hpp"

namespace SIM7080G
{
    void FSM();

    inline SocketClient *socketClient;
    inline String imei = "";
} // namespace SIM7080G

#endif // SIM7080G_FSM_H