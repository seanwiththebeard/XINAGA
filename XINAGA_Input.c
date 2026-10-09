#include "Xinaga.h"

#if defined(__APPLE2__)
#pragma code-name (push, "XINAGA")
#pragma rodata-name (push, "XINAGA")

#define keycode ((byte*)0xC000)
#define keyflag ((byte*)0xC010)
//const byte* keycode = (byte*)0xC000;
//const byte* keyflag = (byte*)0xC010;
#endif

#if defined (__NES__)
#pragma code-name (push, "XINAGA")
#pragma rodata-name (push, "XINAGA")
//#pragma data-name (push, "XRAM")
//#pragma bss-name (push, "XRAM")
#include <nes.h>
#include "neslib_mod.h"
char pad;
char padTemp;
char padStateLast;
#endif

#if defined (__C64__)
#pragma code-name (push, "XINAGA_INPUT")
#pragma rodata-name (push, "XINAGA_INPUT")
#include <conio.h>
//#define keycode ((byte*)0x00C5)
#endif

#if (MSX)
#include "msxbios.h"
char pad;
char padTemp;
char padStateLast;
char trigA;
char trigATemp;
char trigAStateLast;
#endif

sbyte key, keyTemp;
byte keyIgnore;
bool ChangedState;

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
  #if __C64__
  if(kbhit())
  {
    key = cgetc();
    ChangedState = true;
  }
  else
  {
    key = 255;
    ChangedState = false;
    return;
  }
  #endif

  #if defined(__APPLE2__)
  if(kbhit())
    cgetc();
  else
  {
    key = 255;
    return;
  }
  //while(keycode[0] & 128){}
  key = keycode[0];
  //SetChar(key, COLS - 1, 1);
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

  #if (MSX)
  padTemp = GTSTCK(STCK_Joy1);
  trigATemp = GTTRIG(TRIG_Joy1_A);
  if ((padTemp == pad) && (trigATemp == trigA))
    ChangedState = false;
  else
  {
    pad = padTemp;
    trigA = trigATemp;
    ChangedState = true;
    padStateLast = pad;
    trigAStateLast = trigA;
  }
  #endif
}
bool InputUp(void)
{
  #if __C64__
  if ((key == 'w' || key == 'W'))
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

  #if (MSX)
  if (pad == STCK_N)
    return true;
  #endif

  return false;
}
bool InputDown(void)
{
  #if __C64__
  if ((key == 's' || key == 'S'))
    return true;
  #endif
  
  #if defined(__APPLE2__)
  if (key == 's' || key == 'S')
    return true;
  #endif

  #if (__NES__)
  return pad&PAD_DOWN;
  #endif

  #if (MSX)
  if (pad == STCK_S)
    return true;
  #endif

  return false;
}
bool InputLeft(void)
{
  #if __C64__
  if ((key == 'a' || key == 'A'))
    return true;
  #endif

  #if defined(__APPLE2__)
  if (key == 'a' || key == 'A')
    return true;
  #endif

  #if (__NES__)
  return pad&PAD_LEFT;
  #endif

  #if (MSX)
  if (pad == STCK_W)
    return true;
  #endif

  return false;
}
bool InputRight(void)
{
  #if __C64__
  if ((key == 'd' || key == 'D'))
    return true;
  #endif

  #if defined(__APPLE2__)
  if (key == 'd' || key == 'D')
    return true;
  #endif

  #if (__NES__)
  return pad&PAD_RIGHT;
  #endif

  #if (MSX)
  if (pad == STCK_E)
    return true;
  #endif

  return false;
}
bool InputFire(void)
{
  #if __C64__
  if (key == ' ')
    return true;
  #endif
  
  #if defined(__APPLE2__)
  if (key == ' ')
    return true;
  #endif

  #if (__NES__)
  return pad&PAD_A;
  #endif

  #if (MSX)
  if (GTTRIG(TRIG_Joy1_A))
  {
    ChangedState = true;
    return true;
  }
  #endif

  return false;
}
/*
void WaitForInput(void)
{
  bool ex = false;
  //WriteLineMessageWindow("Press space to continue", 0);

  while (!ex)
  {
    UpdateInput();
    if (InputFire())
      ex = true;
  }
}*/
