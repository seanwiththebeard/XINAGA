#if defined(__APPLE2__)
#pragma code-name (push, "LC")
#endif

#include <stdio.h>
#include <stdlib.h>
#include "Xinaga.h"
#include "GameData.h"

#define height 16
#define width 16
#define posX 1
#define posY 1

#define consolePosX  1
#define consolePosY 18
#define consoleWidth 28
#define consoleHeight 6

#define menuPosX  2 + ROWS - (ROWS - consoleWidth)
#define menuPosY consolePosY
#define menuWidth 8
#define menuHeight consoleHeight
#define menuCount 4

//Quests are like stars in Mario 64
//Each world is like a level, each continent/quest is like a star
//100 coin star?

byte QuestOrigin = 0;
byte QuestType = 0;
byte QuestGiver = 0;
byte QuestTarget = 0;
byte QuestLocation = 0;
//byte randSeed = 0;

char *questOrigin[] = { "the castle", "a tavern rumor", "your library studies", "a dream at the inn"};
//Map Location Types (rule of fours, four tileset variations of each of four options)

//Primary Settlement: 	
//			Castle		
//			City		
//			Seaport		
//			Outpost		

//Secondary Settlement:
//			Village		
//			Library		Scholar Library, Wizard's Collection, War Records, Ruined Archive
//			Inn		
//			Abandoned Cabin	

//Dungeon:		Cavern		
//			Tower		
//			Ruins		
//			Portal		Crystal Rift, Nightmare, Clouds, Lunar

//Landmark:		Forrest		Open Fields, Dense Woods, Swamp, Giant Stump
//			Seaside		Dock, Beach, Waterfall, Lake
//			Shipwreck	Ship, Airship, Siege Engine, Leviathan Fossil
//			Tomb		Pyramid, Crypt, Graveyard, Catacombs

char *questGiver[][] = {
  /*Castle*/	{"King", "Blacksmith", "Guildmaster", "Town Council"},
  /*Town Tavern*/	{"Tavern Owner", "Cloaked Figure", "Guildmaster", "People"},
  /*Library Book*/{"History Book", "Librarian", "Scholar", "Cloaked Figure"},
  /*Dream*/	{"Statue", "Restless Spirit", "Talking Animal", "Wise Tree"}
};
char *questType[] = { "kill", "retrieve", "explore and map out", "solve the puzzle in", "visit", "play cards with"};
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
  srand (rand() % 32767);
  
  QuestOrigin = rand() % 4;
  QuestType = rand() % 6;
  QuestGiver = rand() % 4;
  QuestTarget = rand() % 4;
  QuestLocation = rand() % 4;
  
  
  sprintf(strTemp, "In %s, the %s asks you to %s the %s at the %s@", questOrigin[QuestOrigin], questGiver[QuestOrigin][QuestGiver], questType[QuestType], questTarget[QuestType][QuestTarget], questLocation[QuestType][QuestLocation]);
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
  int seed = 0;
  screenName nextScreen = Title;
  bool exit = false;
  ResizeMessageWindow(consolePosX, consolePosY, consoleWidth, consoleHeight);
  {
    ResetMenu("Scenario@", menuPosX, menuPosY, menuWidth, menuHeight, menuCount);
    SetMenuItem(0, "Next@");
    SetMenuItem(1, "Last@");
    SetMenuItem(2, "Go@");
    SetMenuItem(3, "End@");
    while (!exit)
    {
      sprintf(strTemp, "Continent Quest Seed: %d @", seed);
      SetLineMessageWindow(strTemp, 0);
      
      switch (GetMenuSelection())
      {
        case 0:
          ++seed;
          if (seed < 0)
            seed = 0;
          break;
        case 1:
          --seed;
          if (seed < 0)
            seed = 32767;
          break;
        case 2:
          GenerateContinent(seed);
          ++seed;
          if (seed < 0)
            seed = 0;
          break;
        case 3:
          exit = true;
          break;
        default:
          break;
      }

    }
  }
  return nextScreen;
}

#if defined(__APPLE2__)
#pragma code-name (pop)
#endif