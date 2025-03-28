#include "Xinaga.h"
#include "GameData.h"

#if defined(__APPLE2__)
#pragma code-name (push, "LC")
#endif

#if defined (__NES__)
#pragma code-name (push, "SCENARIO")
#pragma rodata-name (push, "SCENARIO")
#include "neslib.h"

//#pragma data-name (push, "XRAM")
//#pragma bss-name (push, "XRAM")
#endif


/*
World Structure
	World Map - Surface
        World Map - Sky (Floating Continents, Lunar Surface, , Dream Dimension)
        World Map - Underworld (Magma Cave, Ancient Ruins, Ocean Floor, )
        Orbiting Ship (Airship Dreadnaught, Alien Satellite, Flying Turtle, Floating Tower)
        Town A
        Town B
        Town C
        Town D
        Dungeon A
        Dungeon B
        Dungeon C
        Dungeon D
        Landmark A
        Landmark B
        Landmark C
        Landmark D
*/

/*
Quest Structrue (sine wave going through diagram)

							<-Sky-> (transition?)
Start |		TownDungeonLandmark A	(transition) TownDungeonLandmark...D (transition?) | Endgame
							<-Underworld-> (transition?)

You might get to the endgame from the last continent, from the sky, or from the underworld
Transition types - Walk Overland, Boat (Given or Built), Through Dungeon

*/


byte QuestOrigin;
byte QuestType;
byte QuestGiver;
byte QuestTarget;
byte QuestLocation;

//Quests are like stars in Mario 64
//Each world is like a level, each continent/quest is like a star
//100 coin star?

const char* const questOrigin[4] = { "the castle", "a tavern rumor", "your library studies", "a dream at the inn"};
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

const char* const questGiver[4][4] = {
  /*Castle*/		{"King", "Blacksmith", "Guildmaster", "Council"},
  /*Town Tavern*/	{"Innkeeper", "Shadow", "Guildmaster", "People"},
  /*Library Book*/	{"History Book", "Librarian", "Scholar", "Shadow"},
  /*Dream*/		{"Statue", "Spirit", "Animal", "Wise Tree"}
};
const char* const questType[6] = 	{ "kill", "retrieve", "map out", "solve a puzzle in", "visit", "play cards with"};
const char* const questTarget[6][4] = { //Point of Interest
  /*Kill*/		{"Dragon", "Vampire", "Wizard", "Owlbear"},
  /*Retrieve*/		{"Scroll", "Gauntlet", "Orb", "Artifact"},
  /*Explore*/		{"Cavern", "Hidden Cellar", "Burial Site", "Treasure Room"},
  /*PuzzleSolve*/	{"Crypt", "Shipwreck", "Magic Device", "Steam Device"},
  /*Visit*/		{"Crypt", "Water's Edge", "Monument", "Wise Tree"},
  /*PlayCards*/		{"Shadow", "Lost Knight", "Animal", "Wizard"}
};
const char* const questLocation[6][4] = { //Map Location
  /*Kill*/		{"Forrest", "Dungeon", "Dwarven Tower", "Wormhole"},
  /*Retrieve*/		{"Archive", "Dungeon", "Oubliette", "Dwarven Tower"},
  /*Explore*/		{"Siege Engine", "Archive", "Dwarven Tower", "Wormhole"},
  /*PuzzleSolve*/	{"Clock Tower", "Library", "Siege Engine", "Windmill"},
  /*Visit*/		{"Ruins", "Lake", "Open Plains", "Forrest"},
  /*Play Cards*/	{"Castle", "Forrest", "Wormhole", "Tavern"}
};

void setSeed(uint16_t seed)
{
  srand(seed);
  srand (rand());
}

void GenerateContinent()
{
  QuestOrigin = rand() % 4;
  QuestType = rand() % 6;
  QuestGiver = rand() % 4;
  QuestTarget = rand() % 4;
  QuestLocation = rand() % 4;

  sprintf(strTemp, "In %S, the %S asks you to %S the %S at the %S@", questOrigin[QuestOrigin], questGiver[QuestOrigin][QuestGiver], questType[QuestType], questTarget[QuestType][QuestTarget], questLocation[QuestType][QuestLocation]);
  WriteLineMessageWindow(strTemp, 1);
  WriteLineMessageWindow("@", 0);

  //Add Landmarks
  //Landmark 1 - Quest Origin
  //Landmark 2 - Quest Destination (Is this the same as the origin?)
  //Landmark 3 - Do we have a town/castle yet? Else random non-town; Ensure we have a ferry station to reach next continent
  //Landmark 4 - Do we have a dungeon yet? Else small outpost/encampment; Ensure we have somewhere to grind for exp/resources
  //Landmark 5 - Do we have an interesting non-combat area?
  //Random number 0-3 for additional dungeons/non-combat locations
}

const char* const Syllables[6][8] = { //Map Location
  {"R", "S", "Qw", "Tr"},
  {"i", "iuo", "e", "o"},
  {"-", "'", "ui", "rr"},
  {"ma", "to", "wa", "po"},
  {"q", "ag", "guer", "qui"},
  {"mir", "fol", "ll", "lii"}
};

void GenerateName()
{
  byte x;
  //WriteLineMessageWindow("@", 0);
  ConsoleBufferReset();
  for (x = 0; x < 6; x++)
  {
    char *syl = (char*)Syllables[x][rand()%4];
    ConsoleBufferAdd(syl);
    ConsoleBufferBackspace();
  }
  WriteLineMessageWindow(strTemp, 1);
  //WriteLineMessageWindow("@", 0);
}

screenName Update_Scenario()
{
  bool exit = false;
  uint16_t scenarioSeed = 0;
  ScreenFadeOut();
  ResizeMessageWindow(consolePosX, consolePosY, consoleWidth, consoleHeight);


  ResetMenu("@", selectionPosX, selectionPosY, selectionWidth, selectionHeight, selectionCount);
  SetMenuItem(0, "Next@");
  SetMenuItem(1, "Last@");
  SetMenuItem(2, "Go@");
  SetMenuItem(3, "End@");
  ScreenFadeIn();


  while (!exit)
  {
    sprintf(strTemp, "Continent: %U @", scenarioSeed);
    SetLineMessageWindow(strTemp, 0);

    //while(1)
    {
     // setSeed(++scenarioSeed);
      //WriteLineMessageWindow(strTemp, 1);

      //GenerateName();
    }

    switch (GetMenuSelection())
    {
      case 0:
        ++scenarioSeed;
        break;
      case 1:
        --scenarioSeed;
        break;
      case 2:
        setSeed(scenarioSeed);
        GenerateContinent();
        ++randseed;
        break;
      case 3:
        exit = true;
        break;
      default:
        break;
    }
  }
  ScreenFadeOut();
  return EditParty;
}
