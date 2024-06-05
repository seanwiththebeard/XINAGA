#if defined(__APPLE2__)
#pragma code-name (push, "LOWCODE")
#endif

#include <stdio.h>
#include <stdlib.h>
#include "Xinaga.h"
#include "GameData.h"

screenName Update_Title()
{
  screenName nextScreen = EditParty;
  bool exit = false;
  DrawBorder("@",12, 5, 16, 5, true);
  PrintString("Title Screen@", 14, 7, true, false);
  DrawBorder("@", 12, 19, 16, 5, true);
  PrintString("Press Space@", 14, 21, true, false);
  
  while (!exit)
  {
    UpdateInput();
    if (InputChanged())
      if (InputFire())
        exit = true;
  }
  
  return nextScreen;
}

#if defined(__APPLE2__)
#pragma code-name (pop)
#endif