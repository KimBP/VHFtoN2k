#ifndef _BoatData_H_
#define _BoatData_H_

#include <Arduino.h>

class tBoatData {

private:
  
  double mSOG, mCOG;
  double mLatitude, mLongitude;
  unsigned long mMmsi;
  unsigned int mHDG;
  uint8_t mAccuracy;
  uint8_t mNavStatus;
  uint8_t mRepeat;
  unsigned mRateOfTurn;
  uint8_t mTimeStampSeconds;
  uint8_t mRaim;
  boolean updated;
  
public:
  tBoatData()
  : mSOG(0), mCOG(0),
  mLatitude(0), mLongitude(0),
  mMmsi(0), mHDG(0),
  mAccuracy(0), mNavStatus(0),
  mRepeat(0), mRateOfTurn(0),
  mTimeStampSeconds(0),
  updated(false) {};

  void setRead();
  boolean getRead();
  
  void setSOG(long SOG, double precision=1e-01);
  double getSOG();

  void setCOG(long COG, double precision=1e-01);
  double getCOG();
  
  void setHDG(unsigned int HDG);
  double getHDG();
  

  void setLatitude(long latitude, double precision=1e-04);
  double getLatitude();
  
  void setLongitude(long longitude, double precision=1e-04);
  double getLongitude();
  
  void setMmsi(unsigned long mmsi);
  unsigned long getMmsi();

  void setRepeat(uint8_t repeat);
  uint8_t getRepeat();
  
  void setNavStat(uint8_t navStatus);
  uint8_t getNavStat();

  void setROT(unsigned rot);
  unsigned getROT();
  
  void setAccuracy(uint8_t accuracy);
  uint8_t getAccuracy();

  void setTimeStampSeconds(uint8_t seconds);
  uint8_t getTimeStampSeconds();
  
  void setRAIM(uint8_t Raim);
  uint8_t getRAIM();
  
private:
  void longToDouble(long inp, double* outp, double precision);
};

#endif // _BoatData_H_

