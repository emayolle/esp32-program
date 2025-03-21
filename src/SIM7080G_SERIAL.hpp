#pragma once
#include <Arduino.h>
#undef SERIAL

#ifndef SIM7080G_SERIAL_H
#define SIM7080G_SERIAL_H

#define Sim7080G Serial1
#define Sim7080G_BAUDRATE 57600
#include "SIM7080G_POWER.hpp"

struct AT_RESPONSE
{
    String message = "";
    bool isFinished = false;
};

namespace SIM7080G
{
    namespace SERIAL
    {
        AT_RESPONSE send_AT(String message, int timeout = -1);
        String send_AT_bloquant(String message, int timeout = -1);
        AT_RESPONSE send_AT(String message, bool isDebug, int timeout = 1000);
        String send_AT_bloquant(String message, bool isDebug, int timeout = 1000);
    } // namespace SERIAL
}

#endif // SIM7080G_SERIAL_H