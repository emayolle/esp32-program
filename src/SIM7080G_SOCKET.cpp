#include "SIM7080G_SOCKET.hpp"

/**
 * @brief Construct a new Socket Client:: Socket Client object
 * @param ip IP address
 * @param port Port
 * @param protocol Protocol
 */
SocketClient::SocketClient(char ip[100], int port, char protocol[5])
{
    this->port = port;
    strcpy(this->ip, ip);
    strcpy(this->protocol, protocol);
}

/**
 * @brief Connect to the server
 * @return true if connected
 * @return false if not connected
 */
bool SocketClient::connect()
{
    AT_RESPONSE connectResponse = SIM7080G::SERIAL::send_AT("AT+CAOPEN=0,0,\"" + String(protocol) + "\",\"" + String(ip) + "\"," + String(port), 1000);

    if (connectResponse.isFinished && connectResponse.message.indexOf("+CAOPEN: 0,0") != -1)
    {
        return true;
    }

    return false;
}

/**
 * @brief Send message to the server
 * @param message Message to send
 * @return true if sent
 * @return false if not sent
 */
bool SocketClient::send(String message)
{
    if (tcp_state == TCP_EMPTY)
    {
        SET_TCPSTATE(TCP_SEND_SIZE);
    }

    if (tcp_state == TCP_SEND_SIZE)
    {
        AT_RESPONSE response = SIM7080G::SERIAL::send_AT("AT+CASEND=0," + String(message.length()));
        if (response.isFinished && !response.message.isEmpty())
        {
            SET_TCPSTATE(TCP_SEND_DATA);
        }
    }

    if (tcp_state == TCP_SEND_DATA)
    {
        AT_RESPONSE response = SIM7080G::SERIAL::send_AT(message);
        if (response.isFinished && !response.message.isEmpty())
        {
            SET_TCPSTATE(TCP_EMPTY);
            return true;
        }
    }
}

/**
 * @brief Send message to the server
 * @param j JSON message to send
 * @return true if sent
 * @return false if not sent
 */
bool SocketClient::send(nlohmann::json j)
{
    std::vector<std::uint8_t> v_cbor = nlohmann::json::to_cbor(j);

    String sendData = "";
    for (std::uint8_t i : v_cbor)
    {
        sendData += (char)i;
    }

    return this->send(sendData);
}

/**
 * @brief Close the connection
 * @return true if closed
 * @return false if not closed
 */
bool SocketClient::close()
{
    AT_RESPONSE response = SIM7080G::SERIAL::send_AT("AT+CACLOSE=0", 1000);

    if (response.isFinished)
    {
        return true;
    }

    return false;
}

/**
 * @brief Destroy the Socket Client:: Socket Client object
 */
SocketClient::~SocketClient()
{
    this->close();
}