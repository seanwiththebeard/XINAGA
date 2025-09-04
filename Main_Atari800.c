#include "Xinaga.h"
#include "GameData.h"
//#link "XINAGA_Graphics.c"
//#link "XINAGA_Input.c"
////#link "XINAGA_FileIO.c"
////#link "XINAGA_Audio.c"
//#link "XINAGA_Console.c"
////#link "Game_Demo.c"
////#link "Game_System.c"
////#link "Game_Screen_Title.c"
////#link "Game_Screen_AddCharacter.c"
////#link "Game_Screen_Map.c"
////#link "Game_Screen_Combat.c"
////#link "Game_Screen_MapGen.c"
////#link "Game_Screen_Scenario.c"

uint16_t randseed;
byte strTemp[ConsoleBufferLength];


void main(void)
{
   byte x, y;
  
  //InitializeGraphics();
  
  ResizeMessageWindow(consolePosX, ROWS - 9, consoleWidth, 6);
   //for (y = 0; y < 16; ++y)
    
  
  while(1)
  {
    for (x = 0; x < 16; ++x)
      SetChar(x + y*16, x + viewportPosX, y + viewportPosY);
    ++y;
  }
}
