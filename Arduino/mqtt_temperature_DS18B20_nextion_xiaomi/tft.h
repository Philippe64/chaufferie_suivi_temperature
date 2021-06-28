#ifndef TFT_H
#define TFT_H
  
  #include <Arduino.h>
  #include <FS.h>    // File system manager
  #include <SPI.h>
  #include "Cia_print.h"
  
  
  // JPEG decoder library
  #include <JPEGDecoder.h>
  #include <TFT_eSPI.h>      // Hardware-specific library
  
  void initTFT();
  void displayTempTft(float tempAller, float tempRetour);
  void _display_BgImage(const char *filename);
  void _drawJpeg(const char *filename, int xpos, int ypos);
  void _jpegRender(int xpos, int ypos);
  void _jpegInfo();
#endif
