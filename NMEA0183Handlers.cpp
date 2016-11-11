/* 
NMEA0183Handlers.cpp

2015 Copyright (c) Kave Oy, www.kave.fi  All right reserved.

Author: Timo Lappalainen

  This library is free software; you can redistribute it and/or
  modify it as you like.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/
 
#include <N2kMsg.h>
#include <N2kMessages.h>
#include <NMEA0183Messages.h>
#include "NMEA0183Handlers.h"
#include <AIS.h>

typedef struct {
  const char *Code;
  void (*Handler)(const tNMEA0183Msg &NMEA0183Msg); 
} tNMEA0183Handler;

// Predefinition for functions to make it possible for constant definition for NMEA0183Handlers
void HandleVDM(const tNMEA0183Msg &NMEA0183Msg);

// Internal variables
tNMEA2000 *pNMEA2000=0;
tBoatData *pBD=0;


tNMEA0183Handler NMEA0183Handlers[]={
  {"VDM",&HandleVDM},
  {0,0}
};

void InitNMEA0183Handlers(tNMEA2000 *_NMEA2000, tBoatData *_BoatData) {
  pNMEA2000=_NMEA2000;
  pBD=_BoatData;
}


void HandleNMEA0183Msg(const tNMEA0183Msg &NMEA0183Msg) {
  int i;
  // Find handler
  for (i=0; NMEA0183Handlers[i].Code!=0 && !NMEA0183Msg.IsMessageCode(NMEA0183Handlers[i].Code); i++);
  if (NMEA0183Handlers[i].Code!=0) {
    NMEA0183Handlers[i].Handler(NMEA0183Msg); 
  }
}

// NMEA0183 message Handler functions

void HandleVDM(const tNMEA0183Msg &NMEA0183Msg) {
  if (pBD==0) return;
  if (pNMEA2000 == 0) return;
  
  uint8_t pkgCnt;
  uint8_t pkgNmb;
  unsigned int seqMessageId;
  char channel;
  unsigned int length;
  char buf[60];
  unsigned int fillBits;

  length = 40;
  
  if (NMEA0183ParseVDM_nc(
        NMEA0183Msg, 
        pkgCnt, 
        pkgNmb, 
        seqMessageId, 
        channel, 
        length, 
        buf, 
        fillBits)) {

    // Multi packets not supported yet
    if (pkgCnt != 1) return; 
  
    AIS ais_msg(buf, fillBits);

    // Check if supported message
    uint8_t msgType = ais_msg.get_type();
    if ( (msgType != 1) && (msgType != 2) && (msgType != 3)) return;
        
    pBD->setMmsi(ais_msg.get_mmsi());
    pBD->setSOG(ais_msg.get_SOG());
    pBD->setCOG(ais_msg.get_COG());
    pBD->setLatitude(ais_msg.get_latitude());
    pBD->setLongitude(ais_msg.get_longitude());
    pBD->setHDG(ais_msg.get_HDG());
    pBD->setRepeat(ais_msg.get_repeat());
    pBD->setNavStat(ais_msg.get_navStatus());
    pBD->setROT(ais_msg.get_rot());
    pBD->setAccuracy(ais_msg.get_posAccuracy());
    pBD->setRAIM(ais_msg.get_raim());
    
    // Forward message in N2K domain
    // TODO: Act correctly upon Channel - for now always class A
    tN2kMsg N2kMsg;
    SetN2kPGN129038(N2kMsg, 
                    seqMessageId, 
                    static_cast<tN2kAISRepeat>(pBD->getRepeat()),
                    pBD->getMmsi(),
                    pBD->getLongitude(),
                    pBD->getLatitude(),
                    pBD->getAccuracy(),
                    pBD->getRAIM(),
                    pBD->getTimeStampSeconds(),
                    pBD->getCOG(),
                    pBD->getSOG(),
                    pBD->getHDG(),
                    pBD->getROT(),
                    static_cast<tN2kAISNavStatus>(pBD->getNavStat()));
     pNMEA2000->SendMsg(N2kMsg);    
                    
  }
}





