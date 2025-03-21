#include "SIM7080G_FSM.hpp"

void SIM7080G::FSM()
{
    switch (state)
    {
    case STARTING:
        stateTimer = millis();
    case STARTING_ON_PWR_KEY:
    case STARTING_ON_BEFORE_END:
        SIM7080G::POWER::On();
        break;

    case HARDRESET:
    case HARDRESET_PWR_KEY:
    case HARDRESET_BEFORE_END:
        SIM7080G::POWER::HardReset();
        break;
    case TEST_AT:
    {
        AT_RESPONSE response = SIM7080G::SERIAL::send_AT("AT");
        if (response.isFinished)
        {
            if (response.message.isEmpty() || response.message[0] == '\0' || response.message.indexOf("OK") == -1)
            {
                Serial.printf("AT Command not reacheable, HARDRESET\n");
                SET_STATE(HARDRESET);
                break;
            }

            Serial.printf("ATE: %s\n", response.message.c_str());
            SET_STATE(CATM1_INIT);
        }

        break;
    }

    case CATM1_INIT:
    case CATM1_SET_MODE_GSM:
    case CATM1_SET_CATM:
    case CATM1_DEACTIVATE_PDP:
    case CATM1_DEFINE_PDPCTX:
    case CATM1_CONFIGURE_PDPCTX:
    case CATM1_ACTIVATE_PDP:
    case CATM1_WAITING_FOR_NETWORK:
    case CATM1_WAITING_FOR_IP:
        SIM7080G::CATM1::Setup();
        break;

    case TCP_INIT:
    {
        socketClient = new SocketClient("2.tcp.eu.ngrok.io", 11562, "TCP");
        SET_STATE(TCP_CONNECT);
        break;
    }
    case TCP_CONNECT:
    {
        if (socketClient->connect())
        {
            SET_STATE(TCP_SEND_TEST_GET_IMEI);
        }
        break;
    }
    case TCP_SEND_TEST_GET_IMEI:
    {
        AT_RESPONSE IMEI = SIM7080G::SERIAL::send_AT("AT+GSN");
        if (IMEI.isFinished && !IMEI.message.isEmpty() && IMEI.message.endsWith("OK\r\n"))
        {
            IMEI.message = IMEI.message.substring(IMEI.message.indexOf("\r\n") + 2);
            IMEI.message = IMEI.message.substring(0, IMEI.message.length() - 5);
            IMEI.message.trim();
            SIM7080G::imei = IMEI.message;
            SET_STATE(TCP_SEND_TEST);
        }
        break;
    }
    case TCP_SEND_TEST:
    {
        nlohmann::json data = nlohmann::json{
            {"i", SIM7080G::imei.c_str()}};
        if (socketClient->send(data))
        {
            SET_STATE(TCP_CLOSE);
        }
        break;
    }
    case TCP_CLOSE:
    {
        socketClient->close();
        SET_STATE(EMPTY);
        break;
    }

    default:
        break;
    }
}