#include "game.h"
#include "xinaga.h"

#include <stdio.h>
#include <stdlib.h>

#define height 16
#define width 16
#define posX 1
#define posY 1

byte QuestOrigin, QuestType, QuestGiver, QuestTarget, QuestLocation;

char *questOrigin[] = { "the castle", "a tavern rumor", "a library book", "a dream at the inn"};
char *questGiver[] = {"King", "Blacksmith", "Guildmaster", "People"};
char *questType[] = { "kill", "retrieve", "explore and map out", "solve the puzzle at", "visit"};
char *questTarget[][] = {
  			/*Kill*/	{"Dragon", "Vampire", "Wizard", "Owlbear"},
			/*Retrieve*/	{"Scroll", "Gauntlet", "Orb", "Artifact"},
			/*Explore*/	{"Cavern", "Cellar", "Crypt", "Swamp"},
			/*PuzzleSolve*/	{"Graveyard", "Shipwreck", "Clocktower", "Steam Device"},
			/*Visit*/	{"Tombstone", "Water's Edge", "Monument", "Wise Tree"}
			};
char *questLocation[][] = {
  			/*Kill*/	{"Forrest", "Dungeon", "Tower", "Dimensional Rift"},
			/*Retrieve*/	{"Ruined Archive", "Dungeon", "Castle Basement", "Tower"},
			/*Explore*/	{"Mobile Siege Engine", "Ruined Archive", "Tower", "Dimensional Rift"},
			/*PuzzleSolve*/	{"pA", "pB", "pC", "pD"},
			/*Visit*/	{"Ruins", "Lake", "Open Plains", "Forrest"},
			};



void GenerateContinent()
{
  //Which Quest Origin? Castle, Town, Library, Dream
  QuestOrigin = rand() % 4;
  QuestType = rand() % 5;
  QuestGiver = rand() % 4;
  QuestTarget = rand() % 4;
  QuestLocation = rand() % 4;
  
  sprintf(strTemp, "The quest is initiated in@");
  WriteLineMessageWindow(strTemp, 0);
  sprintf(strTemp, "%s@", questOrigin[QuestOrigin]);
  WriteLineMessageWindow(strTemp, 0);  
  sprintf(strTemp, "The %s asks you to@", questGiver[QuestGiver]);
  WriteLineMessageWindow(strTemp, 0);
  sprintf(strTemp, "%s the @", questType[QuestType]);
  WriteLineMessageWindow(strTemp, 0);
  sprintf(strTemp, "%s at the %s@", questTarget[QuestType][QuestTarget], questLocation[QuestType][QuestLocation]);
  WriteLineMessageWindow(strTemp, 0);
  WriteLineMessageWindow("@", 0);
  
}

screenName Update_Scenario()
{
  screenName nextScreen = Title;
  bool exit = false;
  ResizeMessageWindow(1, 16, 30, 8);
  BlankMessageWindow();
  DrawBorder("Scenario@",posX - 1, posY - 1, width + 2, height + 2, true);
  while (!exit)
  {
  GenerateContinent();
    
    UpdateInput();
    if (InputChanged())
    {
      if (InputUp())
      {
        
      }
      if (InputFire())
      {
        exit = true;
      }
    }
  }

  return nextScreen;
}