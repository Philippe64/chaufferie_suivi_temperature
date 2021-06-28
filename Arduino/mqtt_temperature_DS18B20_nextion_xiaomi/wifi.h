#ifndef WIFI_H
#define WIFI_H

#include "connect.h"
#include "parametre.h"
#if defined(ESP8266)
    #include <ESP8266WiFi.h>  //ESP8266 Core WiFi Library  
  #elif defined(ESP32)
    #include <WiFi.h>      //ESP32 Core WiFi Library   
  #else 
#endif

#include <NTPClient.h>
#include <WiFiUdp.h>
#include "Cia_print.h"

void setup_wifi();
void NTP_setup();
int NTP_heures();
int NTP_minutes();

#endif
