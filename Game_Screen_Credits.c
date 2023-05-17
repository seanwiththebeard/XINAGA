#include <stdio.h>
#include "game.h"
#include "xinaga.h"
#if defined(__C64__)
byte yscroll;
//byte sinevalue[32] = {12,14,17,19,20,22,23,24, 24,24,23,22,20,19,17,14, 12,10,7,5,4,2,1,0, 0,0,1,2,4,5,7,10};
byte xcredit;
byte delay;
byte xoff;
bool skipline;

#define linecount 14

char CreditsLines[][20] =
{
  "Credits@",
  "(c) 2022@",
  "All rights reserved@",
  
  "Lead Designer@",
  "and Programmer:@",
  "Somebody@",
  
  "Makes use of the@",
  "following@",
  "libraries@",
  
  "C/ASM SID Player:@",
  "https://github.com/@",
  "solidcore-commodore@",
  
  "Special Thanks@",
  "YOU!@"
  };

void ScrollUp()
{
  if (xcredit == linecount)
  {
    ++delay;
  }
  {
    //#if defined (__C64__)
    Scroll(up);
    //#endif
    if (xcredit < linecount)
    {
      if (xoff %4 != 0)
      {
        PrintString(CreditsLines[xcredit], xcredit % 3, ROWS - 1, true, false);
        ++xcredit;   
      }
      else
        PrintString("@", xcredit % 3, 0, ROWS - 1, false);
    }
    ++xoff;
  }
  
}

screenName Update_Credits()
{
  //byte temp = VIC.ctrl1;
  screenName nextScreen = Title;
  bool exit = false;
  yscroll = 0;
  xcredit = 0;
  delay = 0;
  xoff = 0;
  skipline = false;
  
  ClearScreen();
  //PlaySID();
  
  while (!exit)
  {
    #if defined (__C64__)
    UpdateInput();
    #endif
    ScrollUp();
    
    if ((InputChanged() && InputFire()) || (delay > 35))
        exit = true;
  }
  //StopSID();
  //VIC.ctrl1 = temp;
  return nextScreen;
}
#endif
#if defined(__APPLE2__)
screenName Update_Credits()
{
  return Title;
}
#endif