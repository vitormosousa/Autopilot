
/*
 * SeatalkTo.cpp
 *
 *  Created on: 14/11/2018
 *      Author: vitor
 */

#include "SeatalkTo.h"

// +++++++++++++++++++       SeatalkTo        ++++++++++++++++++++++

// *************************************************************
// non blocking reading - temp storage rxBuffer - final sent to data
// bool SeatalkTo::getMessage() {
//   if (!available()) return false;
//   while (available()) {
//     uint16_t last = read();
//     //		Serial.print ("last : ");
//     //		Serial.println (last, HEX);

//     switch (stateSeatalkMsg) {
//       case waiting:
//         if ((last & 0x100) >> 8) {
//           stateSeatalkMsg = with_id;
//           rxBuf.id = last & 0xFF;
//           rxBuf.buffer[0] = rxBuf.id;
//         }
//         break;
//       case with_id:
//         rxBuf.len = ((uint8_t)(last & 0x0f)) + 3;
//         rxBuf.buffer[1] = (uint8_t)last;
//         stateSeatalkMsg = with_len;
//         rxBuf.idx = 2;
//         break;
//       case with_len:
//         rxBuf.buffer[rxBuf.idx++] = (uint8_t)last;
//         stateSeatalkMsg = in_data;
//         break;
//       case in_data:
//         rxBuf.buffer[rxBuf.idx++] = (uint8_t)last;
//         if (rxBuf.idx > rxBuf.len) {
//           stateSeatalkMsg = waiting;
//           setRxData(rxBuf.buffer, rxBuf.len);

//           for (int i = 0; i < rxBuf.len; i++) {
//             Serial.print(" : ");
//             Serial.print(rxBuf.buffer[i], HEX);
//           }
//           Serial.println();
//           return true;
//         }
//         break;
//     }
//   }
//   return false;
// }

// bool SeatalkTo::sendMessage(uint16_t* buf, uint8_t len) {
//   print9(buf, len);
//   return true;
// }

// //*****************************************************************************
// void SeatalkTo::clear() {
//   data[0] = 0;  // Sender is empty
//   data[1] = 0;  // Message code is empty
// }

// *********************************************************
uint8_t SeatalkTo::getAttr(enum nibble qual) const {
  ;  // 0 - lower nibble 1 - upper 2- both
  uint8_t ret;
  switch (qual) {
    case 0:
      ret = data[1] & 0x0f;
      break;
    case 1:
      ret = (data[1] >> 4) & 0xf;
      break;
    case 2:
      ret = data[1];
      break;
  }
  return ret;
}

// *********************************************************
uint8_t SeatalkTo::getCmd() const { return data[0]; }

// *********************************************************
uint32_t SeatalkTo::getBytes(uint8_t idx_start, uint8_t qde_bytes) const {
  uint32_t x = data[idx_start];

  for (uint8_t idx = 1; idx < qde_bytes; idx++) {
    x >>= 8;
    x |= ((uint32_t)data[idx_start + idx]) << 8;
  }
  return x;
}

// *********************************************************
void SeatalkTo::addBytes(uint32_t valor, uint8_t qde_bytes, uint8_t loc) {
  data[loc] = (uint8_t)valor;
  for (uint8_t idx = 1; idx < qde_bytes; idx++) {
    valor = valor >> 8;
    data[++loc] = (uint8_t)valor;
  }
}

//*****************************************************************************
void SeatalkTo::print(Stream* port) {
  if (port == 0) return;
  uint8_t len = getAttr(lower) + 3;
  port->print(F("Cmd:"));
  for (int i = 0; i < len; i++) {
    port->print(data[i], HEX);
    port->print(F(" "));
  }

  port->println(F(""));
}

// 0x00
// ****************************************************

bool SeatalkTo::parseDepth(uint16_t& depthFeetx10, uint8_t& alarms) const {
  if (getAttr(lower) == 2) {
    uint8_t y, z;
    uint32_t x;

    y = z = (uint8_t)getBytes(2, 1);
    y = (y >> 4);
    z = (z & 0xf);
    x = getBytes(3, 2);  // em metros -- 1 ft = 0.3048 mt
    /*
     if ( y & 0b1000 )
     alarms |= ALARM_ANCHOR_ALARAM ;
     ; //Serial.write("Anchor Alarm is active\n");
     if ( y & 0b0100 )
     alarms |= ALARM_METRIC_DISPLAY ; //Serial.write("Metric display units \n");
     if ( y & 0b0010 )
     alarms |= ALARAM_UNUSED ; //Serial.write("Used, unknown meaning\n");

     if ( x & 0b100 )
     alarms |= ALARM_TRANSDUCTER_DEFECTIVE ; //Serial.write("Transducer
     defective\n"); if ( x & 0b010 ) alarms |= ALARM_DEPTH ;
     //Serial.write("Deep Alarm is active\n"); if ( x & 0b001 ) alarms |=
     ALARM_SHALLOW_DEPTH ; //Serial.write("Shallow Depth Alarm is active\n");
     */
    return 1;
  }
  return 0;
}

//	case 0x10:
// ****************************************************
bool SeatalkTo::parseApparentWindAngle(uint16_t& anglex2) const {
  if (getAttr(lower) == 1) {
    uint16_t x = getBytes(2, 2);
    anglex2 =
        x * 10 / 2;  // print_float("Apparent Wind Angle:",((float)x)/2.0, LF);
    return 1;
  }
  return 0;
}

// void SeatalkTo::setApparentWindAngle(int16_t angle) {
//   clear();
//   addBytes(0x10, 1, 0);
//   addBytes(0x01, 1, 1);
//   addBytes(angle * 2, 2, 2);
// }

// case 0x11:		speed = x10Knots
// ****************************************************
bool SeatalkTo::parseApparentWindSpeed_nc(uint16_t& speedx10) {
  if (getAttr(lower) == 1) {
    uint16_t a = (getBytes(2, 1) & 0x7f) * 10 + getBytes(3, 1);
    return 1;
  }
  return 0;
}

// void SeatalkTo::setApparentWindSpeed(int16_t speed) {
//   clear();
//   addBytes(0x11, 1, 0);
//   addBytes(0x01, 1, 1);
//   speed *= 10;
//   uint8_t y = (uint8_t)speed / 10;
//   addBytes(speed - y, 1, 2);
//   addBytes(y, 1, 3);

//   // msg.addInt()
// }

// case 0x20:		speed = x10Knots
// ****************************************************
bool SeatalkTo::parseSpeedThroughWater(uint16_t& speedx10) {
  if (getAttr(lower) == 1) {
    speedx10 = getBytes(2, 2);
    return 1;
  }
  return 0;
}

// case 0x21:		x100 nautical miles
// ****************************************************
bool SeatalkTo::parseTrip(uint32_t& tripx100) {
  if (getAttr(lower) == 1) {
    tripx100 = (getBytes(2, 2) << 4) || (getBytes(4, 1) & 0x0f);
    return 1;
  }
  return 0;
}

// case 0x22:		x10 nautical miles
// ****************************************************
bool SeatalkTo::parseTotal(uint16_t& totalx10) {
  if (getAttr(lower) == 2) {
    totalx10 = getBytes(2, 2);
  }
  return 0;
}

// case 0x23:			graus centigrados
// ****************************************************
bool SeatalkTo::parseWaterTemperature(int16_t& temp, bool& error) {
  if (getAttr(lower) == 2) {
    if (getAttr(upper) | 0x4) {
      error = true;
      ;  // write("Water temperature sensor disconnected\n");
    } else {
      error = false;
      temp = getBytes(2, 1);
    }
    return 1;
  }
  return 0;
}

// case 0x24:
// ****************************************************
bool SeatalkTo::parseSpeedUnits(uint8_t& units) {
  if (getAttr(lower) == 2) {
    uint8_t a = getBytes(4, 1);
    /*
     if ( !a )
     units = UNIT_NM ; //print_strs("Milage and speed dislpayed in",  NM,"/",
     KNOTS,LF); if ( a == 0x06 ) units = UNIT_SM ; //print_strs("Milage and
     speed dislpayed in ",SM,"/",MPH,LF); if ( a == 0x86 ) units = UNIT_KM ;
     //print_strs("Milage and speed dislpayed in ",KM,"/",KMH,LF);
     */
    return 1;
  }
  return 0;
}

// case 0x25:
// ****************************************************
bool SeatalkTo::parseTotalTripLog(uint16_t& total, uint16_t& trip) {
  if (getAttr(lower) == 4) {
    uint8_t z = getAttr(upper), x = getBytes(2, 1), y = getBytes(3, 1),
            u = getBytes(4, 1), v = getBytes(5, 1), w = getBytes(6, 1) & 0x0f;

    total = (x + y * 256 + z * 4096);
    trip = (u + v * 256 + w * 65536);
    return 1;
  }
  return 0;
}

// case 0x26:  x100 knots
// ****************************************************
bool SeatalkTo::parseSpeedsThroughWater(uint16_t& speedx100,
                                        uint16_t& speedMeanx100) {
  if (getAttr(lower) == 4) {
    speedx100 = getBytes(2, 2);
    speedMeanx100 = getBytes(4, 2);

    // uint8_t ee = msg.getBytes ( 6, 1 ) ; TODO
    return 1;
  }
  return 0;
}

// case 0x27:
// ****************************************************
bool SeatalkTo::parseWaterTemperature2(uint16_t& tempx10) {
  if (getAttr(lower) == 1) {
    tempx10 = getBytes(2, 2) - 100;
    return 1;
  }
  return 0;
}

// case 0x30:
// ****************************************************
bool SeatalkTo::parseLampIntensity(uint8_t lamp) {
  if (getAttr(lower) == 0) {
    uint8_t a = getBytes(2, 1) & 0x0f;
    if (!a) lamp = 0;        // Serial.write("LO\n");
    if (a == 4) lamp = 1;    // Serial.write("L1\n");
    if (a == 8) lamp = 2;    // Serial.write("L2\n");
    if (a == 0xC) lamp = 3;  // Serial.write("L3\n");
    return 1;
  }
  return 0;
}

// case 0x36:
// ****************************************************
bool SeatalkTo::parseMob(bool& mob) {
  if (getAttr(lower) == 0) {
    mob = false;
    return 1;
  }
  return 0;
}

void SeatalkTo::geoCoord2GrausMinutos(uint32_t intCord, int16_t& grau,
                                      uint16_t& minutosx100) {
  grau = intCord >> 16;
  minutosx100 = ((uint16_t)intCord) & 0x7fff;
  if (((uint16_t)intCord) & 0x8000) {
    grau = -grau;
  }
}

// case 0x50:
// ****************************************************
bool SeatalkTo::parseLatitude(uint32_t& latitude) {
  if (getAttr(lower) == 2) {
    uint16_t x = getBytes(2, 1) << 16, latitude = x || +getBytes(3, 2);
    return 1;
  }
  return 0;
}

// case 0x51:
// ****************************************************
bool SeatalkTo::parseLongitude(uint32_t& longitude) {
  if (getAttr(lower) == 2) {
    uint16_t x = getBytes(2, 1) << 16, longitude = x || +getBytes(3, 2);
    return 1;
  }
  return 0;
}

// case 0x52:
// ****************************************************
bool SeatalkTo::parseSOG(uint16_t& speedx10) {
  if (getAttr(lower) == 1) {
    speedx10 = getBytes(2, 2);
    return 1;
  }
  return 0;
}

// case 0x53:
// ****************************************************
bool SeatalkTo::parseMagneticCOG(uint16_t& coursex8) {
  if (getAttr(lower) == 0) {
    uint8_t u = getAttr(upper), v = getBytes(2, 1);
    coursex8 = ((u & 0x3) * 90 + (v & 0x3F) * 2 * 8 + (u & 0xC));
    return 1;
  }
  return 0;
}

// case 0x54:
// ****************************************************
bool SeatalkTo::parseTime(struct time& tempo) {
  if (getAttr(lower) == 1) {
    uint8_t t = getAttr(upper), rs = getBytes(2, 1), h = getBytes(3, 1);
    uint16_t rst = (rs << 4) | t;
    tempo.time_h = h;
    tempo.time_m = ((rs)&0xfc) >> 2;
    tempo.time_s = ((rst)&0x3f);
    return 1;
  }
  return 0;
}

// case 0x56:
// ****************************************************
bool SeatalkTo::parseDate(struct dia& hoje) {
  if (getAttr(lower) == 1) {
    uint8_t m = getAttr(upper), d = getBytes(2, 1), y = getBytes(3, 1);
    hoje.date_d = d;
    hoje.date_m = m;
    hoje.date_y = y;
    return 1;
  }
  return 0;
}

// case 0x57:
// ****************************************************
bool SeatalkTo::parseSatInfo(uint8_t& n_sat) {
  if (getAttr(lower) == 1) {
    n_sat = getAttr(upper);
    return 1;
  }
  return 0;
}

// case 0x58:
// ****************************************************
bool SeatalkTo::parseLatitude_Longitude(int32_t& latitude, int32_t& longitude) {
  if (getAttr(lower) == 5) {
    uint8_t z = getAttr(upper), la = getBytes(2, 1), x = getBytes(3, 1),
            y = getBytes(4, 1), lo = getBytes(5, 1), q = getBytes(6, 1),
            r = getBytes(7, 1);
    uint32_t pos = (((uint32_t)la) << 16) + ((((uint16_t)x) << 8) + y);
    latitude = ((z & 1) ? (-pos) : (pos));

    pos = (((uint32_t)lo) << 16) + ((((uint16_t)q) << 8) + r);
    longitude = ((z & 2) ? (-pos) : (pos));
    return 1;
  }
  return 0;
}

// case 0x59:
/*
 bool seatalkParseTimeCountDown_nc(const msg &msg, tBOD &bod) {
 if (msg.get_4(3) == 2 && msg.get_4(3) == 0
 && msg.get_4(3) == 0xA) {
 ; //Serial.write("It lefts 10 seconds (ST60 contdown)\n");
 }
 return 0;
 }
 */

// case 0x66:
// ****************************************************
bool SeatalkTo::parseWindAlarms(uint8_t& alarms) {
  if (getAttr(lower) == 0) {
    alarms = getBytes(2, 1);
    return 1;
  }
  return 0;
}

// case 0x84:
// ****************************************************
bool SeatalkTo::parseAutopilot(uint16_t& compass, bool& turning,
                               uint16_t& course, uint8_t& autopilot_mode,
                               uint8_t& alarms,
                               uint16_t& rudder_position) const {
  if (getAttr(lower) == 6) {
    uint8_t u = getAttr(upper), vw = getBytes(2, 1), xy = getBytes(3, 1),
            z = getBytes(4, 1), m = getBytes(5, 1), r = getBytes(6, 1),
            s = getBytes(7, 1), t = getBytes(8, 1);

    // compass heading
    compass = (u & 0x3) * 90 + (vw & 0x3f) * 2 +
              (u & 0xc ? ((u & 0xc) == 0xc ? 2 : 1) : 0);
    // print_int("Compass heading: ",ch," degres\n");

    turning = u & 0x8;

    // autopilot course
    course = (vw >> 6) * 90 + xy / 2;

    autopilot_mode = z;

    // alarm
    /*
     if ( m & 0x4 )
     alarms |= ALARM_OFF_COURSE ;
     if ( m & 0x8 )
     alarms |= ALARM_WIND_SHIFT ;
     */
    rudder_position = r;

    //		Serial.print("compass :");
    //		Serial.println(compass );
    //
    //		Serial.print("autopilot mode :");
    //		Serial.println(autopilot_mode );

    return 1;
  }
  return 0;
}

// case 0x86
bool SeatalkTo::parseAutopilotCmd(uint8_t& dev, uint8_t& mode) const {
  if (getAttr(lower) == 1) {
    dev = getAttr(upper);
    mode = getBytes(2, 1);
    return true;
  }
  return false;
}

// case 0x88:
// ****************************************************
bool SeatalkTo::parseAutopilotInfo(struct autopilot_params& autopilot) {
  if (getAttr(lower) == 3) {
    uint8_t w = getBytes(2, 1), x = getBytes(3, 1), y = getBytes(4, 1),
            z = getBytes(5, 1);
    switch (w) {
      case 1:
        autopilot.rudder_gain = x;
        break;
      case 2:
        autopilot.counter_rudder = x;
        break;
      case 3:
        autopilot.rudder_limit = x;
        break;
      case 4:
        autopilot.turn_rate_speed = x;
        break;
      case 5:
        autopilot.speed = x;
        break;
      case 6:
        autopilot.off_course_limit = x;
        break;
      case 7:
        autopilot.trim = x;
        break;
      case 9:
        autopilot.power_steer = x;
        break;
      case 0XA:
        autopilot.drive_type = x;
        break;
      case 0xB:
        autopilot.rudder_damping = x;
        break;
      case 0xC:
        autopilot.variations = (int8_t)x;
        break;
      case 0xD:
        autopilot.auto_adapt = x;
        break;
      case 0xE:
        autopilot.auto_adapt_latitude = x;
        break;
      case 0xF:
        autopilot.auto_release = x != 0;
        break;
      case 0x10:
        autopilot.rudder_alignement = x;
        break;
      case 0x11:
        autopilot.wind_trim = x;
        break;
      case 0x12:
        autopilot.response = x;
        break;
      case 0x13:
        autopilot.boat_type = x;
        break;
      case 0x15:
        autopilot.cal_lock = x != 0;
        break;
      case 0x1D:
        autopilot.tack_angle = x;
        break;
    }
    return 1;
  }
  return 0;
}

// case 0x89:
// ****************************************************
bool SeatalkTo::parseCompass(uint16_t& compassx8, uint16_t& stear_referencex2,
                             bool& st40_mode) const {
  if (getAttr(lower) == 2) {
    uint8_t u = getAttr(upper), vw = getBytes(2, 1), xy = getBytes(3, 1),
            z = getBytes(4, 1) & 0x0F;
    compassx8 = ((uint16_t)u & 0x3) * 90 * 8 + (vw & 0x3F) * 16 + (u & 0xC);

    stear_referencex2 = (vw & 0xc0) * 90 * 2 + xy;
    st40_mode = (z & 2);
    return 1;
  }
  return 0;
}

// case 0x90:
// ****************************************************
bool SeatalkTo::parseDevId(uint8_t& id) const {
  if (getAttr(lower) == 0) {
    id = getBytes(2, 1);
    return 1;
  }
  return 0;
}

// case 0x9C:
// ****************************************************
bool SeatalkTo::parseCompassRudderInfo(uint16_t& compass, bool& turning,
                                       int8_t& rudder_position) const {
  if (getAttr(lower) == 1) {
    uint8_t u = getAttr(upper), vw = getBytes(2, 1), r = getBytes(3, 1);

    compass = (u & 0x3) * 90 + (vw & 0x3f) * 2 +
              (u & 0xc ? ((u & 0xc) == 0xc ? 2 : 1) : 0);
    turning = u & 0x8;
    rudder_position = r & 0x80;
    return 1;
  }
  return 0;
}

// ----------------------------------------------------
// void SeatalkTo::hDepth(Skipper_t& sk) {  // 0x00
//   uint16_t depthFeetx10;
//   uint8_t alarms;

//   parseDepth(depthFeetx10, alarms);

//   //	Serial.print(F("Depth "));
//   //	Serial.print(depthFeetx10);
//   //	Serial.print(F("  Alarms  "));
//   //	Serial.println(alarms);
// }

// void SeatalkTo::hAutopilot(Skipper_t& sk) {  // 0x84
//   uint16_t compass;
//   bool turning;
//   uint16_t course;
//   uint8_t autopilot_mode;
//   uint8_t alarms;
//   uint16_t rudder_position;

//   parseAutopilot(compass, turning, course, autopilot_mode, alarms,
//                  rudder_position);
//   sk.compass = compass;
//   if (autopilot_mode != 0)
//     sk.mode = disabled;
//   else if (sk.mode == disabled) {
//     sk.mode = toCompass;
//     sk.heading = course;
//   }

//   //	Serial.print(F("Autopilot compass "));
//   //	Serial.print(compass);
//   //	Serial.print(F("  turning "));
//   //	Serial.print(turning);
//   //	Serial.print(F("  course "));
//   //	Serial.print(course);
//   //	Serial.print(F("  autopilot mode "));
//   //	Serial.print(autopilot_mode);
//   //	Serial.print(F("  alarms "));
//   //	Serial.print(alarms);
//   //	Serial.print(F("  rudder position "));
//   //	Serial.println(rudder_position);
// }

// void SeatalkTo::hApparentWindAngle(Skipper_t& sk) {
//   uint16_t anglex2;
//   parseApparentWindAngle(anglex2);
//   sk.wind = anglex2 / 2;
//   //	Serial.print(F("Aparente Wind angle "));
//   //	Serial.println(anglex2);
// }

// void SeatalkTo::hDeviceId(Skipper_t& sk) {  // 0x90
//   uint8_t id;

//   parseDevId(id);

//   //	Serial.print(F("device id "));
//   //	Serial.println(id);
// }

// void SeatalkTo::hCompassRudder(Skipper_t& sk) {  // 0x9c
//   uint16_t compass;
//   bool turning;
//   int8_t rudder_position;

//   parseCompassRudderInfo(compass, turning, rudder_position);
//   sk.compass = compass;
//   //	Serial.print(F("Compass "));
//   //	Serial.print(compass);
//   //	Serial.print(F("  turning "));
//   //	Serial.print(turning);
//   //	Serial.print(F("  rudder position "));
//   //	Serial.println(rudder_position);
// }

// void SeatalkTo::hCompass(Skipper_t& sk) {  // 0x9c
//   uint16_t compassx8;
//   uint16_t stear_referencex2;
//   bool st40_mode;

//   parseCompass(compassx8, stear_referencex2, st40_mode);
//   sk.heading = stear_referencex2 / 2;
//   sk.compass = compassx8 / 8;
//   //	Serial.print(F("Compassx8 "));
//   //	Serial.print(compassx8);
//   //	Serial.print(F("  stear_referencex2 "));
//   //	Serial.print(stear_referencex2);
//   //	Serial.print(F("  st40_mode "));
//   //	Serial.println(st40_mode);
// }

// void SeatalkTo::hAutopilotCmd(Skipper_t& sk) {
//   uint8_t dev;
//   uint8_t mode;
//   parseAutopilotCmd(dev, mode);
//   //	Serial.print(F("AutopilotCmd dev "));
//   //	Serial.print(dev);
//   //	Serial.print(F("  mode "));
//   //	Serial.println(mode);
// }

// //**************************************************************
// void SeatalkTo::sendAutopilotCmd(uint8_t dev, uint8_t mode) {
//   //	uint8_t c_mode = ~mode;
//   //	write ( (uint16_t)0x186) ;
//   //	write ( (uint16_t)((dev << 4) | 0x01) ) ;
//   //	write ( (uint16_t)mode ) ;
//   //	write ( (uint16_t)c_mode ) ;

//   //	uint8_t c_mode = ~mode;
//   //	write9((uint16_t) 0x186);
//   //	write9((uint16_t) 0x41);
//   //	write9((uint16_t) 0x1);
//   //	write9((uint16_t) 0xFE);

//   //	Serial.print("sendAutopilotCmd ");
//   //	Serial.print(0x186, HEX);
//   //	Serial.print(" ");
//   //	Serial.print((dev << 4) | 0x01, HEX);
//   //	Serial.print(" ");
//   //	Serial.print(mode, HEX);
//   //	Serial.print(" ");
//   //	Serial.println(c_mode, HEX);
// }

// void SeatalkTo::setSeatalkHandler(SeatalkHandler_st* seatalkHandler) {
//   this->seatalkHandler = seatalkHandler;
// }

// // ********************************************************************
// bool SeatalkTo::handleSeatalkMsg(char* buf) {
//   bool res = false;
//   if (getMessage()) {
//     uint8_t iHandler;
//     uint8_t cmd = getCmd();

//     // Find handler
//     for (iHandler = 0; (seatalkHandler[iHandler].cmd != 0) &&
//                        (cmd != seatalkHandler[iHandler].cmd);
//          iHandler++)
//       ;
//     //		Serial.print("Cmd : ");
//     //		Serial.print(cmd, HEX);
//     //		Serial.print("  Handler : ");
//     //		Serial.println(iHandler);
//     switch (seatalkHandler[iHandler].switchHandler) {
//       case 1:  // 0x84
//         uint16_t compass;
//         bool turning;
//         uint16_t course;
//         uint8_t autopilot_mode;
//         uint8_t alarms;
//         uint16_t rudder_position;
//         parseAutopilot(compass, turning, course, autopilot_mode, alarms,
//                        rudder_position);
//         sprintf(buf, "$STPAP,%d,%d,%d", compass, course, autopilot_mode);
//         //				Serial.println(buf);
//         // TODO
//         // pNmea->sendMessage(buf);//  $--PAS,[SAW],x*hh<CR><LF> autopilot
//         state res = true; break;
//       case 2:
//         break;
//       default:
//         break;
//     }
//   }
//   return res;
// }

//// ********************************************************************
// void SeatalkTo::handleSeatalkMsg() {
//	int iHandler;
//	uint8_t cmd = getCmd();
//	char buf[81];
//	Serial.print(cmd, HEX);
//	Serial.print(F("   "));
//	// Find handler
//	for (iHandler = 0;
//			seatalkHandler[iHandler].cmd != 0xff
//					&& !(cmd ==
//seatalkHandler[iHandler].cmd); iHandler++)
//		;
//	switch(seatalkHandler[iHandler].switchHandler){
//	case 0:
//		break;
//	case 1:					// 0x84
//		uint16_t compass;
//		bool turning;
//		uint16_t course;
//		uint8_t autopilot_mode;
//		uint8_t alarms;
//		uint16_t rudder_position;
//		 parseAutopilot(  compass, turning,  course, autopilot_mode,
// alarms,  rudder_position); 		 sprintf(buf, "$STPAP,%d,%d,%c",
// compass, course,autopilot_mode);
//
//		 pNmea->sendMessage (buf);//  $--PAS,[SAW],x*hh<CR><LF>
//autopilot state
//
//		break;
//	case 2:
//		break;
//	default:
//		break;
//	}
//}
