#include <Arduino.h>
#include "ds18b20.h"
// On intégre les librairies necessaires aux Capteurs.
#include <OneWire.h>                                  //Librairie du bus OneWire
#include <DallasTemperature.h>                        //Librairie du capteur

// Variables Index Domoticz. (Mettre IDX = 0 si pas de device).
int idxDS18B20_0   = 13;                            // Index du Device de température depart
int idxDS18B20_1   = 14;                            // Index du Device de température retour
int idxDS18B20_2   = 45;                            // Index du Device de température ambiante

// Variables techniques :
#define ONE_WIRE_BUS 15                                  // Pin de connexion de la DS18B20 
// variable to hold device addresses
DeviceAddress Thermometer;
int deviceCount = 0;
float valTemp_0 = 0.0;                                  // Variables contenant la valeur de température.
float valTemp_1 = 0.0;                                  // Variables contenant la valeur de température.
float valTemp_2 = 0.0;                                  // Variables contenant la valeur de température.

// Initialisation du DHT
OneWire oneWire(ONE_WIRE_BUS);              // Initialisation du Bus One Wire
DallasTemperature sensors(&oneWire);        // Utilistion du bus Onewire pour les capteurs

void printAddress(DeviceAddress deviceAddress)
{ 
  for (uint8_t i = 0; i < 8; i++)
  {
    cia.print("0x");
    if (deviceAddress[i] < 0x10) cia.print("0");
    
    cia.print(deviceAddress[i], HEX);
    if (i < 7) cia.print(", ");
  }
  cia.println("");
}
void init_ds18b20() {
  sensors.begin();              // Initialisation du capteur DHT 
  delay(1000); 
  // locate devices on the bus
  cia.print("DS18B20 Locating devices...");
  cia.print("Found ");
  deviceCount = sensors.getDeviceCount();
  cia.print(deviceCount);
  cia.println(" devices.");
  cia.println(""); 

  cia.println("Printing addresses...");
  for (int i = 0;  i < deviceCount;  i++)
  {
    cia.print("Sensor ");
    cia.print(i+1);
    cia.print(" : ");
    sensors.getAddress(Thermometer, i);
    printAddress(Thermometer);
  }
}

 void getDataDS18B20() {
  // Relevé capteur DS18B20 (index =0)
  // On relévé la température.
  // ------------------------
  sensors.requestTemperatures();            // On demande au capteur de lire la température
  for (int i = 0;  i < deviceCount;  i++)  {
    delay(2000);
   if (i==0){
    valTemp_0 = sensors.getTempCByIndex(i);
   }
   if (i==1){
    valTemp_1 = sensors.getTempCByIndex(i);
   }
   if (i==2){
    valTemp_2 = sensors.getTempCByIndex(i);
   }
  }
  /*
   delay(2500);
  valTemp_0 = sensors.getTempCByIndex(0);      // On stocke la température relevé dans une variable temporaire
   delay(1500);
  valTemp_1 = sensors.getTempCByIndex(1);      // On stocke la température relevé dans une variable temporaire
   delay(1500);
  valTemp_2 = sensors.getTempCByIndex(2);      // On stocke la température relevé dans une variable temporaire
   delay(1500);
 */
  cia.print("valTemp_0 Valeur de température Aller : ");
  cia.println(String(valTemp_0));
  cia.print("valTemp_1 Valeur de température Retour: ");
  cia.println(String(valTemp_1));
  cia.print("valTemp_2 Valeur de température Piece : ");
  cia.println(String(valTemp_2));
  cia.printf("isnan valTemp_0 : %f\n",!isnan(valTemp_0));
  cia.printf("isnan valTemp_1 : %f\n",!isnan(valTemp_1));
  cia.printf("isnan valTemp_2 : %f\n",!isnan(valTemp_2));
  
  // envoi temperature sur TFT  
  displayTemp(valTemp_0, valTemp_1, valTemp_2);
  // ************ prov  *******
  //displayTemp(42.55, 60.85, 20.5);
  
  // Si on a des données, on envoi à Domoticz 
  if (valTemp_0,1 != -127 && valTemp_1 !=-127 && valTemp_2 !=-127) {
    cia.println("check if data to send");              
    if ( idxDS18B20_0 != 0) {
      String svalue =  String(valTemp_0,1); 
      svalue += ";" ; 
      cia.print("DS18B20_0 : svalue = "); 
      cia.println(svalue);
      SendData("udevice", idxDS18B20_0, 0, svalue);
    }
      
    if ( idxDS18B20_1 != 0) {
      String svalue =  String(valTemp_1,1) ;
      svalue += ";" ;
      cia.print("DS18B20_1 : svalue = "); 
      cia.println(svalue);
      SendData("udevice", idxDS18B20_1, 0, svalue);
    }
      
    if ( idxDS18B20_2 != 0) {
      String svalue =  String(valTemp_2,1) ;
      svalue += ";" ;
      cia.print("DS18B20_1 : svalue = "); 
      cia.println(svalue);
      SendData("udevice", idxDS18B20_2, 0, svalue);
    }
  }
  else{
    if (valTemp_0 == -127){
      cia.println("Attention problème sur capteur de température Aller");
    }
    if (valTemp_1 == -127){
      cia.println("Attention problème sur capteur de température Retour");
    }
     if (valTemp_2 == -127){
      cia.println("Attention problème sur capteur de température Piece");
    }
  }
  
}
