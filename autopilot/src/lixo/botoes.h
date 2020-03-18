/*
 * botoes.h
 *
 *  Created on: 18/11/2017
 *      Author: vitor
 */

#ifndef BOTOES_H_
#define BOTOES_H_

#include "Arduino.h"
#include "OneButton.h"
#include "SeatalkSerial.h"

//**********************************************
// ****************************************************************
#define RX_PIN 7
#define TX_PIN 6
#define INVERSE_LOGIC true

// for seatalk true

SeatalkSerial seaSerial (RX_PIN, TX_PIN, false);

//**********************************************
// ****************************************************************
#define BUT_BB 	A3
#define BUT_AUTO 	A4
#define BUT_ST		A0

OneButton buttonBB = OneButton (BUT_BB, true);
OneButton buttonAUTO = OneButton (BUT_AUTO, true);
OneButton buttonST = OneButton (BUT_ST, true);

uint32_t inicio_longPress;

//**********************************************
// *********************************************************************
enum ButtonState {
	but_STANDBY, but_STANDBY_WIND, but_AUTO,  but_WIND, but_BB_TACK_W, but_BB_TACK_A,
	but_ST_TACK_W,	but_ST_TACK_A
};

enum ButtonState buttonState;

//**********************************************
// ***********************************************************************
#define LED_BB 	A5
#define LED_AUTO 	A2
#define LED_ST		A1

#define LED_PIN  13

enum Leds {
	led_off, led_on, led_toggle
};

enum Leds ledBB;
enum Leds ledAUTO;
enum Leds ledST;

//**********************************************
// *************************************************************************
#define TOGGLE_TIME 200
#define LONG_TIME  10000

//**********************************************
// *********************************************************************
#define LAMP_OFF       0x00
#define LAMP_ON        0x0C
#define PLUS_ONE       0x07
#define MINUS_ONE      0x05
#define PLUS_TEN       0x08
#define MINUS_TEN      0x06
#define STANDBY        0x02
#define AUTO           0x01
#define TRACK          0x03
#define DISP           0x04
#define TACK_MINUS     0x21
#define TACK_PLUS      0x22
#define WIND				0x23

enum Cmd {
	cmd_nulo, cmd_menos10, cmd_menos1, cmd_bb_tack,
	cmd_auto, cmd_standby, cmd_wind,
	cmd_mais10, cmd_mais1, cmd_st_tack
};

bool b_cmdSend;
enum Cmd command;

//**********************************************

//**********************************************
uint8_t sendCmd( uint8_t cData) {
	uint8_t sent = 0;
//	Serial.println(seaSerial.write9s (0x86 | 0x100));

	do {
		seaSerial.checkBus ();
		if (!seaSerial.write9s (0x86  | 0x100))
			continue;
		if (!seaSerial.write9s (0x41)) // 0x21 ST4000  // 0x11 remote Z101  // 0x01 ST1000
			continue;
		if (!seaSerial.write9s (cData))
			continue;
		if (!seaSerial.write9s ((uint8_t)~cData))
			continue;
		else
			sent=1;
	} while (!sent);

	return 1;

}

//**********************************************
void duringLongPressBB() {
	if (millis () - inicio_longPress > LONG_TIME)
		ledBB = led_on;
}

void duringLongPressST() {
	if (millis () - inicio_longPress > LONG_TIME)
		ledST = led_on;

}

// -----------------------------------------------------
void longPressStartBB() {
	ledBB = led_off;
	inicio_longPress = millis ();
}

void longPressStartST() {
	ledST = led_off;
	inicio_longPress = millis ();
}

// ***********************************************
void longPressStopBB() {

	Serial.println ("longPressBB");
	if (buttonState != but_STANDBY) {
		if (millis () > inicio_longPress + LONG_TIME) {
			ledAUTO = led_on;

			if (buttonState == but_AUTO)
				buttonState = but_BB_TACK_A;
			else
				buttonState = but_BB_TACK_W;
		} else
			ledBB = led_toggle;
	}
}

//**********************************************
void longPressStopST() {
	Serial.println ("longPressST");
	if (buttonState != but_STANDBY) {
		if (millis () > inicio_longPress + LONG_TIME) {
			ledST = led_on;

			if (buttonState == but_AUTO)
				buttonState = but_ST_TACK_A;
			else
				buttonState = but_ST_TACK_W;
		} else
			ledBB = led_toggle;

	}
}

// *********************************************
void clickBB() {
	Serial.println ("clickBB");

	if ((buttonState != but_STANDBY) || (buttonState != but_STANDBY_WIND)) {
		b_cmdSend = true;
		command = cmd_menos1;
	} else {
		if (buttonState == but_STANDBY){
			buttonState=but_STANDBY_WIND;
		} else {
			buttonState=but_STANDBY;
		}
	}
}

//**********************************************
void clickST() {
	Serial.println ("clickST");

	if ((buttonState != but_STANDBY) || (buttonState != but_STANDBY_WIND)) {
		b_cmdSend = true;
		command = cmd_mais1;
	} else {
		if (buttonState == but_STANDBY){
			buttonState=but_STANDBY_WIND;
		} else {
			buttonState=but_STANDBY;
		}
	}
}
// *********************************************
void clickAUTO() {
	enum ButtonState st_new;
	Serial.println ("clickAUTO");

	switch (buttonState) {
	case but_AUTO:
		;
	case but_WIND:
		ledBB = led_off;
		ledST = led_off;
		ledAUTO = led_toggle;

		st_new = but_STANDBY;
		command = cmd_standby;
		break;
	case but_STANDBY:
	case but_STANDBY_WIND:
		ledBB = led_toggle;
		ledST = led_toggle;
		ledAUTO = led_toggle;

		st_new = but_AUTO;
		command = cmd_auto;
		break;
	case but_BB_TACK_W:
		ledBB = led_toggle;
		ledST = led_toggle;
		ledAUTO = led_toggle;

		st_new = but_WIND;
		command = cmd_bb_tack;
		break;
	case but_BB_TACK_A:
		ledBB = led_toggle;
		ledST = led_toggle;
		ledAUTO = led_toggle;

		st_new = but_AUTO;
		command = cmd_bb_tack;
		break;

	case but_ST_TACK_W:
		ledBB = led_toggle;
		ledST = led_toggle;
		ledAUTO = led_toggle;

		st_new = but_WIND;
		command = cmd_st_tack;
		break;
	case but_ST_TACK_A:
		ledBB = led_toggle;
		ledST = led_toggle;
		ledAUTO = led_toggle;

		st_new = but_AUTO;
		command = cmd_st_tack;
		break;
	default:
		break;
	}
	b_cmdSend = true;
	buttonState = st_new;
}

//**********************************************
void doubleClickBB() {
	Serial.println ("doubleClickBB");

	if (buttonState != but_STANDBY) {
		b_cmdSend = true;
		command = cmd_menos10;
	}
}

//**********************************************
void doubleClickST() {
	Serial.println ("doubleClickST");

	if (buttonState != but_STANDBY) {
		b_cmdSend = true;
		command = cmd_mais10;
	}
}

//**********************************************
void doubleClickAUTO() {
	Serial.println ("doubleClickAUTO");

	ledBB = led_toggle;
	ledST = led_toggle;
	ledAUTO = led_off;
	buttonState = but_WIND;

	b_cmdSend = true;
	command = cmd_wind;
}

//**********************************************
void beginControl() {

	seaSerial.begin (4800);

	pinMode (LED_BB, OUTPUT);
	pinMode (LED_ST, OUTPUT);
	pinMode (LED_AUTO, OUTPUT);
	digitalWrite (LED_BB, HIGH);
	digitalWrite (LED_ST, HIGH);
	digitalWrite (LED_AUTO, HIGH);
// ------------------------------------
	buttonAUTO.attachClick (clickAUTO);              // click -> auto/standby
	buttonAUTO.attachDoubleClick (doubleClickAUTO);                  // 1º

	buttonBB.attachClick (clickBB);                  // 1º
	buttonBB.attachDoubleClick (doubleClickBB);                  // 10º
	buttonBB.attachLongPressStart (longPressStartBB);
	buttonBB.attachLongPressStop (longPressStopBB);               //alterar lado
	buttonBB.attachDuringLongPress (duringLongPressBB);
	buttonBB.setPressTicks (5000);

	buttonST.attachClick (clickST);                  // 1º
	buttonST.attachDoubleClick (doubleClickST);                  // 10º
	buttonST.attachLongPressStart (longPressStartST);
	buttonST.attachLongPressStop (longPressStopST);               //alterar lado
	buttonST.attachDuringLongPress (duringLongPressST);
	buttonST.setPressTicks (5000);
}

//**********************************************

void ledProcess() {
	static uint32_t antes;
	uint32_t agora = millis ();
	static uint8_t b_toggle;
	if (ledBB == led_on)
		digitalWrite (LED_BB, HIGH);
	if (ledST == led_on)
		digitalWrite (LED_ST, HIGH);
	if (ledAUTO == led_on)
		digitalWrite (LED_AUTO, HIGH);

	if (ledBB == led_off)
		digitalWrite (LED_BB, LOW);
	if (ledST == led_off)
		digitalWrite (LED_ST, LOW);
	if (ledAUTO == led_off)
		digitalWrite (LED_AUTO, LOW);

	if (agora > antes + TOGGLE_TIME) {
		antes = agora;
		b_toggle = b_toggle ? LOW : HIGH;
		if (ledBB == led_toggle) {
			digitalWrite (LED_BB, b_toggle);
		}
		if (ledST == led_toggle) {
			digitalWrite (LED_ST, b_toggle);
		}
		if (ledAUTO == led_toggle) {
			if (buttonState == but_WIND)
				digitalWrite (LED_AUTO, !b_toggle);
			else
				digitalWrite (LED_AUTO, b_toggle);
		}
	}
}

// ********************************************

void orderProcess() {

	buttonBB.tick ();
	buttonST.tick ();
	buttonAUTO.tick ();

	switch (command) {
	case cmd_nulo:
		//Serial.println ("ordem de -10");
		break;
	case cmd_menos10:
		Serial.println ("ordem de -10");
		sendCmd ( MINUS_TEN);
		break;
	case cmd_menos1:
		Serial.println ("ordem de -1");
		sendCmd ( MINUS_ONE);
		break;
	case cmd_bb_tack:
		Serial.println ("ordem de tack to bb");
		sendCmd ( TACK_MINUS);
		break;
	case cmd_auto:
		Serial.println ("ordem de auto");
		sendCmd ( AUTO);
		buttonState = but_AUTO;
		break;
	case cmd_standby:
		Serial.println ("ordem de standby");
		sendCmd ( STANDBY);
		buttonState = but_STANDBY;
		break;
	case cmd_wind:
		Serial.println ("ordem de wind");
		sendCmd ( WIND);
		break;
	case cmd_mais10:
		Serial.println ("ordem de +10");
		sendCmd ( PLUS_TEN);
		break;
	case cmd_mais1:
		Serial.println ("ordem de +1");
		sendCmd ( PLUS_ONE);
		break;
	case cmd_st_tack:
		Serial.println ("ordem de tack to st");
		sendCmd ( TACK_PLUS);
		break;
	}
	command = cmd_nulo;

	ledProcess ();
}

/*
 *
 *
 */

#endif /* SRC_BOTOES_H_ */
