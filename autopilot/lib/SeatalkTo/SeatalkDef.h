/*
N2kDef.h

Copyright (c) 2015-2017 Timo Lappalainen, Kave Oy, www.kave.fi

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Type definitions and utility macros used in the NMEA2000 libraries.

*/

#ifndef _SEATALK_DEF_H_
#define _SEATALK_DEF_H_

#include <stdint.h>

#if !defined(ARDUINO)
extern "C" {
// Application execution delay. Must be implemented by application.
extern void delay(uint32_t ms);

// Current uptime in milliseconds. Must be implemented by application.
extern uint32_t millis();
}
#endif

// Declare PROGMEM macros to nothing on non-AVR targets.
#if !defined(__AVR__) && !defined(ARDUINO)
// ESP8266 provides it's own definition - Do not override it.
#if !defined(ARDUINO_ARCH_ESP8266)
#define PROGMEM
#define pgm_read_byte(var) *var
#define pgm_read_word(var) *var
#define pgm_read_dword(var) *var
#endif
#endif

// Definition for the F(str) macro. On Arduinos use what the framework
// provides to utilize the Stream class. On standard AVR8 we declare
// our own helper class which is handled by the N2kStream. On anything
// else we resort to char strings.
#if defined(ARDUINO)
#include <WString.h>
#elif defined(__AVR__)
#include <avr/pgmspace.h>
class __FlashStringHelper;
#define F(str) (reinterpret_cast<const __FlashStringHelper*>(PSTR(str)))
#else
#ifndef F
#define F(str) str
#endif
#endif

enum StateSeatalkMessage { waiting, with_id, with_len, in_data };
enum nibble { lower, upper, both };

enum SkipperMode { disabled, toCompass, toWind };

typedef struct skipper_t {
  int16_t heading;
  int16_t compass;
  int16_t wind;
  int16_t windRef;
  enum SkipperMode mode;
  bool modeChanged;

} Skipper_t;

#define CALL_MEMBER_FN(object, ptrToMember)  ((object).*(ptrToMember)

struct autopilot_params {
  uint8_t rudder_gain;
  uint8_t counter_rudder;
  uint8_t rudder_limit;
  uint8_t turn_rate_speed;
  uint8_t speed;
  uint8_t off_course_limit;
  uint8_t trim;
  uint8_t power_steer;
  uint8_t drive_type;
  uint8_t rudder_damping;
  uint8_t variations;
  uint8_t auto_adapt;
  uint8_t auto_adapt_latitude;
  uint8_t auto_release;
  uint8_t rudder_alignement;
  uint8_t wind_trim;
  uint8_t response;
  uint8_t boat_type;
  uint8_t cal_lock;
  uint8_t tack_angle;
};

struct Autopilot_t {
  uint16_t compass;
  uint16_t course;
  uint8_t mode;
  int8_t rudder;
  bool turn;
  uint8_t alarm;
  uint8_t display;
};

struct time {
  uint8_t time_h;
  uint8_t time_m;
  uint8_t time_s;
};

struct dia {
  uint8_t date_d;
  uint8_t date_m;
  uint8_t date_y;
};

struct SeatalkBuffer {
  uint8_t id;
  uint8_t len;
  uint8_t buffer[17];
  uint8_t idx;
};

#endif