#include "Xinaga.h"

#if defined(__APPLE2__)
//#pragma code-name (push, "LOWCODE")
#endif

#if defined (__NES__)
#pragma code-name (push, "STARTUP")
//#pragma rodata-name (push, "STARTUP")
//#pragma data-name (push, "XRAM")
//#pragma bss-name (push, "XRAM")
#endif


sbyte key;
byte keyIgnore;
bool ChangedState;

#if __C64__
#include <joystick.h>
byte joyState;
byte joyStateLast;
byte joyTemp;
#endif

#if (__NES__)
// include CC65 NES Header (PPU)
#include <nes.h>
// include NESLIB header
#include "neslib.h"
char pad;
char padTemp;
char padStateLast;
#endif

void InitializeInput()
{
  #if __C64__
  joy_install(joy_static_stddrv);
  #endif
}

const byte* keycode = (byte*)0xC000;
const byte* keyflag = (byte*)0xC010;

bool InputChanged(void)
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

void UpdateInput(void)
{ 
  ++randseed;
  srand(randseed);
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
  
  #if (__NES__)
  padTemp = pad_poll(0);
  if (padTemp == pad)
    ChangedState = false;
  else
  {
    pad = padTemp;
    ChangedState = true;
    padStateLast = pad;
  }
  #endif
}

bool InputUp(void)
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
  
  #if (__NES__)
  return pad&PAD_UP;
  #endif
  
  return false;
}

bool InputDown(void)
{
  #if __C64__
  if (JOY_DOWN(joyState))
    return true;
  #endif
  #if defined(__APPLE2__)
  if (key == 's' || key == 'S')
    return true;
  #endif
  
  #if (__NES__)
  return pad&PAD_DOWN;
  #endif
  
  return false;
}

bool InputLeft(void)
{
  #if __C64__
  if (JOY_LEFT(joyState))
    return true;
  #endif
  
  #if defined(__APPLE2__)
  if (key == 'a' || key == 'A')
    return true;
  #endif
  
  #if (__NES__)
  return pad&PAD_LEFT;
  #endif
  return false;
}

bool InputRight(void)
{
  #if __C64__
  if (JOY_RIGHT(joyState))
    return true;
  #endif
  
  #if defined(__APPLE2__)
  if (key == 'd' || key == 'D')
    return true;
  #endif
  
  #if (__NES__)
  return pad&PAD_RIGHT;
  #endif
  return false;
}

bool InputFire(void)
{
  #if __C64__
  if (JOY_FIRE(joyState))
    return true;
  #endif
  #if defined(__APPLE2__)
  if (key == ' ')
    return true;
  #endif
  
  #if (__NES__)
  return pad&PAD_A;
  #endif
  return false;
}

void WaitForInput(void)
{
  bool ex = false;
  WriteLineMessageWindow("Press space to continue@", 0);
  
  while (!ex)
  {
    UpdateInput();
    if (InputFire())
      ex = true;
  }
}