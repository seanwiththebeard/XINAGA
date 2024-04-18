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
char *questGiver[][] = {
  			/*Castle*/	{"King", "Blacksmith", "Guildmaster", "People"},
  			/*Tavern*/	{"Tavern Owner", "Cloaked Figure", "Guildmaster", "People"},
  			/*Library Book*/{"History Book", "Librarian", "Scholar", "Cloaked Figure"},
  			/*Dream*/	{"Statue", "Restless Spirit", "Talking Animal", "Wise Tree"}
			};
char *questType[] = { "kill", "retrieve", "explore and map out", "solve the puzzle at", "visit"};
char *questTarget[][] = {
  			/*Kill*/	{"Dragon", "Vampire", "Wizard", "Owlbear"},
			/*Retrieve*/	{"Scroll", "Gauntlet", "Orb", "Artifact"},
			/*Explore*/	{"Cavern", "Cellar", "Crypt", "Swamp"},
			/*PuzzleSolve*/	{"Graveyard", "Shipwreck", "Clocktower", "Steam Device"},
			/*Visit*/	{"Tombstone", "Water's Edge", "Monument", "Wise Tree"}
			};
char *questLocation[][] = {
  			/*Kill*/	{"Forrest", "Dungeon", "Dwarven Tower", "Dimensional Rift"},
			/*Retrieve*/	{"Ruined Archive", "Dungeon", "Castle Basement", "Dwarven Tower"},
			/*Explore*/	{"Mobile Siege Engine", "Ruined Archive", "Dwarven Tower", "Dimensional Rift"},
			/*PuzzleSolve*/	{"Clock Tower", "Library", "Mobile Siege Engine", "Windmill"},
			/*Visit*/	{"Ruins", "Lake", "Open Plains", "Forrest"},
			};



void GenerateContinent()
{
  QuestOrigin = rand() % 4;
  QuestType = rand() % 5;
  QuestGiver = rand() % 4;
  QuestTarget = rand() % 4;
  QuestLocation = rand() % 4;
  
  sprintf(strTemp, "The quest is initiated in@");
  WriteLineMessageWindow(strTemp, 0);
  sprintf(strTemp, "%s@", questOrigin[QuestOrigin]);
  WriteLineMessageWindow(strTemp, 0);  
  sprintf(strTemp, "The %s asks you to@", questGiver[QuestOrigin][QuestGiver]);
  WriteLineMessageWindow(strTemp, 0);
  sprintf(strTemp, "%s the @", questType[QuestType]);
  WriteLineMessageWindow(strTemp, 0);
  sprintf(strTemp, "%s at the %s@", questTarget[QuestType][QuestTarget], questLocation[QuestType][QuestLocation]);
  WriteLineMessageWindow(strTemp, 0);
  WriteLineMessageWindow("@", 0);
  
  //Add Landmarks
  //Landmark 1 - Quest Origin
  //Landmark 2 - Quest Destination
  //Landmark 3 - Do we have a town/castle yet? Else random non-town; Ensure we have a ferry station to reach next continent
  //Landmark 4 - Do we have a dungeon yet? Else small outpost/encampment; Ensure we have somewhere to grind for exp/resources
  //Landmark 5 - Do we have an interesting non-combat area?
  //Random number 0-3 for additional dungeons/non-combat locations
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