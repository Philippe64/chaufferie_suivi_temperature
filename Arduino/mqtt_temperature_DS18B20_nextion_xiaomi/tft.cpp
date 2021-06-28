#include <Arduino.h>
#include "tft.h"

TFT_eSPI tft = TFT_eSPI(); // Invoke custom library


#define TFT_LED    16

// Return the minimum of two values a and b
#define minimum(a,b)     (((a) < (b)) ? (a) : (b)) 

void initTFT(){
  // Initialise the TFT screen
  tft.init();
  
  // Turn on Backlight
  pinMode(TFT_LED, OUTPUT);
  digitalWrite(TFT_LED, HIGH);

  // Set the rotation 
  tft.setRotation(3);
  
  if(!SPIFFS.begin()){
    cia.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  
  _display_BgImage("/montain.jpg");
  
}


void displayTempTft(float tempAller, float tempRetour){
  tft.setTextColor(TFT_RED,TFT_WHITE);  // Text colour
  tft.setCursor(010, 010); // Set cursor at top left of screen
  tft.setTextSize(2);           // font size
  cia.print("temp aller : " ); cia.println (tempAller);
  cia.print("temp retour: " ); cia.println (tempRetour);
  /*
  tft.println(" ");
  tft.print ("Temp eau Aller   : "); tft.println(tempAller);
  tft.print ("Temp eau Retour  : "); tft.println(tempRetour);
  */
  String strToDisp;
  strToDisp = "Temp eau Aller  : " + String(tempAller);  
  tft.drawString(strToDisp, 010,010);
  
  strToDisp = "Temp eau Retour : " + String(tempRetour);
  tft.drawString(strToDisp, 010,040);
}

void _display_BgImage(const char *filename){
  cia.println("displayBG");
  tft.fillScreen(random(0xFFFF)); // black screen
  _drawJpeg(filename, 0 , 0);     // 240 x 320 image
   delay(2000); 
}


//====================================================================================
//   Opens the image file and prime the Jpeg decoder
//====================================================================================
void _drawJpeg(const char *filename, int xpos, int ypos) {

  cia.println("===========================");
  cia.print("Drawing file: "); cia.println(filename);
  cia.println("===========================");

  // Open the named file (the Jpeg decoder library will close it after rendering image)
  fs::File jpegFile = SPIFFS.open( filename, "r");    // File handle reference for SPIFFS
  //  File jpegFile = SD.open( filename, FILE_READ);  // or, file handle reference for SD library
  cia.println(jpegFile);
  cia.print(" jpegFile : "); 
  cia.print(jpegFile.name());  
  cia.print("  Size: "); 
  cia.println(jpegFile.size());
  //ESP32 always seems to return 1 for jpegFile so this null trap does not work
  if ( !jpegFile ) {
    cia.print("ERROR: File \""); cia.print(filename); cia.println ("\" not found!");
    return;
  }

  // Use one of the three following methods to initialise the decoder,
  // the filename can be a String or character array type:

  boolean decoded = JpegDec.decodeFsFile(jpegFile); // Pass a SPIFFS file handle to the decoder,
  //boolean decoded = JpegDec.decodeSdFile(jpegFile); // or pass the SD file handle to the decoder,
  //boolean decoded = JpegDec.decodeFsFile(filename);  // or pass the filename (leading / distinguishes SPIFFS files)
  //boolean decoded = JpegDec.decodeFsFile("/tiger.jpg");  // or pass the filename (leading / distinguishes SPIFFS files)
  
  if (decoded) {
    // print information about the image to the serial port
    _jpegInfo();

    // render the image onto the screen at given coordinates
    _jpegRender(xpos, ypos);
  }
  else {
    cia.println("Only Jpeg baseline format is supported!... image ignored");
  }
}

//====================================================================================
//   Decode and render the Jpeg image onto the TFT screen
//====================================================================================
void _jpegRender(int xpos, int ypos) {
  cia.println("Enter _jpegRender module");
  // retrieve infomration about the image
  uint16_t  *pImg;
  int16_t mcu_w = JpegDec.MCUWidth;
  int16_t mcu_h = JpegDec.MCUHeight;
  int32_t max_x = JpegDec.width;
  int32_t max_y = JpegDec.height;

  // Jpeg images are draw as a set of image block (tiles) called Minimum Coding Units (MCUs)
  // Typically these MCUs are 16x16 pixel blocks
  // Determine the width and height of the right and bottom edge image blocks
  int32_t min_w = minimum(mcu_w, max_x % mcu_w);
  int32_t min_h = minimum(mcu_h, max_y % mcu_h);

  // save the current image block size
  int32_t win_w = mcu_w;
  int32_t win_h = mcu_h;

  cia.print("win_w : "); cia.print(win_w); cia.print("  win_h : "); cia.println(win_h); 
  cia.print("max-x : "); cia.print(max_x); cia.print("  max_y : "); cia.println(max_y); 
  // record the current time so we can measure how long it takes to draw an image
  uint32_t drawTime = millis();

  // save the coordinate of the right and bottom edges to assist image cropping
  // to the screen size
  max_x += xpos;
  max_y += ypos;

  // read each MCU block until there are no more
  while ( JpegDec.readSwappedBytes()) { // Swapped byte order read

    // save a pointer to the image block
    pImg = JpegDec.pImage;

    // calculate where the image block should be drawn on the screen
    int mcu_x = JpegDec.MCUx * mcu_w + xpos;  // Calculate coordinates of top left corner of current MCU
    int mcu_y = JpegDec.MCUy * mcu_h + ypos;

    // check if the image block size needs to be changed for the right edge
    if (mcu_x + mcu_w <= max_x) win_w = mcu_w;
    else win_w = min_w;

    // check if the image block size needs to be changed for the bottom edge
    if (mcu_y + mcu_h <= max_y) win_h = mcu_h;
    else win_h = min_h;

    // copy pixels into a contiguous block
    if (win_w != mcu_w)
    {
      for (int h = 1; h < win_h-1; h++)
      {
        memcpy(pImg + h * win_w, pImg + (h + 1) * mcu_w, win_w << 1);
      }
    }

    // draw image MCU block only if it will fit on the screen
    if ( mcu_x < tft.width() && mcu_y < tft.height())
    {
      // Now push the image block to the screen
      tft.pushImage(mcu_x, mcu_y, win_w, win_h, pImg);
    }

    else if ( ( mcu_y + win_h) >= tft.height()) 
    {
      cia.println("image do not fit the sceen");
      JpegDec.abort();
    
    }
  }

  // calculate how long it took to draw the image
  drawTime = millis() - drawTime; // Calculate the time it took

  // print the results to the serial port
  cia.print("Total render time was    : "); cia.print(String(drawTime)); cia.println(" ms");
  cia.println("=====================================");

}

//====================================================================================
//   Print information decoded from the Jpeg image
//====================================================================================
void _jpegInfo() {

  cia.println("===============");
  cia.println("JPEG image info");
  cia.println("===============");
  cia.print  ("Width      :"); cia.println(JpegDec.width);
  cia.print  ("Height     :"); cia.println(JpegDec.height);
  cia.print  ("Components :"); cia.println(JpegDec.comps);
  cia.print  ("MCU / row  :"); cia.println(JpegDec.MCUSPerRow);
  cia.print  ("MCU / col  :"); cia.println(JpegDec.MCUSPerCol);
  cia.print  ("Scan type  :"); cia.println(JpegDec.scanType);
  cia.print  ("MCU width  :"); cia.println(JpegDec.MCUWidth);
  cia.print  ("MCU height :"); cia.println(JpegDec.MCUHeight);
  cia.println("===============");
  cia.println("");
}
