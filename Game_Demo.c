#include "Xinaga.h"
#include "GameData.h"

#if defined(__APPLE2__)
#pragma code-name (push, "LOWCODE")
#endif

#if defined (__NES__)
//#pragma code-name (push, "CODE")
//#pragma rodata-name (push, "CODE")
#endif

#define DefaultScreen EditParty
screenName currentScreen;
byte strTemp[ConsoleBufferLength];

const RaceDescriptionDef const RaceDescription[] = 
{
  {"Human@", 255},
  {"Elf@", 6}, 
  {"Dwarf@", 255}, 
  {"Construct@", 6}
};


const ClassDescriptionDef const ClassDescription[]= 
{
  {"Fighter@", 8},
  {"Wizard@", 4}, 
  {"Cleric@", 6}, 
  {"Thief@", 4},
  {"Ranger@", 8},
  {"Monk@", 6},
  {"Bard@", 4},
  {"Construct@", 8} // Never levels up, strong early on but expensive to upgrade stats
  
};

const sbyte const AbilityModifier[20] = {-3, -3, -3, -3, -3, -2, -2, -1, -1, -1, 0, 0, 0, 0, +1, +1, +1, +2, +2, +3};

#if defined (__NES__)

void SelectBank()
{
    MMC3_PRG_8000(currentScreen);
}
#endif


void SwitchScreen(screenName screen)
{
  //ResizeMessageWindow(1, 1, 10, 15);
  //WriteLineMessageWindow("Hello@", 0);
  
  //ScreenDisable();
  ClearScreen();
  //Load specified screen
  //UpdateInput();
  currentScreen = screen;
  //ScreenEnable();
  #if defined (__NES__)
  SelectBank();
  #endif

  switch (currentScreen)
  {
    case Title:
      currentScreen = Update_Title();
      break;
    case EditParty:
      currentScreen = DrawAddCharacterScreen();
      break;
    case Map:
      currentScreen = MapUpdate();
      break;
    case Combat:
      currentScreen = Update_Combat();
      break;
    case MapGen:
      currentScreen = Update_MapGen();
      break;
    case Scenario:
      currentScreen = Update_Scenario();
    default:
      currentScreen = DefaultScreen;
      break;
  }
  
  SwitchScreen(currentScreen);
  ScreenFadeOut();
}

void Demo()
{  
  InitializeInput();
  InitializeGraphics();
  ClearScreen();
  
  #if defined(__NES__)
  MMC3_PRG_8000(0);
  #endif
  LoadMap();
  //currentScreen = DefaultScreen;
  
  while(1)
  {
    SwitchScreen(currentScreen);
  }
}
