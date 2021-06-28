/*
 Station météo à base d'un ESP8266, et envoi des données vers Domoticz via MQTT.
 Création Dominique PAUL.
 Dépot Github : https://github.com/DomoticDIY/MQTT-ModuleTemp_DS18B20
 Chaine YouTube du Tuto Vidéo : https://www.youtube.com/c/DomoticDIY
  
 Bibliothéques nécessaires, et version utilisées :
  - pubsubclient v2.7.0 : https://github.com/knolleary/pubsubclient
  - ArduinoJson v6.15.0 : https://github.com/bblanchon/ArduinoJson
  - ESP8266Wifi v1.0.0
  - ArduinoOTA v1.0.0
   - OnWire
  - DallasTemperature


 Installer le gestionnaire de carte ESP8266 version 2.7.1 
 Si besoin : URL à ajouter pour le Bord manager : http://arduino.esp8266.com/stable/package_esp8266com_index.json
 
 Adaptation pour reconnaissance dans Domoticz :
 Dans le fichier PubSubClient.h : La valeur du paramètre "MQTT_MAX_PACKET_SIZE" doit être augmentée à 512 octets. Cette définition se trouve à la ligne 26 du fichier.
 Sinon cela ne fonctionne pas avec Domoticz
 
 Pour prise en compte du matériel :
 Installer si besoin le Driver USB CH340G : https://wiki.wemos.cc/downloads
 dans Outils -> Type de carte : generic ESP8266 module
  Flash mode 'QIO' (régle générale, suivant votre ESP, si cela ne fonctionne pas, tester un autre mode.
  Flash size : 1M (no SPIFFS)
  Port : Le port COM de votre ESP vu par windows dans le gestionnaire de périphériques.
*/

#include "connect.h"
#include "parametre.h"
#include "wifi.h"
#include "udp.h"
#include "mqtt.h"
#include "ota.h"
#include "Cia_print.h"
#include "ds18b20.h"
#include "LYWSD03MMC.h"
#include "next.h"
// #include "tft.h"
// #include "bme280.h"
// #include "dht22.h"

// Initialisation CiaTools
//CiaTools cia = CiaTools(true);
// Variables et constantes utilisateur :
unsigned long t_lastActionCapteur = 0;                // enregistre le Time de la dernière intérogation.
//const long t_interoCapteur = 60000;                 // Valeur de l'intervale entre 2 relevés. en milli seconde 
const long t_interoCapteur = 30000;

void setup() {
  delay(1000);
  cia.print("debugNEXT : " + String(debugNEXT));
  if(debugNEXT){
    _debugSerial = false;
  }
  cia.print("_debugSerial : " + String(_debugSerial));
  if (_debugSerial){
    Serial.begin(115200);                           // On initialise la vitesse de transmission de la console.
  }
  setup_wifi();                                   // Connexion au Wifi
  NTP_setup();                                    // Connexion au NTP
  initOTA();                                      // Initialisation de l'OTA
  init_connect_mqtt();                            // On défini la connexion MQTT
  init_capteurs();                                // Initialisation des Capteurs de relevés
  //initTFT();
  initNEXT();
}

void loop() {
  maj_ota();                                      // On verifie si une mise a jour OTA nous est envoyée. Si OUI, la lib ArduinoOTA se charge de faire la MAJ.
  
  unsigned long currentMillis = millis();         // On enregistre le Time courant

  // On vérifie que l'intervale de relevé des capteurs est atteint. (60s)
  if (currentMillis - t_lastActionCapteur >= t_interoCapteur) {
    // MQTT connecté, on exécute les traitements

    #ifdef DHT22_H
      getDataDHT();       // Relevé des données du DHT, et envoi à Domoticz
    #endif

    #ifdef DS18B20_H
      getDataDS18B20();       // Relevé des données du DS18B20, et envoi à Domoticz
    #endif

    #ifdef LYWSD03MMC_H
      getDataLYWSD03MMC();       // Relevé des données du capteur externe, et envoi à Domoticz
    #endif
        
    // Traitement effectué, on met à jour la valeur du dernier traitement.
    t_lastActionCapteur = currentMillis;
  }
}



// INITIALISATION des Capteurs
// ***************************
void init_capteurs() {
  #ifdef DHT22_H
    init_dht();             // Initialisation du capteur DHT
  #endif

  #ifdef DS18B20_H
    init_ds18b20();             // Initialisation du capteur DHT
  #endif

  
  #ifdef LYWSD03MMC_H
    init_LYWSD03MMC();             // Initialisation du capteur DHT
  #endif
}



 
