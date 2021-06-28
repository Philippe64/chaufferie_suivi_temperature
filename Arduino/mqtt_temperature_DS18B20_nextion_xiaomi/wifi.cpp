#include <Arduino.h>
#include "wifi.h"


// Initialisation CiaTools
//CiaTools cia3 = CiaTools(true);

// Appel de la bibliothéque NTP
WiFiUDP ntpUDP;
//WiFiUDP UdpSD;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 60000);


// CONNEXION WIFI
// **************
void setup_wifi() {
  // Connexion au réseau Wifi
  delay(10);
  cia.println();
  cia.print("Connection au réseau : ");
  cia.println(ssid);
  WiFi.mode(WIFI_STA);            // Passage de la puce en mode client
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    // Tant que l'on est pas connecté, on boucle.
    delay(500);
    cia.print(".");
  }
  // Initialise la séquence Random
  randomSeed(micros());

  cia.println("");
  cia.println("WiFi connecté");
  cia.println("Adresse IP : "  + WiFi.localIP().toString());
  cia.println ("Passerelle : "  + WiFi.gatewayIP().toString());
  cia.println ("DNS IP : "      + WiFi.dnsIP().toString());
  cia.println ("Puissance de reception : " + String(WiFi.RSSI()));
}

// CONNEXION NTP
// *************
void NTP_setup(){
  // Begin du NTP
  timeClient.begin();
}

int NTP_heures() {
  // Retour int de l'heure NTP
  timeClient.update();
  return timeClient.getHours();
}

int NTP_minutes(){
  // Retour int des minutes NTP
  timeClient.update();
  return timeClient.getMinutes();
}
