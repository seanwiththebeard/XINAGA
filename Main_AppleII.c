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
//#link "Game_Screen_AddCharacter.c"
//#link "Game_Screen_Map.c"
//#link "Game_Screen_Combat.c"
////#link "Game_Screen_MapGen.c"
////#link "Game_Screen_Scenario.c"

#if defined(__APPLE2__)
//#resource "apple2-xinaga.cfg"
//#resource "apple2.rom"
#pragma code-name (push, "LOWCODE")
#define CFGFILE apple2-xinaga.cfg
#endif

screenName Update_Title(){return EditParty;}
screenName Update_MapGen(){return EditParty;}
screenName Update_Scenario(){return EditParty;}
//screenName Update_Combat(){return 0;}

//void LoadMap(){}
//screenName MapUpdate(){return 0;}

//void Demo(void);
void main()
{
  //STROBE(0xC080);
  
  Demo();
}
#if defined(__APPLE2__)
#pragma code-name (pop)
#endif


#pragma data-name (push, "HGR")
//char hgr[0x2000] = {};
#pragma data-name (pop)
