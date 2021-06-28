#ifndef PARAMETRE_H
#define PARAMETRE_H

//#define debugNEX                     // Definir uniquement si debug dans HMI


// -------------------------------------
// Variables de configuration debug:
extern const bool _debug;              // True/false Active envoi message
extern       bool _debugSerial;         // True/false Active envoi message via port Serie
extern const bool _debugUDP;            // True/false Active envoi message via UDP
extern const bool debugNEXT;            // True/false Active envoi message via UDP
// -------------------------------------


// -------------------------------------
// Variables de configuration :
extern const char* topicIn;             // Nom du topic envoyé par Domoticz
extern const char* topicOut;            // Nom du topic écouté par Domoticz
// -------------------------------------



#endif
