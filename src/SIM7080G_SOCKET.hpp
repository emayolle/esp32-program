#pragma once
#ifndef SIM7080G_SOCKET_H
#define SIM7080G_SOCKET_H
#include <nlohmann/json.hpp>
#include "Utils.hpp"
#include "SIM7080G_SERIAL.hpp"

class SocketClient
{
public:
    char ip[100];
    char protocol[5];
    int port;

    SocketClient(char ip[100], int port, char protocol[5]);
    ~SocketClient();

    bool connect();
    bool send(String message);
    bool send(nlohmann::json j);
    bool close();
};

#endif // SIM7080G_SOCKET_H