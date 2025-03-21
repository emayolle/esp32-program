#pragma once
#ifndef STATE_H
#define STATE_H

#define AD_STATE(state) Serial.printf("%s (%dms)\n", #state, millis() - stateTimer);
#define SET_STATE(newState)                                                              \
    if (at_state == AT_EMPTY && tcp_state == TCP_EMPTY)                                  \
    {                                                                                    \
        Serial.printf("Set state to %s after %dms\n", #newState, millis() - stateTimer); \
        state = newState;                                                                \
        stateTimer = millis();                                                           \
    }

#define SET_ATSTATE(newState)                                                             \
    Serial.printf("Set AT state to %s after %dms\n", #newState, millis() - atStateTimer); \
    at_state = newState;                                                                  \
    atStateTimer = millis();

#define SET_TCPSTATE(newState) \
    tcp_state = newState;      \
    tcpStateTimer = millis();

enum STATE_MACHINE
{
    STARTING,
    STARTING_ON_PWR_KEY,
    STARTING_ON_BEFORE_END,

    HARDRESET,
    HARDRESET_PWR_KEY,
    HARDRESET_BEFORE_END,

    TEST_AT,
    STOPPING,
    EMPTY,

    CATM1_INIT,
    CATM1_SET_MODE_GSM,
    CATM1_SET_CATM,
    CATM1_DEACTIVATE_PDP,
    CATM1_DEFINE_PDPCTX,
    CATM1_CONFIGURE_PDPCTX,
    CATM1_ACTIVATE_PDP,
    CATM1_WAITING_FOR_NETWORK,
    CATM1_WAITING_FOR_IP,

    TCP_INIT,
    TCP_CONNECT,
    TCP_SEND_TEST_GET_IMEI,
    TCP_SEND_TEST,
    TCP_CLOSE,

};

enum AT_STATE
{
    AT_EMPTY,
    AT_WAITING
};

enum TCP_STATE
{
    TCP_EMPTY,
    TCP_SEND_SIZE,
    TCP_SEND_DATA,
};

inline int stateTimer = 0;
inline int atStateTimer = 0;
inline int tcpStateTimer = 0;
inline STATE_MACHINE state = STARTING;
inline AT_STATE at_state = AT_EMPTY;
inline TCP_STATE tcp_state = TCP_EMPTY;

#endif // STATE_H