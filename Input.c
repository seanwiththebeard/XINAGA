#include "xinaga.h"
//#include <conio.h> //for cgetc() and kbhit()
sbyte key = 0;
byte keyIgnore = 0;

#if __C64__
//#include <joystick.h>
//byte joyState = 0;
//byte joyStateLast = 0;
//byte joyTemp;
#endif

#if defined(__APPLE2__)

#endif

void InitializeInput()
{
  #if __C64__
  //joy_install(joy_static_stddrv);
  #endif
}

byte* keycode = (byte*)0xC000;
byte* keyflag = (byte*)0xC010;

void waitforkey()
{
  //while(keyflag[0] & 128){}
}
void UpdateInput()
{
  while (keycode[0] & 128)
  {
    SetChar('x', COLS - 1, 1);
  }
  STROBE(0xc010);

  key = keycode[0];
  SetChar(key, COLS - 1, 1);
  #if __C64__
  /*joyTemp = joy_read(0);

  if (joyState == joyTemp)
  {
    ChangedState = false;
  }
  else
  {
    joyState = joyTemp;
    ChangedState = true;
    joyStateLast = joyState;
  }*/
  #endif

  #if defined(__APPLE2__)


  #endif
}

bool InputUp()
{
  #if __C64__
  if (JOY_UP(joyState))
    return true;
  #endif

  #if defined(__APPLE2__)
  if (key == 'w')
  {
    return true;
  }
  #endif
  return false;
}

bool InputDown()
{
  #if __C64__
  if (JOY_DOWN(joyState))
    return true;
  #endif
  #if defined(__APPLE2__)
  if (key == 's')
    return true;
  #endif
  return false;
}

bool InputLeft()
{
  #if __C64__
  if (JOY_LEFT(joyState))
    return true;
  #endif
  #if defined(__APPLE2__)
  if (key == 'a')
    return true;
  #endif
  return false;
}

bool InputRight()
{
  #if __C64__
  if (JOY_RIGHT(joyState))
    return true;
  #endif
  #if defined(__APPLE2__)
  if (key == 'd')
    return true;
  #endif
  return false;
}

bool InputFire()
{
  #if __C64__
  if (JOY_FIRE(joyState))
    return true;
  #endif
  #if defined(__APPLE2__)
  if (key == ' ')
    return true;
  #endif
  return false;
}