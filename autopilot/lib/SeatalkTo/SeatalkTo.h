#ifndef SEATALK_TO_H
#define SEATALK_TO_H

#include <inttypes.h>
#include "Arduino.h"
#include "SeatalkDef.h"

class SeatalkTo {
 public:
  SeatalkTo() {}
  ~SeatalkTo() {}

 private:
  uint8_t *data;  // uint8_t data[18];

  uint8_t getAttr(enum nibble qual) const;
  uint8_t getCmd() const;
  uint32_t getBytes(uint8_t idx_start, uint8_t qde_bytes) const;
  void addBytes(uint32_t valor, uint8_t qde_bytes, uint8_t loc);

 protected:
  bool setData(uint8_t *msg) { this->data = msg; }
  bool parseDepth(uint16_t &depthFeetx10, uint8_t &alarms) const;
  bool parseApparentWindAngle(uint16_t &anglex2) const;
  bool parseApparentWindSpeed_nc(uint16_t &speedx10);
  bool parseSpeedThroughWater(uint16_t &speedx10);
  bool parseTrip(uint32_t &tripx100);
  bool parseTotal(uint16_t &totalx10);
  bool parseWaterTemperature(int16_t &temp, bool &error);
  bool parseSpeedUnits(uint8_t &units);
  bool parseTotalTripLog(uint16_t &total, uint16_t &trip);
  bool parseSpeedsThroughWater(uint16_t &speedx100, uint16_t &speedMeanx100);
  bool parseWaterTemperature2(uint16_t &tempx10);
  bool parseLampIntensity(uint8_t lamp);
  bool parseMob(bool &mob);
  void geoCoord2GrausMinutos(uint32_t intCord, int16_t &grau,
                             uint16_t &minutosx100);
  bool parseLatitude(uint32_t &latitude);
  bool parseLongitude(uint32_t &longitude);
  bool parseSOG(uint16_t &speedx10);
  bool parseMagneticCOG(uint16_t &coursex8);
  bool parseTime(struct time &tempo);
  bool parseDate(struct dia &hoje);
  bool parseSatInfo(uint8_t &n_sat);
  bool parseLatitude_Longitude(int32_t &latitude, int32_t &longitude);
  bool parseWindAlarms(uint8_t &alarms);
  bool parseAutopilot(uint16_t &compass, bool &turning, uint16_t &course,
                      uint8_t &autopilot_mode, uint8_t &alarms,
                      uint16_t &rudder_position) const;
  bool parseAutopilotCmd(uint8_t &dev, uint8_t &mode) const;
  bool parseAutopilotInfo(struct autopilot_params &autopilot);
  bool parseCompass(uint16_t &compassx8, uint16_t &stear_referencex2,
                    bool &st40_mode) const;
  bool parseDevId(uint8_t &id) const;
  bool parseCompassRudderInfo(uint16_t &compass, bool &turning,
                              int8_t &rudder_position) const;

  void print(Stream *port);
};
#endif  // SEATALK_TO_NMEA_H
