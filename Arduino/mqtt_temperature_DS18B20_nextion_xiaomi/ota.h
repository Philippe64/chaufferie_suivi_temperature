#ifndef OTA_H
#define OTA_H


#include "connect.h"
#include "Cia_print.h"

// On intégre les bibliothéques necessaire à la mise à jour via OTA.
#if defined(ESP8266)
    #include <ESP8266mDNS.h>   // for OTA Update
  #elif defined(ESP32)
    #include <ESPmDNS.h>
  #else 
#endif

#include <ArduinoOTA.h>

void initOTA();
void maj_ota();

#endif
