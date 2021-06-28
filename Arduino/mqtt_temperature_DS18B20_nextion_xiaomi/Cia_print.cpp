#include <Arduino.h>
#include "Cia_print.h"

WiFiUDP UdpSD;

//SET YOUR PC IP ADDRESS Listening for UDP message
IPAddress _udpIP(192, 168, 250, 255);

CiaTools cia = CiaTools(true);

// constructor
CiaTools::CiaTools(bool displayMsg=true) {
  // Anything you need when instantiating your object goes here
  _msg = displayMsg;
}

//Public

//Print String Value
void CiaTools::print(String message){   
  _ciaPrint(message);
}
//Println String Value
void CiaTools::println(String message){
  message += "\n";   
  _ciaPrint(message);
}

// format Value BIN=2, OCT=8, DEC=10, HEX=16
// Print int Value
void CiaTools::print(int intValue,int format){  
  String message;
  if (format == BIN){
    message = String(intValue,BIN);
  } 
  else if (format == OCT){
    message = String(intValue,OCT);
  } 
  else if (format == HEX){
    message = String(intValue,HEX);
  }
  else{
    message = String(intValue);
  }
  _ciaPrint(message);
}
// Println int Value
void CiaTools::println(int intValue,int format){  
  String message;
  if (format == BIN){
    message = String(intValue,BIN);
  } 
  else if (format == OCT){
    message = String(intValue,OCT);
  } 
  else if (format == HEX){
    message = String(intValue,HEX);
  }
  else{
    message = String(intValue);
  }
  message += "\n";
  _ciaPrint(message);
}

// Print Double value
void CiaTools::print(double dblValue,int format){
    String message;  
  if (format == BIN){
    message = String(dblValue,BIN);
  } 
  else if (format == OCT){
    message = String(dblValue,OCT);
  } 
  else if (format == HEX){
    message = String(dblValue,HEX);
  } 
  else{
    message = String(dblValue);
  }
  _ciaPrint(message);
}
// Println Double value
void CiaTools::println(double dblValue,int format){
  String message;  
  if (format == BIN){
    message = String(dblValue,BIN);
  } 
  else if (format == OCT){
    message = String(dblValue,OCT);
  } 
  else if (format == HEX){
    message = String(dblValue,HEX);
  }
  else{
    message = String(dblValue);
  }
  message += "\n"; 
  _ciaPrint(message);
}

// print Long Value
void CiaTools::print(long lgValue,int format){ 
  String message;  
  if (format == BIN){
    message = String(lgValue,BIN);
  } 
  else if (format == OCT){
    message = String(lgValue,OCT);
  } 
  else if (format == HEX){
    message = String(lgValue,HEX);
  }
 else{
    message = String(lgValue);
  }
  _ciaPrint(message);
}
// println Long Value
void CiaTools::println(long lgValue,int format){ 
  String message;  
  if (format == BIN){
    message = String(lgValue,BIN);
  } 
  else if (format == OCT){
    message = String(lgValue,OCT);
  } 
  else if (format == HEX){
    message = String(lgValue,HEX);
  }
 else{
    message = String(lgValue);
  }
  message += "\n"; 
  _ciaPrint(message);
}

// print size-t Value
void CiaTools::print(size_t szeValue,int format){ 
  String message;  
  if (format == BIN){
    message = String(szeValue,BIN);
  } 
  else if (format == OCT){
    message = String(szeValue,OCT);
  } 
  else if (format == HEX){
    message = String(szeValue,HEX);
  }
 else{
    message = String(szeValue);
  }
  _ciaPrint(message);
}
// println size_t Value
void CiaTools::println(size_t szeValue,int format){ 
  String message;  
  if (format == BIN){
    message = String(szeValue,BIN);
  } 
  else if (format == OCT){
    message = String(szeValue,OCT);
  } 
  else if (format == HEX){
    message = String(szeValue,HEX);
  }
 else{
    message = String(szeValue);
  }
  message += "\n"; 
  _ciaPrint(message);
}

// Print Float value
void CiaTools::print(float flValue,int decimal){  
  String message = String(flValue, decimal); 
  _ciaPrint(message);
}
void CiaTools::println(float flValue,int decimal){
    String message = String(flValue, decimal);
    message += "\n";
  _ciaPrint(message);
}



// printf
void CiaTools::printf(const char *fmt_string, ...){
  char buffOut[128];
  va_list args;           /* to access the arguments passed in  */ 
  va_start(args, fmt_string);
  vsprintf(buffOut,fmt_string, args);

  String messageOut(buffOut);
  messageOut += "\n";
  _ciaPrint(messageOut);
  
  /* Reset, required cleanup */
  va_end(args);
  
  return;
}

//Private
void CiaTools::_ciaPrint(String message){    
  bool ret = false;
  if (_debug){
    if (_debugSerial && Serial){
      Serial.print(message);
    }

    if (_debugUDP && (WiFi.status() == WL_CONNECTED)){
      ret = _udpprint(message, _printPort);  
      /*
      Serial.print ("_udpLocalPort _udpPrint : "); Serial.println(_udpLocalPort);  
      Serial.print ("_printPort _udpPrint : "); Serial.println(_printPort); 
      Serial.print ("retour de _udpPrint : "); Serial.println(ret);  
      */
    }
  }
}

// Routines envoi message via UDP
// ******************************
bool CiaTools::_udpprint(String text1, int printPort) {
  bool result=false;
 //Construct Char* from String  
  String str = text1;

  int strSize = str.length()+1; 
  //char buffer[strSize];   // Tableau des valeurs ascii du message
  char buffer[strSize];
  str.toCharArray(buffer, strSize);
  
  result = _udp_write(buffer, printPort, strSize);
  return (result);
}

//bool _udp_write(char* text, int port) {
bool CiaTools::_udp_write( char* buffer, int port, int strSize) {
  //UDP Write

  UdpSD.stop();
  UdpSD.begin(_udpLocalPort);
  delay(100); //Tweak this
  if (UdpSD.beginPacket(_udpIP, port)) {      
    UdpSD.write((uint8_t*)buffer, strSize);
    UdpSD.endPacket();
    return (true);
  } else {
    UdpSD.stop();
    return (false);
  }
} 
