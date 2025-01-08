#include "Xinaga.h"
#include "GameData.h"

//#link "XINAGA_Graphics.c"
//#link "XINAGA_Input.c"
//#link "XINAGA_FileIO.c"
//#link "XINAGA_Audio.c"
//#link "XINAGA_Console.c"
//#link "Game_Demo.c"
//#link "Game_System.c"
////#link "Game_Screen_Title.c"
////#link "Game_Screen_AddCharacter.c"
//#link "Game_Screen_Map.c"
////#link "Game_Screen_Combat.c"
////#link "Game_Screen_MapGen.c"
////#link "Game_Screen_Scenario.c"

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "msxbios.h"
//#link "msxbios.c"

//typedef uint8_t byte;
//typedef uint16_t word;
//typedef uint8_t bool;

void main(void)
{
  byte x, y = 0;
  CLS();
  
  for (y = 0; y < 16; ++y)
    for (x = 0; x < 16; ++x)
      //SetChar(x + 16*y, x, y);
  
  //LoadMap();
  //Update_MapGen();
  //Update_Scenario();
  InitializeInput();
  InitializeGraphics();
  ClearScreen();
  ResizeMessageWindow(1, 1, 10, 15);
  WriteLineMessageWindow("Hello@", 0);
  while (1)
  {
  WriteLineMessageWindow("Hello@", 0);
    
    //for (y = 0; y < 16; ++y)
      //for (x = 0; x < 16; ++x)
        //SetChar(x + 16*y, x, y);
  }
}
