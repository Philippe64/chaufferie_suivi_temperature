#include <Arduino.h>
#include "connect.h"

const char* ssid = "CIA_PA";                   // SSID du réseau Wifi
const char* password = "RCn81BNDJK";      // Mot de passe du réseau Wifi.
const char* mqtt_server = "192.168.250.122";        // Adresse IP ou DNS du Broker.
const int mqtt_port = 1883;                        // Port du Brocker MQTT
const char* mqtt_login = "ciaMQTT";                // Login de connexion à MQTT.
const char* mqtt_password = "Doudou64";          // Mot de passe de connexion à MQTT.

const int _printPort    = 8787 ;                    // Port du destinataire message UDP  (Cia_print)
const int _udpLocalPort = 3424 ;                    // Portlocal message UDP  (Cia_print)

// Variables et constantes utilisateur :
const char* nomModule  = "Temperature DS18B20";          // Nom usuel de ce module. Sera visible uniquement dans les Log Domoticz.
