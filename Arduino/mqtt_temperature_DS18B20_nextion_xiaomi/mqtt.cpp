#include <Arduino.h>
#include "mqtt.h"

// Initialisation CiaTools
//CiaTools cia1 = CiaTools(true);

// Initialisation du Wifi et du MQTT.
WiFiClient espClient;
PubSubClient client(espClient);

// CONNEXION MQTT
// **************
void init_connect_mqtt() {
  cia.println (" setServer MQTT");
  client.setServer(mqtt_server, mqtt_port);       // On défini la connexion MQTT
}


void reconnect() {
  // Boucle jusqu'à la connexion MQTT
  while (!client.connected()) {
    cia.print("Tentative de connexion MQTT...");
    // Création d'un ID client aléatoire
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    
    // Tentative de connexion
    if (client.connect(clientId.c_str(), mqtt_login, mqtt_password)) {
      cia.println("connecté");
      
      // Connexion effectuée, publication d'un message...
      String message = "Connexion MQTT de "+ String(nomModule) + " réussi sous référence technique : " + clientId + ".";
      
      // Création de la chaine JSON6
      DynamicJsonDocument doc(256);
      // On renseigne les variables.
      doc["command"] = "addlogmessage";
      doc["message"] = message;
      // On sérialise la variable JSON
      String messageOut;
      serializeJson(doc, messageOut);
      
      // Convertion du message en Char pour envoi dans les Log Domoticz.
      char messageChar[messageOut.length()+1];
      messageOut.toCharArray(messageChar,messageOut.length()+1);
      client.publish(topicOut, messageChar);
        
      // On souscrit (écoute)
      client.subscribe("#");
    } else {
      cia.print("Erreur, rc=");
      cia.print(String(client.state()));
      //cia.print(client.state());
      cia.println(" prochaine tentative dans 2s");
      // Pause de 2 secondes
      delay(2000);
    }
  }
}


// ENVOI DES DATAS.
// ***************
void SendData (String command, int idxDevice, int nvalue, String svalue) {
  // On vérifie la connexion MQTT
  cia.println("SendData verif");
  verif_connexion_mqtt();
  cia.println("SendData verif  -OK- ");
  // Création de la chaine JSON6
  DynamicJsonDocument doc(256);
  // On renseigne les variables.
  doc["command"]  = command;
  doc["idx"]      = idxDevice;
  doc["nvalue"]   = nvalue;
  doc["svalue"]   = svalue;
  
  // On sérialise la variable JSON
  String messageOut;
  serializeJson(doc, messageOut);
    char* topicOut    = "domoticz/in";  
  // Convertion du message en Char pour envoi dans les Log Domoticz.
  char messageChar[messageOut.length()+1];
  messageOut.toCharArray(messageChar,messageOut.length()+1);
  client.publish(topicOut, messageChar);
  // Pause de 1 secondes
  delay(1000);
  cia.println("\t -> Message envoyé à Domoticz");
}

// On vérifie si MQTT est connecté, sinon on connect.
// *************************************************
void verif_connexion_mqtt() {
  if (!client.connected()) {
    // MQTT déconnecté, on reconnecte.
    cia.println("MQTT déconnecté, on reconnecte !");
    reconnect();
  }
}
