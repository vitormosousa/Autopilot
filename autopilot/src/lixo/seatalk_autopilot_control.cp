/* Seatalk AutoPilot Remote Control
 *
 * http://www3.sympatico.ca/ericn/
 *
 This Project was created for the DIY Sailor Electronics Geek.
 After searching the web, I found that several DIY AP remote projects
 required quite a few discrete components. I like to use as many off the
 shelf assemblies as possible.

 The code for the RCSwitch is from http://dzrmo.wordpress.com/2012/07/08/remote-control-pt2272-for-android/
 The code for the Main Program is from http://www.vermontficks.org/pic.htm
 The authoritive Website is located here http://www.thomasknauf.de/seatalk.htm
 All code has been modified to work in this project.

 REMEMBER, USE AT YOUR OWN RISK!

 preTack-ON						preTack->ON
 standby->flash
 auto->ON
 leds					BB_LED		AUTO_LED			ST_BB

 buttons			BB_BUTTON		AUTO_BUTTON		ST_BUTTON
 CLICK			-1/TACK		AUTO/STANDBY		+1/TACK
 DCLICK			-10									+10
 LPRESS				WIND			PREP_TACK		WIND

 */
#include "Arduino.h"
#include "botoes.h"
#include <SPI.h>
#include "RF24.h"



/*
 #define CMD_ST_10 		21772
 #define CMD_ST_1
 #define CMD_BB_10			21763
 #define CMD_BB_1
 #define CMD_BB_TACK		21808
 #define CMD_ST_TACK		21952
 #define CMD_AUTO			21964
 #define CMD_STANDY		21811

 */

int ledPin = 13;                       // LED connected to digital pin 13

//RF24 radio (9, 10);


// **********************************************
void setup() {

	Serial.begin (115200);
	delay (500);
	Serial.println ("Inicio");
	beginControl();
}

// -----------------------------------------------------
void loop()
{
	orderProcess ();

}

