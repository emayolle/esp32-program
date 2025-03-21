#include "SIM7080G_POWER.hpp"

void SIM7080G::POWER::On()
{
    if (state == STARTING)
    {
        digitalWrite(PWR_KEY, LOW);
        SET_STATE(STARTING_ON_PWR_KEY);
    }

    if (state == STARTING_ON_PWR_KEY && millis() - stateTimer > 200)
    {
        digitalWrite(PWR_KEY, OUTPUT_OPEN_DRAIN);
        SET_STATE(STARTING_ON_BEFORE_END);
    }

    if (state == STARTING_ON_BEFORE_END && millis() - stateTimer > 1000)
    {
        SET_STATE(TEST_AT);
    }
}

void SIM7080G::POWER::Off()
{
    AT_RESPONSE response = SIM7080G::SERIAL::send_AT("AT+CPOWD=0");

    if (response.isFinished && !response.message.isEmpty())
    {
        SET_STATE(STARTING);
    }
}

void SIM7080G::POWER::Reboot()
{
    if (state == STOPPING)
    {
        Off();
    }

    if (state == STARTING)
    {
        On();
    }
}

void SIM7080G::POWER::HardReset()
{
    if (state == HARDRESET)
    {
        digitalWrite(PWR_KEY, LOW);
        SET_STATE(HARDRESET_PWR_KEY);
    }

    if (state == HARDRESET_PWR_KEY && millis() - stateTimer > 15000)
    {
        digitalWrite(PWR_KEY, OUTPUT_OPEN_DRAIN);
        SET_STATE(HARDRESET_BEFORE_END);
    }

    if (state == HARDRESET_BEFORE_END && millis() - stateTimer > 3000)
    {
        SET_STATE(STARTING);
    }
}

CHARGING_INFO SIM7080G::POWER::ChargeInfo()
{
    AT_RESPONSE response = SIM7080G::SERIAL::send_AT("AT+CBC");
    CHARGING_INFO chargInfo;

    if (response.isFinished && response.message.length() <= 0)
    {
        chargInfo.status = (CHARGING_STATUS)response.message.substring(response.message.indexOf(",") - 1, response.message.indexOf(",")).toInt();
        chargInfo.batteryPercentage = response.message.substring(response.message.indexOf(",") + 1, response.message.lastIndexOf(",")).toInt();
        chargInfo.batteryVoltage = response.message.substring(response.message.lastIndexOf(",") + 1).toInt();
    }

    return chargInfo;
}