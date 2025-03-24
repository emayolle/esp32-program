#include <Arduino.h>
#include <nlohmann/json.hpp>
#include "Utils.hpp"
#include "SIM7080G_FSM.hpp"
#include "SIM7080G_SERIAL.hpp"
#include "SIM7080G_POWER.hpp"
#include "SIM7080G_GNSS.hpp"
#include "SIM7080G_CATM1.hpp"

const uint32_t BAUD_RATE = 115200;

void setup()
{
	// Initialize pins
	pinMode(PWR_KEY, OUTPUT);

	// Initialize serial communication
	Serial.begin(BAUD_RATE);
	Serial.println("[+] Start Up");

	// Initialize SIM7080G module
	Sim7080G.begin(Sim7080G_BAUDRATE, SERIAL_8N1, RX0, TX0);
	Sim7080G.flush(true);

	// Set initial state
	SET_STATE(STARTING);
}

void loop()
{
	// Run finite state machine
	SIM7080G::FSM();
}