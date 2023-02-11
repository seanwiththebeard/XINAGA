#include "xinaga.h"
#include "game.h"

screenName currentScreen = Title;

void SwitchScreen(screenName screen)
{
  //ScreenDisable();
  ClearScreen();
  //Load specified screen
  UpdateInput();
  currentScreen = screen;
  //ScreenEnable();
  
  switch (currentScreen)
  {
    case Title:
      //currentScreen = Update_Title();
      break;
    case Credits:
      //currentScreen = Update_Credits();
      break;
    case EditParty:
      //currentScreen = DrawAddCharacterScreen();
      break;
    case Map:
      //LoadMap();
      //currentScreen = MapUpdate();
      break;
    case Combat:
      //currentScreen = Update_Combat();
      break;
    case Menu:
      break;
    default:
      break;
  }
  
  SwitchScreen(currentScreen);
}