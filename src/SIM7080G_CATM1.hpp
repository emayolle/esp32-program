#pragma once
#ifndef SIM7080G_CATM1_H
#define SIM7080G_CATM1_H
#include "Utils.hpp"
#include "SIM7080G_FSM.hpp"
#include "SIM7080G_SERIAL.hpp"
// #include "SIM7080G_SOCKET.hpp"

namespace SIM7080G::CATM1
{
    void Setup();
    void Loop();
    bool IsPDPActive();
    bool WaitForNetworkRegistration();
    String GetIpAddress(String response);
    bool WaitForIp();
} // namespace SIM7080G::CATM1

#endif // SIM7080G_CATM1_H