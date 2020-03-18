#include "SeatalkToNmea.h"


bool SeatalktoNmea::toSeatalk(uint8_t* msg){

}

bool SeatalktoNmea::toNmea(uint8_t* msg) {
  setData(msg);
  switch (msg[0]) {
    // case DEPTH:
    //   Depth();
    //   break;
    // case APPARENT_WIND_ANGLE:
    //   ApparentWindAngle();
    //   break;
    // case APPARENT_WIND_SPEED:

    //   break;
    // case SPEED_THROUGH_WATER:

    //   break;
    // case TRIP:

    //   break;
    // case TOTAL:

    //   break;
    // case WATER_TEMPERATURE:

    //   break;
    // case SPEED_UNITS:

    //   break;
    // case TOTAL_TRIP_LOG:

    //   break;
    // case SPEEDS_THROUGH_WATER:

    //   break;
    // case WATER_TEMPERATURE2:

    //   break;
    // case LAMP_INTENSITY:

    //   break;
    // case MOB:

    //   break;
    // case COORD_TO_GRAUS_MINUTOS:

    //   break;
    // case LATITUDE:

    //   break;
    // case LONGITUDE:

    //   break;
    // case SOG:

    //   break;
    // case MAGNETIC_COG:

    //   break;
    // case TIME:

    //   break;
    // case DATE:

    //   break;
    // case SAT_INFO:

    //   break;
    // case LATITUDE_LONGITUDE:

    //   break;
    // case WIND_ALARMS:

    //   break;
    case AUTOPILOT:
      AutoPilot();
      break;
    case AUTOPILOT_CMD:
      AutoPilotCmd();
      break;
    case AUTOPILOT_INFO:
      // Serial.println("AUTOPILOT_INFO");
      break;
    // case COMPASS:
    //   // Serial.println("COMPASS");
    //   break;
    // case DEV_ID:
    //   Serial.println("DEV_ID");
    //   break;
    case COMPASS_RUDDER_INFO:
      CompassRudderInfo();
      break;
    default:
      // Serial.print("Sem conversão id =");
      // Serial.println(msg[0]);
      break;
  }
  return 1;
}

bool SeatalktoNmea::Depth() {
  uint16_t depthFeetx10;
  uint8_t alarms;
  SeatalkTo::parseDepth(depthFeetx10, alarms);
}

bool SeatalktoNmea::AutoPilot() {
  char buf[82];
  uint16_t compass;
  bool turning;
  uint16_t course;
  uint8_t autopilot_mode;
  uint8_t alarms;
  uint16_t rudder_position;
  parseAutopilot(compass, turning, course, autopilot_mode, alarms,
                 rudder_position);
  sprintf(buf, "$STCCM,%d,%d,%d,%c", compass, course, autopilot_mode,'*');
  char ck[3];
  checksum_NMEA((const uint8_t *)buf, (uint8_t *)ck);
  strcat(buf, ck);
  strcat(buf, "\r\n");
  Serial.print(buf); // TODO
  return 1;
}

bool SeatalktoNmea::AutoPilotCmd() {
  uint8_t dev;
  uint8_t mode;
  parseAutopilotCmd(dev, mode);
  char buf[82];
  sprintf(buf, "$STCMD,%d,%d,%c", dev, mode,'*');
  char ck[3];
  checksum_NMEA((const uint8_t *)buf, (uint8_t *)ck);
  strcat(buf, ck);
  strcat(buf, "\r\n");
  Serial.print(buf); // TODO
  return 1;
}

bool SeatalktoNmea::CompassRudderInfo() {
  uint16_t compass;
  bool turning;
  int8_t rudder_position;

  parseCompassRudderInfo(compass, turning, rudder_position);
  char buf[82];
  sprintf(buf, "$STCTR,%d,%d,%d,%c", compass, turning, rudder_position,'*');
  char ck[3];
  checksum_NMEA( (const uint8_t*)buf, (uint8_t*)ck);
  strcat (buf,ck);
  strcat(buf,"\r\n");
  Serial.print(buf);   // TODO
  return 1;
}

uint8_t* SeatalktoNmea::checksum_NMEA(const uint8_t *input_str,
                                            uint8_t *result)
{
  const uint8_t *ptr;
  uint8_t checksum;
  if (input_str == NULL) return NULL;
  if (result == NULL) return NULL;
  checksum = 0;
  ptr = (const unsigned char *)input_str;
  if (*ptr == '$') ptr++;
  while (*ptr && *ptr != '\r' && *ptr != '\n' && *ptr != '*')
    checksum ^= *ptr++;
  snprintf((char *)result, 3, "%02hhX", checksum);
  return result;

} /* checksum_NMEA */



// int nmea0183_checksum(char *nmea_data)
// {
//   int crc = 0;
//   int i;

//   // the first $ sign and the last two bytes of original CRC + the * sign
//   for (i = 1; i < strlen(nmea_data) - 3; i++)
//   {
//     crc ^= nmea_data[i];
//   }

//   return crc;
// }