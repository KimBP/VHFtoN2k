#include "BoatData.h"

void tBoatData::setRead() 
{ 
   updated = false; 
}
  
boolean tBoatData::getRead()
{ 
  return updated; 
}
  
void tBoatData::setSOG(long SOG, double precision) 
{ 
    longToDouble(SOG, &mSOG, precision); 
}
  
double tBoatData::getSOG() 
{ 
  return mSOG; 
}

void tBoatData::setCOG(long COG, double precision) 
{ 
    longToDouble(COG, &mCOG, precision); 
}

double tBoatData::getCOG() 
{ 
  return mCOG; 
}
  
void tBoatData::setHDG(unsigned int HDG) 
{ 
    mHDG = HDG;
    updated = true;
}
double tBoatData::getHDG() { return mHDG; };
  

void tBoatData::setLatitude(long latitude, double precision) 
{
    longToDouble(latitude, &mLatitude, precision); 
}

double tBoatData::getLatitude() 
{ 
  return mLatitude; 
}
  
void tBoatData::setLongitude(long longitude, double precision) 
{ 
    longToDouble(longitude, &mLongitude, precision); 
}

double tBoatData::getLongitude() 
{ 
  return mLongitude; 
}
  
void tBoatData::setMmsi(unsigned long mmsi) 
{   
    mMmsi = mmsi;
    updated = true;
}

unsigned long tBoatData::getMmsi()
{ 
  return mMmsi; 
}
  
void tBoatData::setRepeat(uint8_t repeat)
{
  mRepeat = repeat;
  updated = true;
}

uint8_t tBoatData::getRepeat()
{
  return mRepeat;
}

void tBoatData::setNavStat(uint8_t navStatus)
{
  mNavStatus = navStatus;
  updated = true;
}

uint8_t tBoatData::getNavStat()
{
  return mNavStatus;
}

void tBoatData::setROT(unsigned rot)
{
  mRateOfTurn = rot;
  updated = true;
}

unsigned tBoatData::getROT()
{
  return mRateOfTurn;
}
  
void tBoatData::setAccuracy(uint8_t accuracy)
{
  mAccuracy = accuracy;
  updated = true;
}

uint8_t tBoatData::getAccuracy()
{
  return mAccuracy;
}


void tBoatData::setRAIM(uint8_t Raim)
{
  mRaim = Raim;
  updated = true;
}

uint8_t tBoatData::getRAIM()
{
  return mRaim;
}

void tBoatData::setTimeStampSeconds(uint8_t seconds)
{
  mTimeStampSeconds = seconds;
  updated = true;
}

uint8_t tBoatData::getTimeStampSeconds()
{
  return mTimeStampSeconds;
}

void tBoatData::longToDouble(long inp, double* outp, double precision) 
{
    *outp = inp;
    *outp *= precision;
    updated = true;
}

