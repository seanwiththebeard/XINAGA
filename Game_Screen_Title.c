#include "game.h"
#include "xinaga.h"

#include <stdio.h>
#include <stdlib.h>


screenName Update_Title()
{
  screenName nextScreen = EditParty;
  bool exit = false;
  DrawBorder("@",12, 5, 16, 5, true);
  PrintString("Title Screen@", 14, 7, true, false);
  DrawBorder("@", 12, 19, 16, 5, true);
  PrintString("Press Space@", 14, 21, true, false);
  
  sprintf(strTemp, "Digit: %d text@", 5);
  PrintString(strTemp, 0, 0, true, false);
  
  //PlaySID();
  
  while (!exit)
  {
    UpdateInput();
    if (InputChanged())
      if (InputFire())
        exit = true;
  }
  
  return nextScreen;
}