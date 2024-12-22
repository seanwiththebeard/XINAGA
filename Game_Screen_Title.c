#include "Xinaga.h"
#include "GameData.h"

#if defined(__APPLE2__)
#pragma code-name (push, "LOWCODE")
#endif

#if defined (__NES__)
#pragma code-name (push, "TITLE")
//#pragma rodata-name (push, "STARTUP")
#pragma data-name (push, "XRAM")
#pragma bss-name (push, "XRAM")
#endif

screenName Update_Title()
{
  DrawBorder("@",12, 5, 16, 5, true);
  PrintString("Title Screen@", 14, 7, true, false);
  DrawBorder("@", 12, 19, 16, 5, true);
  PrintString("Press Space@", 14, 21, true, false);
  
  while (1)
  {
    UpdateInput();
    if (InputChanged())
      if (InputFire())
        return EditParty;
  }  
}