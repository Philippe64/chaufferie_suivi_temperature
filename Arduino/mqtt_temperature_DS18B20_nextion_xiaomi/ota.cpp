#include <Arduino.h>
#include "ota.h"


// Initialisation CiaTools
//CiaTools cia2 = CiaTools(true);


// INITIALISATION Arduino OTA
// **************************
void initOTA() {
  /* Port par defaut = 8266 */
  // ArduinoOTA.setPort(8266);

  /* Hostname, par defaut = esp8266-[ChipID] */
  ArduinoOTA.setHostname(nomModule);

  /* Pas d'authentication par defaut */
  // ArduinoOTA.setPassword("admin");

  /* Le mot de passe peut également être défini avec sa valeur md5 */
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");
  
  // code à exécuter au démarrage de la mise à jour
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";

    // NOTE: si vous mettez à jour SPIFFS, démonter SPIFFS à l'aide de SPIFFS.end ()
    cia.println("Début de update " + type);
  });
  
  // code à exécuter à la fin de la mise à jour
  ArduinoOTA.onEnd([]() {
    cia.println("\nFin");
  });
  
  // code à exécuter pendant la mise à jour
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    cia.printf("Progression : %u%%\r", (progress / (total / 100)));
  });
  
  // code à exécuter en cas d'erreur de la mise à jour
  ArduinoOTA.onError([](ota_error_t error) {
    cia.printf("Erreur[%u] : ", error);
    if (error == OTA_AUTH_ERROR) cia.println("Authentification Failed");
    else if (error == OTA_BEGIN_ERROR) cia.println("Exécution Failed");
    else if (error == OTA_CONNECT_ERROR) cia.println("Connexion Failed");
    else if (error == OTA_RECEIVE_ERROR) cia.println("Réception Failed");
    else if (error == OTA_END_ERROR) cia.println("Fin Failed");
  });
  
  ArduinoOTA.begin();
  //cia.println("Prêt");
  //cia.print("Adresse IP : ");
  //cia.println(WiFi.localIP());
  cia.println("Prêt");
  cia.print("Adresse IP : ");
  cia.println(WiFi.localIP().toString());
}

void maj_ota() {
   ArduinoOTA.handle();                            // On verifie si une mise a jour OTA nous est envoyée. Si OUI, la lib ArduinoOTA se charge de faire la MAJ.
}
