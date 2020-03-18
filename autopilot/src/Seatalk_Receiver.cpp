/*
 * Seatalk_Receiver.cpp
 *
 *  Created on: 19/11/2017
 *      Author: vitor
 */

/* Seatalk AutoPilot Remote Control
 *
 * http://www3.sympatico.ca/ericn/
 *
 This Project was created for the DIY Sailor Electronics Geek.
 After searching the web, I found that several DIY AP remote projects
 required quite a few discrete components. I like to use as many off the
 shelf assemblies as possible.

 The code for the RCSwitch is from
 http://dzrmo.wordpress.com/2012/07/08/remote-control-pt2272-for-android/ The
 code for the Main Program is from http://www.vermontficks.org/pic.htm The
 authoritive Website is located here http://www.thomasknauf.de/seatalk.htm All
 code has been modified to work in this project.

 REMEMBER, USE AT YOUR OWN RISK!
 */

#include "Arduino.h"

#include "SeatalkDriver.h"
#include "SeatalkToNmea.h"
#include "NMEAParser.h"
#include <stdlib.h>

int ledPin = 13; // LED connected to digital pin 13
#define RXPIN 5
#define TXPIN 4

// const char firstSentence[] = "$RPPAP,41,1,FE*58\r\n";
// const char secondSentence[] = "$IIMWV,339,R,09.52,N,A*24\r\n";

NMEAParser<2> parser;

// for seatalk true
SeatalkDriver seatalk(RXPIN, TXPIN, false);
SeatalktoNmea converter;
char bufNmea[82];
uint8_t bufFromSeatalk[18];
uint16_t bufToSeatalk[18];
uint8_t cnt = 0;

#include "FromNmea.hh"
// **********************************************
void setup()
{
  Serial.begin(115200);
  delay(500);
  Serial.println("Inicio");
  seatalk.begin(4800);

  parser.setErrorHandler(errorHandler);
  parser.addHandler("RPPAP", autoPilotCmd_Handler);
  parser.addHandler("IIMWV", apparentWind_Handler);

}


// -----------------------------------------------------
void loop()
{
  int inChar;
  int inByte;


  while (Serial.available() > 0)
  {
    inByte = Serial.read();
    if (inByte == -1)
    {
      Serial.println("falha");
      break;
    }
    bufNmea[cnt++]=inByte;
    //  Serial.println(inByte);
    if (inByte == 0x0A)
    { // LF
      if (cnt)
      {
        bufNmea[cnt] = '\0';
        Serial.print(bufNmea);
        Serial.print("Parsing : ");

        for (uint8_t i = 0; i < cnt; i++)
        {
          parser << bufNmea[i];
        }
        cnt = 0;
        uint8_t sai = parser.getMessage(bufToSeatalk);
        if (sai)
        {
          // for (uint8_t i = 0; i < sai; i++)
          // {
          //   Serial.print(bufToSeatalk[i], HEX);
          //   Serial.print(" -- ");
          // }          
          // Serial.println();
          seatalk.writeMessage(bufToSeatalk, sai);
        }
      }
      break;
    }
  }



  if (seatalk.getMessage(bufFromSeatalk))
  {
    
    converter.toNmea(bufFromSeatalk);
  }
}
