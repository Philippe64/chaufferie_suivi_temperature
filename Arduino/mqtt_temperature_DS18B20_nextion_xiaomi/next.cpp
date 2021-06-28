#include <Arduino.h>

#include "next.h"
 // Initialise serial connection for Nextion Screen
  // check \Arduino\libraries\Easy_Nextion_Library\src\EasyNextionLibrary.cpp for serial pins
  // we use hardware Serial2 (pin RX: 16, TX:17)
  const int RXD = 16;
  const int TXD = 17;

  #ifndef debugNEX
    EasyNex myNex = EasyNex (Serial2);    // Create an object of EasyNex class with the name < myNex >
                                        // Set as parameter the Serial you are going to use
  #else
       // for debug we will use Serial instead of Serial2, HMI is listen on USB port
    // so Serial.print will adress HMI and not monitor
    //EasyNex myNex = EasyNex(Serial);
     EasyNex myNex= EasyNex(Serial); 
  #endif 
void initNEXT(){     
  cia.println("initialisation myNex");
   if (!debugNEXT){    
    myNex.begin(9600,RXD,TXD); // Begin the object with a baud rate of 9600
                                        // If no parameter was given in the begin(), the default baud rate of 9600 will be used
  }
  //Debugging on ne passe pas les pins
  else{                                     
     myNex.begin(9600);
  }
    
}


void displayTemp(float tempAller, float tempRetour, float tempPiece){
  cia.println("on entre dans module displayTemp Nextion");
  // envoi des données à Nextion 
  int _tempAller = tempAller * 100;
  String TtempAller = String(_tempAller);
  cia.println("envoi _tempAller : " +TtempAller);
  myNex.writeStr("TtempAller.txt",TtempAller);
  delay(500);
  
  int _tempRetour = tempRetour * 100;
  String TtempRetour = String(_tempRetour);
  cia.println("envoi _tempRetour : " + TtempRetour);
  myNex.writeStr("TtempRetour.txt",TtempRetour);
  delay(500);
 
  int _tempPiece = tempPiece * 100;
  String TtempPiece = String(_tempPiece);
  cia.println("envoi _tempPiece : " + TtempPiece);
  myNex.writeStr("TtempPiece.txt",TtempPiece);
  delay(500);
}

 void displayTempExt(float tempExt, float hygroExt, float battExt){
  cia.println("on entre dans module displayTempExt Nextion");
  
  // envoi des données à Nextion 
  int _tempExt = tempExt * 100;
  String TtempExt = String(_tempExt);
  cia.println("envoi _tempExt : " + TtempExt);
  myNex.writeStr("TtempExt.txt",TtempExt);
  delay(500);
  
  int _hygroExt = hygroExt * 100;
  String ThygroExt = String(_hygroExt);
  cia.println("envoi _hygroExt : " + ThygroExt);
  myNex.writeStr("ThygroExt.txt",ThygroExt);
  delay(500);
 
  int _battExt = battExt * 100;
  String TbattExt = String(_battExt);
  cia.println("envoi _battExt : " + TbattExt);
  myNex.writeStr("TbattExt.txt",TbattExt);
  delay(500);
}
