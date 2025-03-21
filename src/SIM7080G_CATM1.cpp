#include "SIM7080G_CATM1.hpp"

void SIM7080G::CATM1::Setup()
{
    if (state == CATM1_INIT)
    {
        SET_STATE(CATM1_SET_MODE_GSM);
    }

    if (state == CATM1_SET_MODE_GSM)
    {
        AT_RESPONSE response = SIM7080G::SERIAL::send_AT("AT+CNMP=38");
        if (response.isFinished && !response.message.isEmpty())
        {
            SET_STATE(CATM1_SET_CATM);
        }
    }

    if (state == CATM1_SET_CATM)
    {
        AT_RESPONSE response = SIM7080G::SERIAL::send_AT("AT+CMNB=1");
        if (response.isFinished && !response.message.isEmpty())
        {
            SET_STATE(CATM1_DEACTIVATE_PDP);
        }
    }

    if (state == CATM1_DEACTIVATE_PDP)
    {
        AT_RESPONSE response = SIM7080G::SERIAL::send_AT("AT+CNACT=0,0");
        if (response.isFinished && !response.message.isEmpty())
        {
            SET_STATE(CATM1_DEFINE_PDPCTX);
        }
    }

    if (state == CATM1_DEFINE_PDPCTX)
    {
        AT_RESPONSE response = SIM7080G::SERIAL::send_AT("AT+CGDCONT=1,\"IP\",\"iot.1nce.net\"");
        if (response.isFinished && !response.message.isEmpty())
        {
            SET_STATE(CATM1_CONFIGURE_PDPCTX);
        }
    }

    if (state == CATM1_CONFIGURE_PDPCTX)
    {
        AT_RESPONSE response = SIM7080G::SERIAL::send_AT("AT+CNCFG=0,1,iot.1nce.net");
        if (response.isFinished && !response.message.isEmpty())
        {
            SET_STATE(CATM1_ACTIVATE_PDP);
        }
    }

    if (state == CATM1_ACTIVATE_PDP)
    {
        if (IsPDPActive())
            SET_STATE(CATM1_WAITING_FOR_NETWORK);
    }

    if (state == CATM1_WAITING_FOR_NETWORK)
    {
        if (WaitForNetworkRegistration())
            SET_STATE(CATM1_WAITING_FOR_IP);
    }

    if (state == CATM1_WAITING_FOR_IP)
    {
        if (WaitForIp())
            SET_STATE(TCP_INIT);
    }
}

bool SIM7080G::CATM1::IsPDPActive()
{
    AT_RESPONSE response = SIM7080G::SERIAL::send_AT("AT+CNACT=0,1", 10000);

    if (response.isFinished && !response.message.isEmpty())
    {
        // Set state "STARTING" for all but if return true Set will be overriden by the caller choice
        SET_STATE(STARTING);

        if (response.message.indexOf("ERROR") != -1)
        {
            Serial.println("[x] PDP context error");
        }
        else if (response.message.indexOf("DEACTIVE") != -1)
        {
            Serial.println("[x] PDP context is not active");
        }
        else if (response.message.indexOf("ACTIVE") != -1)
        {
            Serial.println("[+] PDP context is active");
            return true;
        }
        else
        {
            Serial.println("[!] PDP context not detected!");
        }
    }
    return false;
}

bool SIM7080G::CATM1::WaitForNetworkRegistration()
{
    AT_RESPONSE response = SIM7080G::SERIAL::send_AT("AT+CEREG?");
    if (response.isFinished && !response.message.isEmpty())
    {
        Serial.println(response.message);
        if (response.message.indexOf("5") != -1)
            return true;
    }

    return false;
}

String SIM7080G::CATM1::GetIpAddress(String response)
{
    String ip;

    response = response.substring(response.indexOf("\"") + 1);
    ip = response.substring(0, response.indexOf("\""));

    return ip;
}

bool SIM7080G::CATM1::WaitForIp()
{
    AT_RESPONSE response = SIM7080G::SERIAL::send_AT("AT+CNACT?");
    if (response.isFinished && !response.message.isEmpty())
    {
        String ip = GetIpAddress(response.message);
        if (!ip.isEmpty())
        {
            Serial.printf("[+] IP address: %s\n", ip.c_str());
            if (ip.indexOf("0.0.0.0") == -1)
            {
                // SET_STATE(TCP_CONNECT);
                return true;
            }
        }
    }
    return false;
}