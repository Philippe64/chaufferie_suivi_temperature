#ifndef CONNECT_H
#define CONNECT_H

#include "wifi.h"
/* 
 *  Constante de connexion
 */
extern const char* ssid;               // SSID du réseau Wifi
extern const char* password;           // Mot de passe du réseau Wifi.
extern const char* mqtt_server;        // Adresse IP ou DNS du Broker.
extern const int mqtt_port;            // Port du Brocker MQTT
extern const char* mqtt_login;         // Login de connexion à MQTT.
extern const char* mqtt_password;      // Mot de passe de connexion à MQTT.

extern const int _printPort;           // Port du destinataire message UDP  (Cia_print)
extern const int _udpLocalPort;        // Portlocal message UDP  (Cia_print)

// Variables et constantes utilisateur :
extern const char* nomModule;         // Nom usuel de ce module. Sera visible uniquement dans les Log Domoticz.

#endif
