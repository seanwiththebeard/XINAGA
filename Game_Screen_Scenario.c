#include "game.h"
#include "xinaga.h"

#include <stdio.h>
#include <stdlib.h>

#define height 16
#define width 16
#define posX 1
#define posY 1

#define consolePosX  1
#define consolePosY 12
#define consoleWidth 38
#define consoleHeight 12

byte QuestOrigin, QuestType, QuestGiver, QuestTarget, QuestLocation;
int randSeed = 0;

char *questOrigin[] = { "the castle,", "a tavern rumor,", "your library studies,", "a dream at the inn,"};
char *questGiver[][] = {
  			/*Castle*/	{"King", "Blacksmith", "Guildmaster", "People"},
  			/*Town Tavern*/	{"Tavern Owner", "Cloaked Figure", "Guildmaster", "People"},
  			/*Library Book*/{"History Book", "Librarian", "Scholar", "Cloaked Figure"},
  			/*Dream*/	{"Statue", "Restless Spirit", "Talking Animal", "Wise Tree"}
			};
char *questType[] = { "kill", "retrieve", "explore and map out", "solve the puzzle at", "visit", "play cards with"};
char *questTarget[][] = { //Point of Interest
  			/*Kill*/	{"Dragon", "Vampire", "Wizard", "Owlbear"},
			/*Retrieve*/	{"Scroll", "Gauntlet", "Orb", "Artifact"},
			/*Explore*/	{"Cavern", "Hidden Cellar", "Burial Site", "Treasure Room"},
			/*PuzzleSolve*/	{"Burial Site", "Shipwreck", "Magic Device", "Steam Device"},
			/*Visit*/	{"Burial Site", "Water's Edge", "Monument", "Wise Tree"},
			/*PlayCards*/	{"Hooded Figure", "Lost Knight", "Talking Animal", "Wizard"}
			};
char *questLocation[][] = { //Map Location
  			/*Kill*/	{"Forrest", "Dungeon", "Dwarven Tower", "Dimensional Rift"},
			/*Retrieve*/	{"Ruined Archive", "Dungeon", "Castle Basement", "Dwarven Tower"},
			/*Explore*/	{"Mobile Siege Engine", "Ruined Archive", "Dwarven Tower", "Dimensional Rift"},
			/*PuzzleSolve*/	{"Clock Tower", "Library", "Mobile Siege Engine", "Windmill"},
			/*Visit*/	{"Ruins", "Lake", "Open Plains", "Forrest"},
			/*Play Cards*/	{"Castle", "Forrest Clearing", "Dimensional Rift", "Tavern"}
			};



void GenerateContinent(int seed)
{
  srand(seed);
  QuestOrigin = rand() % 4;
  QuestType = rand() % 6;
  QuestGiver = rand() % 4;
  QuestTarget = rand() % 4;
  QuestLocation = rand() % 4;
  
  sprintf(strTemp, "Continent Seed: %d@", seed);
  WriteLineMessageWindow(strTemp, 0);
  sprintf(strTemp, "In %s@", questOrigin[QuestOrigin]);
  WriteLineMessageWindow(strTemp, 0);  
  sprintf(strTemp, "the %s sends you to@", questGiver[QuestOrigin][QuestGiver]);
  WriteLineMessageWindow(strTemp, 0);
  sprintf(strTemp, "%s the %s@", questType[QuestType], questTarget[QuestType][QuestTarget]);
  WriteLineMessageWindow(strTemp, 0);
  sprintf(strTemp, "of the %s@", questLocation[QuestType][QuestLocation]);
  WriteLineMessageWindow(strTemp, 0);
  WriteLineMessageWindow("@", 0);
  
  //Add Landmarks
  //Landmark 1 - Quest Origin
  //Landmark 2 - Quest Destination (Is this the same as the origin?)
  //Landmark 3 - Do we have a town/castle yet? Else random non-town; Ensure we have a ferry station to reach next continent
  //Landmark 4 - Do we have a dungeon yet? Else small outpost/encampment; Ensure we have somewhere to grind for exp/resources
  //Landmark 5 - Do we have an interesting non-combat area?
  //Random number 0-3 for additional dungeons/non-combat locations
}

screenName Update_Scenario()
{
  screenName nextScreen = Title;
  bool exit = false;
  DrawBorder("Scenario@",posX - 1, posY - 1, width + 2, height + 2, true);
  ResizeMessageWindow(consolePosX, consolePosY, consoleWidth, consoleHeight);
  
  GenerateContinent(randSeed);
  
  while (!exit)
  {
    randSeed = rand();
    //GenerateContinent(randSeed);
    
    UpdateInput();
    if (InputChanged())
    {
      if (InputUp())
      {
        //++randSeed;
        GenerateContinent(randSeed);
      }
      if (InputFire())
      {
        exit = true;
      }
    }
  }

  return nextScreen;
}