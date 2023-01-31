#include "xinaga.h"

//#include <stdlib.h>
//#include <string.h>
#include <conio.h>
//#include <apple2.h>

#if __C64__
#include <joystick.h>
byte joyState = 0;
byte joyStateLast = 0;
byte joyTemp;
#endif
bool ChangedState = false;

#if defined(__APPLE2__)
byte key = 0;
byte lastKey = 1;
#endif

void InitializeInput()
{
  #if __C64__
  joy_install(joy_static_stddrv);
  #endif
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
  if (kbhit())
  {
    key = cgetc();
    SetChar(key, COLS - 1, 0);
  }
  else
    key = 0;
  #endif
}

bool NoInput()
{
  #if __C64__
  if (joy_read(0) == 0)
    return true;
  #endif
  #if defined(__APPLE2__)
  if (!kbhit())
    return true;
  #endif
  return false;
}

bool InputChanged()
{
  if (!NoInput())
    return ChangedState;
  else
    return false;
}
bool InputUp()
{
  #if __C64__
  if (JOY_UP(joyState))
    return true;
  #endif

  #if defined(__APPLE2__)
  if (key == 'w')
    return true;
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