#include <Arduino.h>
#include "dht22.h"



// Variables Index Domoticz. (Mettre IDX = 0 si pas de device).
int idxDHT22          = 63;                           // Index du Device de température et humidité.
int idxTempRessenti   = 65;                           // Index du Device de température ressenti (fonction de la température et de l'humidité du DHT22).

// Variables techniques :
#define DHTPIN 4                                      // PIN digital de connexion DATA du DHT22
#define DHTTYPE DHT22                                 // Définition du type de DHT utilisé

// On intégre les librairies necessaires aux Capteurs.
#include <DHT.h>

// Initialisation du DHT
DHT dht(DHTPIN, DHTTYPE);

void init_dht() {
  dht.begin();              // Initialisation du capteur DHT  
}

 void getDataDHT() {
  // Relevé et envoi du DHT11 ou DHT22 (Température et humidité)
  float humDHT   = dht.readHumidity();                         // Lecture de l'humidité
  float tempDHT  = dht.readTemperature();                      // Lecture de la temperature en degrés Celsius (par défaut)

  // Si on a des données, on envoi à Domoticz 
  if (isnan(humDHT) || isnan(tempDHT)) {
    // On établi le degré de confort
    int hum_status = 0;
    if ((humDHT >= 45) and (humDHT <= 70))      { hum_status = 1; } // confortable
    else if ((humDHT >= 30) and (humDHT < 45))  { hum_status = 0; } // Normal
    else if (humDHT < 30)                       { hum_status = 2; } // sec
    else if (humDHT > 70)                       { hum_status = 3; } // humide
                  
    if ( idxDHT22 != 0) {
      String svalue =  String(tempDHT,1) + ";" + String(humDHT,0) + ";" + String(hum_status);
      cia.print("DHT22 : svalue = "); cia.print(svalue); cia.print(" (TEMP;HUM;HUM_STAT)");
      SendData("udevice", idxDHT22, 0, svalue);
    }
      
    if ( idxTempRessenti != 0) {
      String svalue2 = String(dht.computeHeatIndex(tempDHT, humDHT, false),1);   // Calcul de l'indice de chaleur en Celsius - Température ressenti (isFahreheit = false)
      cia.print("DHT22 Temp Ressenti : svalue = "); cia.print(svalue2); cia.print(" (TEMP)");
      SendData("udevice", idxTempRessenti, 0, svalue2);
    } 
  }
}
