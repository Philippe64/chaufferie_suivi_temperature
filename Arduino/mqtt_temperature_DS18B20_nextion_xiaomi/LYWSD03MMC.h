#ifndef LYWSD03MMC_H
#define LYWSD03MMC_H

#include <BLEDevice.h>
#include <SimpleTimer.h>

#include "mqtt.h"
#include "Cia_print.h"
#include "tft.h"
#include "next.h"


void init_LYWSD03MMC();
void getDataLYWSD03MMC();

class MyClientCallback : public BLEClientCallbacks{
  void onConnect(BLEClient* pclient);
  void onDisconnect(BLEClient* pclient);
};

static void notifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length,  bool isNotify);
void registerNotification();
void createBleClientWithCallbacks() ;
void connectSensor() ;


#endif
