#include "Xinaga.h"
#include "GameData.h"

//#link "XINAGA_Graphics.c"
//#link "XINAGA_Input.c"
//#link "XINAGA_FileIO.c"
//#link "XINAGA_Audio.c"
//#link "XINAGA_Console.c"
//#link "Game_Demo.c"
//#link "Game_System.c"
//#link "Game_Screen_Title.c"
//#link "Game_Screen_AddCharacter.c"
////#link "Game_Screen_Map.c"
////#link "Game_Screen_Combat.c"
////#link "Game_Screen_MapGen.c"
//#link "Game_Screen_Scenario.c"

#include "msxbios.h"
//#link "msxbios.c"

//typedef uint8_t byte;
//typedef uint16_t word;
//typedef uint8_t bool;

//byte strTemp[ConsoleBufferLength];
//uint16_t randseed;

void main(void)
{

  //ResizeMessageWindow(1, 1, 10, 15);
  //WriteLineMessageWindow("Hello@", 0);
  Demo();
}
