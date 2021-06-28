#include <Arduino.h>
#include "parametre.h"

// -------------------------------------
// Variables de configuration debug:
const bool _debug       = true;             // true/false Active envoi message
      bool _debugSerial = true;             // true/false Active envoi message via port Serie
const bool _debugUDP    = true;             // true/false Active envoi message via UDP
const bool debugNEXT   = false;
// -------------------------------------

// ---------------------------------------------------
// Variables de configuration :
const char* topicIn     = "domoticz/out";             // Nom du topic envoyé par Domoticz
const char* topicOut    = "domoticz/in";              // Nom du topic écouté par Domoticz
// ---------------------------------------------------



// On intégre les librairies necessaires à l'I2C.
#include <Wire.h>
