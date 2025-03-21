#pragma once
#ifndef SIM7080G_GNSS_H
#define SIM7080G_GNSS_H
#include "Utils.hpp"
#include "SIM7080G_SERIAL.hpp"

class FloatCoordinate
{
public:
    short integer;
    long decimal;

    FloatCoordinate() : integer(0), decimal(0) {}
    FloatCoordinate(short integer, long decimal) : integer(integer), decimal(decimal) {}
    FloatCoordinate(String value);

    String toString();
};

class DateTime
{
public:
    short year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    unsigned short millisecond;

    DateTime() : year(0), month(0), day(0), hour(0), minute(0), second(0), millisecond(0) {}
    DateTime(short year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second, unsigned short millisecond) : year(year), month(month), day(day), hour(hour), minute(minute), second(second), millisecond(millisecond) {}
    DateTime(String value);
    ~DateTime() {}

    long long int toUnixTime();
    String toString();
};

struct CGNS_INFO
{
    bool gnss_status;
    bool fix_status;
    DateTime utc_time;
    FloatCoordinate latitude;
    FloatCoordinate longitude;
    double altitude;
    double speed;
    double course;

    String toString();
};

namespace SIM7080G
{
    namespace GNSS
    {
        void On();
        void Off();
        void Reboot();
        CGNS_INFO GNSS_Info();

    } // namespace GNSS

} // namespace SIM7080G
#endif // SIM7080G_GNSS_H