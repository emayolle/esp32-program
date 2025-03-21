#include "SIM7080G_SERIAL.hpp"

int countFail = 0;
AT_RESPONSE response;

/**
 * @brief Send AT command to SIM7080G
 * @param message AT command
 * @param timeout Timeout in milliseconds
 * @return AT_RESPONSE
 */
AT_RESPONSE SIM7080G::SERIAL::send_AT(String message, int timeout)
{
    if (at_state == AT_EMPTY)
    {
        /* Reset response */
        response.isFinished = false;
        response.message = "";
        Sim7080G.println(message);

        /* Set AT state to AT_WAITING, to avoid multiple call */
        SET_ATSTATE(AT_WAITING);
    }

    if (at_state == AT_WAITING)
    {
        if (Sim7080G.available())
        {
            response.message += (char)Sim7080G.read();
        }

        if (millis() - atStateTimer > (timeout == -1 ? 1000 : timeout) || (timeout == -1 ? response.message.endsWith("OK\x00D\x00A") : false))
        {
            /* Free AT state */
            SET_ATSTATE(AT_EMPTY);
            response.isFinished = true;
        }
    }

    return response;
}

/**
 * @brief Send AT command without FSM architecture to SIM7080G
 * @param message AT command
 * @param timeout Timeout in milliseconds
 * @return String
 */
String SIM7080G::SERIAL::send_AT_bloquant(String message, int timeout)
{
    uint32_t startTime = millis();
    Sim7080G.println(message);

    String response = "";
    while (millis() - startTime < timeout)
    {
        if (timeout == 1000)
        {
            if (response.endsWith("OK\r"))
            {
                break;
            }
        }

        if (Sim7080G.available())
        {

            char byte_recv = Sim7080G.read();
            response += byte_recv;
        }
    }

#pragma region ANCIEN CODE PAS PROPRE ET PAS OPTI POUR POSTPROCESS LA REPONSE
    response = response.substring(response.indexOf("\r"), response.length());
    response.trim();

    if (timeout == 1000)
    {

        if (response.indexOf("\n") != -1)
        {
            response = response.substring(0, response.length() - 3);
            response.trim();
        }
    }
#pragma endregion

    if (response.length() == 0)
    {
        countFail += 1;
        if (countFail >= 5)
        {
            SIM7080G::POWER::HardReset();
        };
        return "";
    }

    return response;
}

/**
 * @brief Send AT command to SIM7080G with debug
 * @param message AT command
 * @param isDebug Debug mode
 * @param timeout Timeout in milliseconds
 * @return AT_RESPONSE
 */
AT_RESPONSE SIM7080G::SERIAL::send_AT(String message, bool isDebug, int timeout)
{
    Serial.printf("[?] Message: `%s`\n", message.c_str());

    AT_RESPONSE response = send_AT(message, timeout);

    Serial.printf("[+] Response: `%s` (size: %d)\n", response.message.c_str(), response.message.length());

    return response;
}

/**
 * @brief Send AT command without FSM architecture to SIM7080G with debug
 * @param message AT command
 * @param isDebug Debug mode
 * @param timeout Timeout in milliseconds
 * @return String
 */
String SIM7080G::SERIAL::send_AT_bloquant(String message, bool isDebug, int timeout)
{
    Serial.printf("[?] Message: `%s`\n", message.c_str());

    String response = send_AT_bloquant(message, timeout);

    Serial.printf("[+] Response: `%s` (size: %d)\n", response.c_str(), response.length());

    return response;
}