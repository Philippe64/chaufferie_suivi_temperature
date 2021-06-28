#include <Arduino.h>
#include "LYWSD03MMC.h"

// adresse MAC du senssor
#define LYWSD03MMC_ADDR "a4:c1:38:02:a5:e1"

// contruction d'un client BLE
BLEClient* pClient;

// construction object adreesse
static BLEAddress htSensorAddress(LYWSD03MMC_ADDR);

bool connectionSuccessful = false;

float tempLYWSD03MMC = 0;
float hygroLYWSD03MMC = 0;
float voltLYWSD03MMC = 0;
float battLYWSD03MMC = 0;

int idxLYWSD03MMC   = 47;          // Index domoticz du Device de température exterieure

// The remote service we wish to connect to.
static BLEUUID serviceUUID("ebe0ccb0-7a0a-4b0c-8a1a-6ff2997da3a6");
// The characteristic of the remote service we are interested in.
static BLEUUID    charUUID("ebe0ccc1-7a0a-4b0c-8a1a-6ff2997da3a6");
bool bleDeviceConnected =false;
bool bleDeviceDisConnected =false;


void MyClientCallback::onConnect(BLEClient* pclient) {
  cia.println("BLEClient Connected");
  bleDeviceConnected=true;
  bleDeviceDisConnected = false;
}

void MyClientCallback::onDisconnect(BLEClient* pclient) {
  cia.println("BLEClient Disconnected");
  bleDeviceDisConnected = true;
  bleDeviceConnected=false;
  if (!connectionSuccessful) {
    cia.println("RESTART");
    //ESP.restart();
  }
}



static void notifyCallback( BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify) {

  cia.println("Notify callback for characteristic ");
  //cia.println(pBLERemoteCharacteristic->getUUID().toString().c_str());
  tempLYWSD03MMC = (pData[0] | (pData[1] << 8)) * 0.01; //little endian 
  hygroLYWSD03MMC = pData[2];
  voltLYWSD03MMC = ((pData[4] * 256) + pData[3]) / 1000.0;
  battLYWSD03MMC = (voltLYWSD03MMC - 2.1) * 100.0;
  cia.printf("temp = %.2f : humidity = %.0f : voltage = %.2f : battery = %.0f \n", tempLYWSD03MMC, hygroLYWSD03MMC, voltLYWSD03MMC, battLYWSD03MMC);

  // Envoi ecran Nextion
  displayTempExt(tempLYWSD03MMC, hygroLYWSD03MMC, battLYWSD03MMC);
  
  // envoie Domoticz via MQTT
  String svalue =  String(tempLYWSD03MMC) ;  // temperature exterieure
  svalue += ";" ;
  svalue +=  String(hygroLYWSD03MMC) ;  // Hygrometri exterieure
  svalue += ";" ;
  svalue += "0" ;                       // Notion de confort
  SendData("udevice", idxLYWSD03MMC, 0, svalue);
  
  pClient->disconnect();
}


void registerNotification() {

  // Obtain a reference to the service we are after in the remote BLE server.
  BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
  if (pRemoteService == nullptr) {
    cia.print("Failed to find our service UUID: ");
    cia.println(serviceUUID.toString().c_str());
    pClient->disconnect();
  }
  cia.println(" - Found our service");

  // Obtain a reference to the characteristic in the service of the remote BLE server.
  BLERemoteCharacteristic* pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
  if (pRemoteCharacteristic == nullptr) {
    cia.print("Failed to find our characteristic UUID: ");
    //cia.println(charUUID.toString().c_str());
    pClient->disconnect();
  }
  cia.println(" - Found our characteristic");
  pRemoteCharacteristic->registerForNotify(notifyCallback);
}

void createBleClientWithCallbacks() {
  pClient = BLEDevice::createClient();
  pClient->setClientCallbacks(new MyClientCallback());
}

void connectSensor() {
  pClient->connect(htSensorAddress);
  connectionSuccessful = true;
}


void init_LYWSD03MMC(){
  BLEDevice::init("ESP32");
  cia.println("BLE device initialised");
}

void getDataLYWSD03MMC(){
  createBleClientWithCallbacks();
  delay(500);
  connectSensor();
  registerNotification(); 
}
