#include "Xinaga.h"
#include "GameData.h"
#include "neslib.h"


//#resource "neslib2_.cfg"
//#define CFGFILE neslib2_.cfg
// VRAM buffer module
#include "vrambuf.h"
//#link "vrambuf.c"

// bank-switching configuration
#define NES_MAPPER 4		// Mapper 4 (MMC3)
#define NES_PRG_BANKS 4		// # of 16KB PRG banks
#define NES_CHR_BANKS 2		// # of 8KB CHR banks

//#link "XINAGA_Graphics.c"
//#link "XINAGA_Input.c"
//#link "XINAGA_FileIO.c"
//#link "XINAGA_Audio.c"
//#link "XINAGA_Console.c"

//#link "Game_Demo.c"
//#link "Game_System.c"

//#link "Game_Screen_Title.c"
//#link "Game_Screen_AddCharacter.c"
//#link "Game_Screen_Map.c"
//#link "Game_Screen_Combat.c"
//#link "Game_Screen_MapGen.c"
//#link "Game_Screen_Scenario.c"

//#link "chr_generic.s"

//void Demo(void);
void main(void)
{
      MMC3_PRG_8000(1);
  
  //byte x, y;
  //InitializeGraphics();
  //ClearScreen();
  //for (x = 0; x < 16; ++x)
    //for (y = 0; y < 16; ++y)
      //SetChar('z',x, y);
  //InitializeGraphics();
  //ClearScreen();    
  //ResizeMessageWindow(3, 3, 12, 7);
  
  //ResetMenu("Edit Party@",1, 1, 16, 9, 9);
  //SetMenuItem(0, "Create@");
  //SetMenuItem(1, "Delete@");
  //SetMenuItem(2, "Add@");
  //SetMenuItem(3, "Remove@");
  //SetMenuItem(4, "Begin Adventure@");
  //SetMenuItem(5, "Credits@");
  //SetMenuItem(6, "Combat@");
  //SetMenuItem(7, "Map Gen@");
  //SetMenuItem(8, "Scenario Gen@");
  
      
  Demo();
  while(1){};
}

