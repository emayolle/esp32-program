#include <Arduino.h>
#include <nlohmann/json.hpp>
#include "Utils.hpp"
#include "SIM7080G_FSM.hpp"
#include "SIM7080G_SERIAL.hpp"
#include "SIM7080G_POWER.hpp"
#include "SIM7080G_GNSS.hpp"
#include "SIM7080G_CATM1.hpp"

#define PARAMS(...) __VA_ARGS__

uint32_t last_time = 0;

void setup()
{
	pinMode(PWR_KEY, OUTPUT);

	Serial.begin(115200);
	Serial.println("[+] Start Up");

	Sim7080G.begin(Sim7080G_BAUDRATE, SERIAL_8N1, RX0, TX0);
	Sim7080G.flush(true);

	SET_STATE(STARTING);
	last_time = millis();
}

void loop()
{
	SIM7080G::FSM();
}