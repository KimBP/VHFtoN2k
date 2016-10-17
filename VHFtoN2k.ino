/*
  VHFtoN2k - converts a few NMEA0183 messages to NMEA2000 format.

  The program is intended to run via an RS422 receiver and a CAN MCP2515 tranceiver.

  The NMEA0183 messages being converted are (planned to be):

  !AIVDM (AIS) -> PGN 129038, AIS Class A Position Report
  XXXX         -> PGN 129808, DSC Call information.

  I.e. messages typically send from a VHF

  The program is based on the NMEA0183ToN2k demo provided by the NMEA0183 library
  
*/

#define USE_N2K_CAN USE_N2K_MCP_CAN
#define N2k_SPI_CS_PIN    9 
#define N2k_CAN_INT_PIN   2  // 9  
#define USE_MCP_CAN_CLOCK_SET 8

#include <Arduino.h>
#include <Time.h>
#include <N2kMsg.h>
#include <NMEA2000.h>
#include <N2kMessages.h>
#include <NMEA0183.h>
#include <NMEA0183Msg.h>
#include <NMEA0183Messages.h>
#include <AIS.h>
//#include "NMEA0183Handlers.h"
//#include "BoatData.h"

#include <due_can.h>  // https://github.com/collin80/due_can
#include <NMEA2000_due.h>
tNMEA2000_due NMEA2000;

#define NMEA0183SourceGPSCompass 3
#define NMEA0183SourceGPS 1

tBoatData BoatData;

tNMEA0183Msg NMEA0183Msg;
tNMEA0183 NMEA0183_3;

void setup() {

  // Setup NMEA2000 system
  Serial.begin(115200);
  NMEA2000.SetProductInformation("00000008", // Manufacturer's Model serial code
                                 107, // Manufacturer's product code
                                 "NMEA0183 -> N2k -> PC",  // Manufacturer's Model ID
                                 "1.0.0.1 (2015-11-18)",  // Manufacturer's Software version code
                                 "1.0.0.0 (2015-11-18)" // Manufacturer's Model version
                                 );
  // Det device information
  NMEA2000.SetDeviceInformation(8, // Unique number. Use e.g. Serial number.
                                130, // Device function=PC Gateway. See codes on http://www.nmea.org/Assets/20120726%20nmea%202000%20class%20%26%20function%20codes%20v%202.00.pdf
                                25, // Device class=Inter/Intranetwork Device. See codes on http://www.nmea.org/Assets/20120726%20nmea%202000%20class%20%26%20function%20codes%20v%202.00.pdf
                                2046 // Just choosen free from code list on http://www.nmea.org/Assets/20121020%20nmea%202000%20registration%20list.pdf                               
                               );

  // NMEA2000.SetForwardType(tNMEA2000::fwdt_Text); // Show in clear text. Leave uncommented for default Actisense format.
  NMEA2000.SetForwardSystemMessages(true);
  NMEA2000.SetMode(tNMEA2000::N2km_ListenAndNode,25);
  //NMEA2000.EnableForward(false);
  NMEA2000.Open();

  // Setup NMEA0183 ports and handlers
  InitNMEA0183Handlers(&NMEA2000, &BoatData);
  NMEA0183_3.SetMsgHandler(HandleNMEA0183Msg);

  NMEA0183_3.Begin(&Serial3,NMEA0183SourceGPSCompass, 19200);
}

void loop() {
  NMEA2000.ParseMessages();
  NMEA0183_3.ParseMessages();
  
  SendSystemTime();
}

#define TimeUpdatePeriod 1000

void SendSystemTime() {
  static unsigned long TimeUpdated=millis();
  tN2kMsg N2kMsg;

  if ( (TimeUpdated+TimeUpdatePeriod<millis()) && BoatData.DaysSince1970>0 ) {
    SetN2kPGNSystemTime(N2kMsg, 0, BoatData.DaysSince1970, BoatData.GPSTime);
    TimeUpdated=millis();
    NMEA2000.SendMsg(N2kMsg);
  }
}


