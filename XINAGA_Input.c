#if defined(__APPLE2__)
#pragma code-name (push, "LC")
#endif

#include <conio.h> //for cgetc() and kbhit()
#include "Xinaga.h"

sbyte key = 0;
byte keyIgnore = 0;
bool ChangedState = true;

#if __C64__
#include <joystick.h>
byte joyState = 0;
byte joyStateLast = 0;
byte joyTemp = 0;
#endif

void InitializeInput()
{
  #if __C64__
  joy_install(joy_static_stddrv);
  #endif
}

byte* keycode = (byte*)0xC000;
byte* keyflag = (byte*)0xC010;

bool InputChanged()
{
  #if defined(__APPLE2__)
  if(keycode[0] & 128)
  {
    ChangedState = false;
    STROBE (0xc010);
  }
  else
    ChangedState = true;
  #endif
  return ChangedState;
}

void UpdateInput()
{ 
  #if __C64__
  joyTemp = joy_read(0);

  if (joyState == joyTemp)
  {
    ChangedState = false;
  }
  else
  {
    joyState = joyTemp;
    ChangedState = true;
    joyStateLast = joyState;
  }
  #endif

  #if defined(__APPLE2__)
  cgetc();
  while(keycode[0] & 128){}
  key = keycode[0];
  SetChar(key, COLS - 1, 1);
  #endif
}

bool InputUp()
{
  #if __C64__
  if (JOY_UP(joyState))
    return true;
  #endif

  #if defined(__APPLE2__)
  if (key == 'w' || key == 'W')
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
  if (key == 's' || key == 'S')
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
  if (key == 'a' || key == 'A')
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
  if (key == 'd' || key == 'D')
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