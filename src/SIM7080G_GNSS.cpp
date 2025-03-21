#include "SIM7080G_GNSS.hpp"

void SIM7080G::GNSS::Off()
{
    SIM7080G::SERIAL::send_AT_bloquant("AT+CGNSMOD=0,0,0,0,0");
    SIM7080G::SERIAL::send_AT_bloquant("AT+CGNSPWR=0");
}

void SIM7080G::GNSS::On()
{
    SIM7080G::SERIAL::send_AT_bloquant("AT+CGNSPWR=1");
    SIM7080G::SERIAL::send_AT_bloquant("AT+CGNSMOD=1,0,0,1,0");
}

void SIM7080G::GNSS::Reboot()
{
    Off();
    On();
}

FloatCoordinate::FloatCoordinate(String value)
{
    // Serial.printf("Value: \"%s\"\n", value);

    if (!value || value.length() == 0)
    {
        this->integer = 0;
        this->decimal = 0;
        return;
    }

    size_t count;
    // char **data = string_split(value, '.', &count);
    String integerStr = value.substring(0, value.indexOf('.'));
    String decimalStr = value.substring(value.indexOf('.') + 1);

    if (integerStr.length() > 0 && decimalStr.length() > 0)
    {
        this->integer = atoi(integerStr.c_str());
        this->decimal = atoi(decimalStr.c_str());
    }
}

String FloatCoordinate::toString()
{
    String integerStr = String(integer);
    String decimalStr = String(decimal);

    while (decimalStr.length() < 6)
    {
        decimalStr = "0" + decimalStr;
    };

    return integerStr + "." + decimalStr + String(" (Integer : ") + integerStr + String(" Decimal: ") + decimalStr + String(")");
}

CGNS_INFO SIM7080G::GNSS::GNSS_Info()
{
    String response = SIM7080G::SERIAL::send_AT_bloquant("AT+CGNSINF", true);
    response = response.substring(response.indexOf(":") + 1);

    if (!response || response.length() == 0)
        return CGNS_INFO();

    CGNS_INFO info;

    response = response.substring(response.indexOf(": ") + 2);

    info.gnss_status = atol(response.substring(0, response.indexOf(",")).c_str()) == 1 ? true : false;
    response = response.substring(response.indexOf(",") + 1);
    info.fix_status = atol(response.substring(0, response.indexOf(",")).c_str()) == 1 ? true : false;
    response = response.substring(response.indexOf(",") + 1);
    info.utc_time = DateTime(response.substring(0, response.indexOf(",")));
    response = response.substring(response.indexOf(",") + 1);
    info.latitude = FloatCoordinate(response.substring(0, response.indexOf(",")).c_str());
    response = response.substring(response.indexOf(",") + 1);
    info.longitude = FloatCoordinate(response.substring(0, response.indexOf(",")).c_str());
    response = response.substring(response.indexOf(",") + 1);
    info.altitude = atof(response.substring(0, response.indexOf(",")).c_str());
    response = response.substring(response.indexOf(",") + 1);
    info.speed = atof(response.substring(0, response.indexOf(",")).c_str());
    response = response.substring(response.indexOf(",") + 1);
    info.course = atof(response.c_str());

    return info;
}

DateTime::DateTime(String value)
{

    if (!value || value.length() == 0)
    {
        this->year = 0;
        this->month = 0;
        this->day = 0;
        this->hour = 0;
        this->minute = 0;
        this->second = 0;
        this->millisecond = 0;
        return;
    }

    // size_t count;
    // char **data = string_split(value, '.', &count);

    // if (data != nullptr)
    {
        // char char4[4];
        // char char2[2];

        this->year = atoi(value.substring(0, 4).c_str());
        value = value.substring(4);
        this->month = atoi(value.substring(0, 2).c_str());
        value = value.substring(2);
        this->day = atoi(value.substring(0, 2).c_str());
        value = value.substring(2);
        this->hour = atoi(value.substring(0, 2).c_str());
        value = value.substring(2);
        this->minute = atoi(value.substring(0, 2).c_str());
        value = value.substring(2);
        this->second = atoi(value.substring(0, 2).c_str());
        value = value.substring(2);
    }
}

long long DateTime::toUnixTime()
{
    struct tm t;
    t.tm_year = year - 1900;
    t.tm_mon = month - 1;
    t.tm_mday = day;
    t.tm_hour = hour;
    t.tm_min = minute;
    t.tm_sec = second;

    return mktime(&t);
}

String DateTime::toString()
{
    return String(year) + "-" + String(month) + "-" + String(day) + " " + String(hour) + ":" + String(minute) + ":" + String(second) + "." + String(millisecond) + " (Unix Time: " + String(toUnixTime()) + ")";
};

String CGNS_INFO::toString()
{
    return "GNSS Status: " + String(gnss_status) + "\n" +
           "Fix Status: " + String(fix_status) + "\n" +
           "UTC Time: " + utc_time.toString() + "\n" +
           "Latitude: " + latitude.toString() + "\n" +
           "Longitude: " + longitude.toString() + "\n" +
           "Altitude: " + String(altitude) + "\n" +
           "Speed: " + String(speed) + "\n" +
           "Course: " + String(course);
}