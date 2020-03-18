#ifndef SEATALK_TO_NMEA_H
#define SEATALK_TO_NMEA_H

#include <inttypes.h>
#include "Arduino.h"
#include "SeatalkTo.h"

#define DEPTH 0x00
#define APPARENT_WIND_ANGLE 0x10
#define APPARENT_WIND_SPEED 0x11
#define SPEED_THROUGH_WATER 0x20
#define TRIP 0x21
#define TOTAL 0x22
#define WATER_TEMPERATURE 0x23
#define SPEED_UNITS 0x24
#define TOTAL_TRIP_LOG 0x25
#define SPEEDS_THROUGH_WATER 0x26
#define WATER_TEMPERATURE2 0x27
#define LAMP_INTENSITY 0x30
#define MOB 0x36
#define COORD_TO_GRAUS_MINUTOS 
#define LATITUDE 0x50
#define LONGITUDE 0x51
#define SOG 0x52
#define MAGNETIC_COG 0x53
#define TIME 0x54
#define DATE 0x56
#define SAT_INFO 0x57
#define LATITUDE_LONGITUDE 0x58
#define WIND_ALARMS 0x66
#define AUTOPILOT 0x84
#define AUTOPILOT_CMD 0x86
#define AUTOPILOT_INFO 0x88
#define COMPASS 0x89
#define DEV_ID 0x90
#define COMPASS_RUDDER_INFO 0x9C


class SeatalktoNmea : public SeatalkTo {
 public:
  SeatalktoNmea():SeatalkTo(){}
  ~SeatalktoNmea() {}

  bool toNmea(uint8_t* msg);  
  bool toSeatalk(uint8_t* msg);

 private:
   uint8_t *checksum_NMEA(const uint8_t *input_str,
                          uint8_t *result);

   bool Depth();
   bool AutoPilot();
   bool AutoPilotCmd();
   bool CompassRudderInfo();
   bool DevId(); // bool parseDevId(uint8_t &id) const;
};
#endif  // SEATALK_TO_NMEA_H
