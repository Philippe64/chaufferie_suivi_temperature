#ifndef CIA_PRINT_H
#define CIA_PRINT_H

#include <WiFiUdp.h>
// On intégre les librairies necessaires à la connexion Wifi, et udp
#include "connect.h"
#include "parametre.h"
#include "wifi.h"



  class CiaTools  {
    public:
    // Constructor 
      CiaTools(bool displayMsg);
      
    // Properties
      
    // Methods        
      void print(String message= " ");
      void println(String message=" ");
      //void print();
      //void println();
      
      // format Value BIN=2, OCT=8, DEC=10, HEX=16
      void print(int intValue,int format=DEC);
      void println(int intValue,int format=DEC);
      
      void print(double dblValue,int format=DEC);
      void println(double dblValue,int format=DEC);   
         
      void print(long lgValue,int format=DEC);
      void println(long lgValue,int format=DEC);

      void print(size_t szeValue,int format=DEC);
      void println(size_t szeValue,int format=DEC);     
            
      void print(float flValue,int decimal=2);
      void println(float flValue,int decimal=2);

      void printf(const char *fmt_string, ...);
       
  private:
    bool _msg;
    void _ciaPrint(String message);
    bool _udp_write( char* buffer, int port, int strSize);
    bool _udpprint(String text1, int printPort);
  };
//Cette classe ne peut avoir qu'une seule instance
//Donc autant la créer ici
extern CiaTools cia;
#endif
