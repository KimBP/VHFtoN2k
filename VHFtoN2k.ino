
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
#include <SPI.h>
#include <NMEA2000_CAN.h>
//#include <N2kMsg.h>
//#include <NMEA2000.h>
#include <N2kMessages.h>
#include <NMEA0183.h>
#include <NMEA0183Msg.h>
#include <NMEA0183Messages.h>
#include <AIS.h>
#include "NMEA0183Handlers.h"
#include "BoatData.h"
#include <mcp_can.h>
#include <NMEA2000_mcp.h>
//
#define NMEA0183SrcId 3

#define N2K_Def_DevId 25

tBoatData BoatData;

 // The object handling NMEA2000 connection
//tNMEA2000 NMEA2000;

// The object handling NMEA0183 connection
tNMEA0183 NMEA0183;

// The latest received NMEA0183Msg
tNMEA0183Msg NMEA0183Msg;

void setup() {

  // Setup NMEA2000 system
  Serial.begin(115200);
  NMEA2000.SetProductInformation("00000111", // Manufacturer's Model serial code
                                 111, // Manufacturer's product code
                                 "VHFtoN2k",  // Manufacturer's Model ID
                                 "1.0.0.1 (2016-10-17)",  // Manufacturer's Software version code
                                 "1.0.0.0 (2015-10-17)" // Manufacturer's Model version
                                 );
  // Device information
  NMEA2000.SetDeviceInformation(111, // Unique number. Use e.g. Serial number.
                                135, // Device function=NMEA 0183 Gateway. See codes on http://www.nmea.org/Assets/20120726%20nmea%202000%20class%20%26%20function%20codes%20v%202.00.pdf
                                25, // Device class=Inter/Intranetwork Device. See codes on http://www.nmea.org/Assets/20120726%20nmea%202000%20class%20%26%20function%20codes%20v%202.00.pdf
                                2046 // Just choosen free from code list on http://www.nmea.org/Assets/20121020%20nmea%202000%20registration%20list.pdf                               
                               );

//  NMEA2000.SetForwardSystemMessages(true);
  NMEA2000.SetMode(tNMEA2000::N2km_NodeOnly,N2K_Def_DevId);
  NMEA2000.EnableForward(false); // Disable all msg forwarding to USB (=Serial)
  NMEA2000.Open();

  // Setup NMEA0183 ports and handlers
  InitNMEA0183Handlers(&NMEA2000, &BoatData);
  NMEA0183.SetMsgHandler(HandleNMEA0183Msg);

  NMEA0183.Begin(&Serial,NMEA0183SrcId, 19200);
}

void loop() {
  NMEA2000.ParseMessages();
  NMEA0183.ParseMessages();

//  SendNMEA2000_AIS();  
}
/*
void SendNMEA2000_AIS(void)
{
  if (!BoatData.updated) return;
  
  tN2kMsg N2kMsg;
  
//  void SetN2kAISClassAPosition(tN2kMsg &N2kMsg, uint8_t MessageID, tN2kAISRepeat Repeat, uint32_t UserID, double Latitude, double Longitude,
//                        bool Accuracy, bool RAIM, uint8_t Seconds, double COG, double SOG, double Heading, double ROT, tN2kAISNavStatus NavStatus)
  // TODO: COnvert format of boatdata - where?
  SetN2kAISClassAPosition (N2kMsg, 
}
*/
