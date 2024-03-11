#include "SPI.h"      
#include "DMD.h" 
#include "TimerOne.h"
#include "Arial_black_16.h"<arial_black_16.h> 
// you can remove the fonts if unused
#include "SystemFont5x7.h"
#define DISPLAYS_ACROSS 1   
#define DISPLAYS_DOWN 1       
/* change these values if you have more than one DMD connected */
DMD dmd(DISPLAYS_ACROSS,DISPLAYS_DOWN);

void ScanDMD()
{ 
  dmd.scanDisplayBySPI();
}
void setup()
{
   Timer1.initialize( 5000 );           
/*period in microseconds to call ScanDMD. Anything longer than 5000 (5ms) and you can see flicker.*/

   Timer1.attachInterrupt( ScanDMD );  
/*attach the Timer1 interrupt to ScanDMD which goes to dmd.scanDisplayBySPI()*/

   dmd.clearScreen( true );            
/* true is normal (all pixels off), false is negative (all pixels on) */
}
void loop(){
//  dmd.selectFont(Arial_Black_16);
//  //dmd.drawChar(  x,  y, ‘any character', GRAPHICS_NORMAL );
  dmd.drawChar(4,1,'A', GRAPHICS_NORMAL);

  dmd.selectFont(SystemFont5x7);
  //dmd.drawString( x,y, "text,", z, GRAPHICS_NORMAL ); where x, y are the coodinates and z is the length of the string/text.
//  dmd.drawString(1,1,"Arduino", 7, GRAPHICS_NORMAL);
//  dmd.drawString(1,8,"uno", 4, GRAPHICS_NORMAL);
}
