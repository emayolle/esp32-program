#pragma once
#ifndef SIM7080G_POWER_H
#define SIM7080G_POWER_H
#include "STATE.hpp"
#include "ARGALI_PINOUT.h"
#include "SIM7080G_SERIAL.hpp"

enum CHARGING_STATUS
{
    NOT_CHARGING = 0,
    CHARGING = 1,
    CHARGED = 2,
};

struct CHARGING_INFO
{
    CHARGING_STATUS status;
    int batteryPercentage;
    int batteryVoltage;
};

namespace SIM7080G
{
    namespace POWER
    {
        void On();
        void Off();
        void Reboot();
        void HardReset();
        CHARGING_INFO ChargeInfo();
    }; // namespace Power
}; // namespace SIM7080G

#endif // SIM7080G_POWER_H