#include "Xinaga.h"
#include "GameData.h"

//Overworld Tiles
//Water
//Grass
//Forrest
//Mountains
//Swamp


/*
Dungeon Geomorphs
One byte describes 16x16 region
0b76543210
	0 - North Exit
        1 - South Exit
        2 - East Exit
        3 - West Exit
        45 - Treasure
        67 - Encounter/Obstacle
*/

#if defined(__APPLE2__)
//#pragma code-name (push, "LC")
//#pragma rodata-name (push, "LC")
#endif

#if defined (__NES__)
#pragma code-name (push, "MAPGEN")
#pragma rodata-name (push, "MAPGEN")
//#pragma data-name (push, "XRAM")
//#pragma bss-name (push, "XRAM")
#endif

#if defined (__C64__)
#pragma code-name (push, "SCREEN_MAPGEN")
#pragma rodata-name (push, "SCREEN_MAPGEN")
#endif

//const byte OverworldGeoMorphSet[256];
//const byte OverworldGeoMorphAttrib[32];
//const byte DungeonGeoMorphSet[256];
//const byte DungeonGeoMorphAttrib[32];

#if defined(__C64__)
#pragma rodata-name (push, "GAME_DISKDATA")
#endif

bool Display;

//Read-Only Map Info
#define mapCount 14
#define treasurePerMap 5
const byte mapType[mapCount] =
{
  #define TYPE_OVERWORLD 0
  #define TYPE_TOWN 1
  #define TYPE_DUNGEON 2
  #define TYPE_BOSS 3
  TYPE_OVERWORLD,
  TYPE_TOWN, TYPE_TOWN, TYPE_TOWN, TYPE_TOWN,
  TYPE_DUNGEON, TYPE_DUNGEON, TYPE_DUNGEON, TYPE_DUNGEON, TYPE_DUNGEON, TYPE_DUNGEON, TYPE_DUNGEON,
  TYPE_BOSS
};
const byte previousMap[mapCount] =
{
  0, //Overworld
  0, //TownA
  0, //TownB
  0, //TownC
  0, //TownD
  0, //DungeonA1
  5, //DungeonA2
  0, //DungeonB1
  7, //DungeonB2
  0, //DungeonC1
  9, //DungeonC2
  0, //Boss1
  11,//Boss2
  12 //Boss3
};
const byte previousMapDoor[mapCount] =
{
  0, //Overworld
  0, //TownA
  1, //TownB
  2, //TownC
  3, //TownD
  4, //DungeonA1
  1, //DungeonA2,
  5, //DungeonB1
  1, //DungeonB2
  6, //DungeonC1
  1, //DungeonC3
  7, //Boss1
  1, //Boss2
  1  //Boss3
};
const byte nextMap[mapCount] =
{
  0, //Overworld
  0, //TownA
  0, //TownB
  0, //TownC
  0, //TownD
  6, //DungeonA1
  0, //DungeonA2,
  8, //DungeonB1
  0, //DungeonB2
  10, //DungeonC1
  0, //DungeonC3
  12, //Boss1
  13, //Boss2
  0  //Boss3
};

const byte nextMapDoor[mapCount] =
{
  0, //Overworld
  0, //TownA
  1, //TownB
  2, //TownC
  3, //TownD
  0, //DungeonA1
  4, //DungeonA2,
  0, //DungeonB1
  5, //DungeonB2
  0, //DungeonC1
  6, //DungeonC3
  0, //Boss1
  0, //Boss2
  7  //Boss3
};
const byte returnMapDoor[mapCount] =
{
  0, //Overworld
  0, //TownA
  1, //TownB
  2, //TownC
  3, //TownD
  4, //DungeonA1
  1, //DungeonA2,
  5, //DungeonB1
  1, //DungeonB2
  6, //DungeonC1
  1, //DungeonC3
  7, //Boss1
  1, //Boss2
  1  //Boss3
};
const byte overworldDoorDest[8] =
{
  1, 2, 3, 4, //Towns
  5, 7, 9, //Dungeons
  11 //Boss
};
const sbyte monsterLevelOverAverage[mapCount] =
{
  0, //Overworld
  2, //TownA
  2, //TownB
  2, //TownC
  2, //TownD
  1, //DungeonA1
  1, //DungeonA2,
  1, //DungeonB1
  2, //DungeonB2
  2, //DungeonC1
  3, //DungeonC3
  3, //Boss1
  3, //Boss2
  4  //Boss3
};

//Variable Map Info
struct{
  byte seed[mapCount];
  byte variation[mapCount];
  byte monsterType[mapCount];
  byte treasureContents[mapCount][treasurePerMap];
}MapDescriptions;

void FillMapDescriptions(int seed)
{
  byte x, y;
  srand(seed);
  MapDescriptions.seed[0] = seed;
  for (x = 0; x < mapCount; ++x)
    {
      MapDescriptions.seed[x] = seed + (x * rand());
      MapDescriptions.variation[x] = rand();
      MapDescriptions.monsterType[x] = rand();
      for(y = 0; y < treasurePerMap; ++y)
        MapDescriptions.treasureContents[x][y] = rand();
    }
}

/*{w:8, h:8, count:32, bpp:1, brev:1, pal:"c64", np:1}*/
const byte OverworldGeoMorphSet[256] =
{
        0x00, 0x7F, 0x67, 0x5B, 0x7F, 0x73, 0x6D, 0x7F,
	0x00, 0x7F, 0x55, 0x7F, 0x6A, 0x7F, 0x55, 0x7F, 0x00, 0x7F, 0x41, 0x41,
	0x41, 0x41, 0x41, 0x7F, 0x00, 0x20, 0x72, 0x27, 0x72, 0x27, 0x72, 0x27,
	0x00, 0x20, 0x70, 0xDA, 0xEF, 0xFD, 0xFF, 0xFF, 0x00, 0x7F, 0x73, 0x6D,
	0x7F, 0x67, 0x5B, 0x7F, 0x00, 0x00, 0xCC, 0x33, 0xCC, 0x33, 0x00, 0x00,
	0x14, 0x14, 0x28, 0x28, 0x14, 0x14, 0x28, 0x28, 0x18, 0x18, 0x18, 0xFF,
	0x18, 0x18, 0x18, 0x18, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x00,
	0x7E, 0x81, 0xBD, 0xA5, 0xA5, 0xBD, 0x81, 0x7E, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0x22, 0x44, 0xFF, 0x00, 0x00,
	0x00, 0x00, 0xFF, 0x44, 0x22, 0xFF, 0x00, 0x00, 0x24, 0x24, 0x2C, 0x34,
	0x24, 0x24, 0x2C, 0x34, 0x34, 0x2C, 0x24, 0x24, 0x34, 0x2C, 0x24, 0x24,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x66, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x66, 0x00,
	0x00, 0x66, 0xFF, 0xFF, 0xFF, 0xFF, 0x66, 0x00, 0xFC, 0xFE, 0xFE, 0xFC,
	0xFC, 0xFE, 0xFE, 0xFC, 0x00, 0xDC, 0xFE, 0xFE, 0xFE, 0xFC, 0xFE, 0xFE,
	0xFE, 0xFE, 0xFC, 0xFE, 0xFE, 0xFE, 0xEC, 0x00, 0x00, 0x68, 0xFC, 0xFE,
	0xFE, 0xFC, 0x68, 0x00, 0x3F, 0x7F, 0x7F, 0x3F, 0x3F, 0x7F, 0x7F, 0x3F,
	0x00, 0x3B, 0x7F, 0x7F, 0x7F, 0x3F, 0x7F, 0x7F, 0x7F, 0x7F, 0x3F, 0x7F,
	0x7F, 0x7F, 0x3B, 0x00, 0x00, 0x1B, 0x3F, 0x7F, 0x7F, 0x3F, 0x1B, 0x00,
	0x3C, 0x7E, 0x7E, 0x3C, 0x3C, 0x7E, 0x7E, 0x3C, 0x00, 0x18, 0x3C, 0x7E,
	0x3C, 0x7E, 0x7E, 0x3C, 0x3C, 0x7E, 0x7E, 0x3C, 0x7E, 0x7E, 0x18, 0x00,
	0x00, 0x18, 0x3C, 0x7E, 0x7E, 0x3C, 0x18, 0x00
};

/*{w:1, h:1, count:32, bpp:4, pal:"c64", layout:"c64"}*/
const byte OverworldGeoMorphAttrib[32] = {
  0x06, 0x08, 0x0C, 0x0D,
  0x0C, 0x0E, 0x08, 0x08, 0x08, 0x03, 0x0A, 0x0E, 0x08, 0x08, 0x08, 0x08,
  0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
  0x05, 0x05, 0x05, 0x05
  };

/*{w:8, h:8, count:32, bpp:1, brev:1, pal:"c64", np:1}*/
const byte DungeonGeoMorphSet[256] =
{
        0x18, 0x18, 0x3C, 0xBD, 0xBD, 0x99, 0x99, 0xFF,
	0x0F, 0x09, 0x3D, 0x3F, 0xFF, 0x3D, 0x09, 0x0F, 0xFF, 0x85, 0xB5, 0xB5,
	0xAD, 0xAD, 0xA1, 0xFF, 0xFF, 0x91, 0xB9, 0x81, 0xFD, 0x9D, 0x81, 0xFF,
	0xFC, 0x84, 0xB6, 0xF7, 0xB1, 0x8D, 0x8D, 0xFF, 0x10, 0x7C, 0x54, 0x10,
	0xFF, 0x10, 0x1E, 0x10, 0xFF, 0xA9, 0x8D, 0xFD, 0x8B, 0xAB, 0xA9, 0xFF,
	0xEF, 0xEF, 0xFF, 0xEF, 0x04, 0xEF, 0xFF, 0xEF, 0x18, 0x7E, 0x5A, 0xFF,
	0xFF, 0x5A, 0x7E, 0x18, 0x18, 0x7E, 0x72, 0xDB, 0xDB, 0x4E, 0x7E, 0x18,
	0x7E, 0xB1, 0xEF, 0xB5, 0xAD, 0xB5, 0x6E, 0x7E, 0x10, 0x57, 0x7C, 0x30,
	0xDC, 0x44, 0x7E, 0x04, 0x18, 0x18, 0xBD, 0xE7, 0xE7, 0xBD, 0x18, 0x18,
	0xFF, 0xDB, 0xBD, 0xFF, 0xFF, 0xBD, 0xDB, 0xFF, 0xFF, 0xAA, 0xFF, 0xAA,
	0xFF, 0xAA, 0xFF, 0xAA, 0x00, 0xFE, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
	0x18, 0x18, 0x18, 0xFF, 0xFF, 0x18, 0x18, 0x18, 0x00, 0x00, 0x00, 0xFF,
	0xFF, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0xFF, 0xFF, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x18, 0x18, 0x18, 0xF8,
	0xF8, 0x18, 0x18, 0x18, 0x00, 0x00, 0x00, 0xF8, 0xF8, 0x18, 0x18, 0x18,
	0x18, 0x18, 0x18, 0xF8, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0,
	0xF0, 0x00, 0x00, 0x00, 0x18, 0x18, 0x18, 0x1F, 0x1F, 0x18, 0x18, 0x18,
	0x00, 0x00, 0x00, 0x1F, 0x1F, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x1F,
	0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x1F, 0x00, 0x00, 0x00,
	0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 0x00, 0x00, 0x18,
	0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00
};
/*{w:1, h:1, count:32, bpp:4, pal:"c64", layout:"c64"}*/
const byte DungeonGeoMorphAttrib[32] = {
  0x0F, 0x0F, 0x0F, 0x0F,
  0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0B,
  0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C,
  0x0C, 0x0C, 0x0C, 0x0C
  };

/* World Seed Parameters
(Eight flags building a byte)
Light /Shadow	//128
North / South	//64
East / West	//32
Before / After	//16
		//8
                //4
                //2
                //1

Eight more flags after all 256 boards clear
Spring/Summer	//128
Fall/Winter	//64
East / West	//32
Before / After	//16
		//8
                //4
                //2
                //1
Traversal challenge locations
	Overland (no challenge)
        Cave
        Dense Woods
        Ruins (they link to each other)
        Mountains
        Ocean(build boat; place shipwright X locations back)

What about a sky world and an underworld? One mega-ruin with lots of exits?
*/

struct vector2 *points;
void createPoint(byte index, byte x, byte y);
void clampPoint(struct vector2 *clmpt);
void deletePoint(int pos);

#define pointsBase 64
#define continentsBase 6

static byte countContinents;
static byte totalPoints;
static byte forrestCount;

static byte scenarioPoints[9];
static byte scenarioDir[9];
static byte scenarioDist[9];
static const sbyte distX[4] = {0, 0, 1, -1};
static const sbyte distY[4] = {-1, 1, 0, 0};

const static char dirChar[5] = {"NSEW"};
const static byte dist[5] = {4, 3, 3, 5, 2};
//const static byte dist[5] = {6, 3, 4, 3, 7};

//Scenario Tiles
#define scenTown 49
#define scenCastle 47
#define scenDungeon 45
#define scenCave 40
#define scenRuins 48
#define scenWater 34
#define scenGrass 36
#define scenForest 44

//Minimap Glyphs
#define miniMapWater 0
//#define miniMapRoads 1
#define miniMapRoadsH 6
#define miniMapRoadsV 7
#define miniMapRoadsI 8

#define miniMapGrass 16
#define miniMapForest 3
#define miniMapMountain 4
#define miniMapWaterTravel 5

#define ScenarioDescX 19
#define ScenarioDescY 1

byte lastPointX;
byte lastPointY;

//Town
//Dungeon

const byte traversal[8] = {scenGrass, scenForest, scenGrass, scenForest, scenGrass, scenGrass, scenGrass, scenWater};
//A location has the same land type as the previous traversal
const byte locations[8] = {scenDungeon, scenCave, scenRuins, scenRuins, scenDungeon, scenRuins, scenDungeon, scenDungeon};

byte GetFringeMask(byte x, byte y, char fringe)
{
  struct vector2 pointAdj;
  byte mask = 0;
  byte z = 0;
  byte adjX[4];
  byte adjY[4];
  adjX[0] = x;
  adjY[0] = y - 1;

  adjX[1] = x;
  adjY[1] = y + 1;

  adjX[2] = x + 1;
  adjY[2] = y;

  adjX[3] = x - 1;
  adjY[3] = y;

  for (;z < 4; ++z)
  {
    pointAdj.x = adjX[z];
    pointAdj.y = adjY[z];
    clampPoint(&pointAdj);
    adjX[z] = pointAdj.x;
    adjY[z] = pointAdj.y;
    if (mapQuads[adjX[z] + (mapMatrixWidth * adjY[z])] == fringe)
      mask += (1 << z);
  }
  return mask;
}

bool CheckOverlap(byte x, byte y)
{
  struct vector2 *p = points;
  int i;
  for (i = 0; i < totalPoints; ++i)
  {
    if (p->x == x)
      if (p->y == y)
      {
        return true;
      }
    p = p->next;
  }
  return false;
}

void DrawScenario()
{
  byte x;
  byte distTravel;
  //byte failure = 0;
  #define originX 8
  #define originY 8
  struct vector2 scenPos = {originX, originY};
  struct vector2 originPos = {originX, originY};
  char scenChar;
  char pathIndex;

  //DrawBorder("Scenario Path", originX, originY , PartyStatsWidth, PartyStatsHeight, false);

  scenPos.x = originX;
  scenPos.y = originY;
  //createPoint(road, scenPos.x, scenPos.y);

  for (x = 0; x < 9; ++x)
  {
    scenPos.x = originPos.x;
    scenPos.y = originPos.y;
    pathIndex = miniMapRoadsV;
    scenChar = '0' + x;
    //scenChar = miniMapGrass;

    if (x % 3 == 0)
      scenarioPoints[x] =locations[rand() %8];
    else
      scenarioPoints[x] =traversal[rand() %8];
    scenarioPoints[0] = scenTown;
    scenarioPoints[8] = scenCastle;

    scenarioDir[x] = rand() %4;
    if (x > 0)
      while (scenarioDir[x] == scenarioDir[x-1])
        scenarioDir[x] = rand() %4;

    if (scenarioDir[x] > 1)
      pathIndex = miniMapRoadsH;

    scenarioDist[x] = rand() %5;
    distTravel = dist[scenarioDist[x]];
    if (scenarioPoints[x] == scenWater)
    {
      if (x > 2)
        distTravel += 5;
      else
        scenarioPoints[x] = scenForest;
    }
    if(Display)
    {
      SetChar('0'+x, ScenarioDescX  + 2*x, ScenarioDescY);
      DrawTileDirectXY(scenarioPoints[x], ScenarioDescX + 2*x,  ScenarioDescY);
      SetChar(dirChar[scenarioDir[x]], ScenarioDescX +1 + 2*x, ScenarioDescY + 2);
    }
    scenPos.x += (distX[scenarioDir[x]] * distTravel);
    scenPos.y += (distY[scenarioDir[x]] * distTravel);
    clampPoint(&scenPos);

    //Check points for overlap and adjust until they're separated
    while(CheckOverlap(scenPos.x, scenPos.y))
    {
      scenPos.x += (distX[scenarioDir[x]]);
      scenPos.y += (distY[scenarioDir[x]]);
      clampPoint(&scenPos);
      ++distTravel;
    }
    if(Display)
      SetChar('0' + distTravel, ScenarioDescX  + 2*x, ScenarioDescY + 2);

    //if (x == 0)
    {
      //SetPlayerPositionX = scenPos.x;
      //SetPlayerPositionY = scenPos.y;
    }

    while ((originPos.x != scenPos.x) || (originPos.y != scenPos.y))
      //for (i = 0; i < scenarioDist[x]; ++i)
    {
      originPos.x += (distX[scenarioDir[x]]);
      originPos.y += (distY[scenarioDir[x]]);
      clampPoint(&originPos);

      if (x > 0)
      {
        byte indexRoad = mapQuads[originPos.x + (mapMatrixWidth* originPos.y)];
        //Don't place a road if a scenario location exists
        if ((indexRoad >= '0') && (indexRoad <= '9'))
          continue;
        //Draw line from last point to this one using the terrain type
        //Unless it's water, in which case we don't want to draw water over an existing critical path
        if ((scenarioPoints[x] == scenWater) && (!CheckOverlap(scenPos.x, scenPos.y)))
        {
          pathIndex = miniMapWaterTravel;
          createPoint(pathIndex, originPos.x, originPos.y);
          deletePoint(totalPoints - 1);
          continue;
        }
        //Skip any existing points along the way
        //if(!CheckOverlap(originPos.x, originPos.y))
        {
          if (pathIndex == miniMapRoadsH)
            if (indexRoad == miniMapRoadsV)
              pathIndex = miniMapRoadsI;
          if (pathIndex == miniMapRoadsV)
            if (indexRoad == miniMapRoadsH)
              pathIndex = miniMapRoadsI;
          createPoint(pathIndex, originPos.x, originPos.y);
        }
      }
    }
    originPos.x = scenPos.x;
    originPos.y = scenPos.y;

    createPoint(scenChar, scenPos.x, scenPos.y);
    if(x < doorCount)
    {
      Doors.doorActive[x] = true;
      Doors.posX[x] = scenPos.x * 16 + 8;
      Doors.posY[x] = scenPos.y * 16 + 8;
      Doors.destMap[x] = overworldDoorDest[x];
      Doors.destDoor[x] = 0;
      //sprintf(strTemp, "Door %d at %d,%d", x, Doors.posX[x], Doors.posY[x]);
      //WriteLineMessageWindow(strTemp, 0);
    }
    if(Display)
      SetChar('0' + x, scenPos.x + viewportPosX, scenPos.y + viewportPosY);
  }
}

void TranslateQuadIndices()
{
  byte x, y;
  //Translate to quad indexes...
  //TO-DO: Change the minimap to draw with an offset to translate quad references into minimap glyphs
  //Need a place for 64 glyphs to correspond to 64 tile geomorphs, probably 0xE0
  //Minimap glyph will be (mapQuads[x + (mapMatrixWidth * y)] + MiniMapGlypBase)
  //8 types of terrain with 8 variations each = 64
  for (y = 0; y < 16; ++y)
    for (x = 0; x < 16; ++x)
    {
      byte offset = x + (mapMatrixWidth * y);
      byte index  = mapQuads[offset];
      if ((index >= '0') && (index <= '9'))
      {
        mapQuads[offset] = 62;//-= '0';
        continue;
      }

      if (index == miniMapWater)
      {
        mapQuads[offset] = 56;
        continue;
      }
      if (index == miniMapRoadsH)
      {
        mapQuads[offset] = 32;
        continue;
      }
      if (index == miniMapRoadsV)
      {
        mapQuads[offset] = 33;
        continue;
      }
      if (index == miniMapRoadsI)
      {
        mapQuads[offset] = 34;
        continue;
      }
      if (index == miniMapGrass)
      {
        mapQuads[offset] = 0;
        continue;
      }
      if (index == miniMapForest)
      {
        mapQuads[offset] = 59;
        continue;
      }
      if (index == miniMapMountain)
      {
        mapQuads[offset] = 60;
        continue;
      }
      if (index == miniMapWaterTravel)
      {
        mapQuads[offset] = 56;
        //mapQuads[offset] = 61;
        continue;
      }
    }

  for (y = 0; y < 16; ++y)
    for (x = 0; x < 16; ++x)
    {
      byte offset = x + (mapMatrixWidth * y);
      byte index  = mapQuads[offset];
      if ( index == 0)
        mapQuads[offset] += GetFringeMask(x, y, 56);
    }

}

void DrawPoint(byte x, byte y)
{
  byte tile = mapQuads[x + (mapMatrixWidth * y)];

  //Don't add offset if tile is a number
  //if (tile < 16)
  tile += MiniMapOffset;

  #if defined(__APPLE2__)
  if (x % 2)
  {
    SetChar(tile + 1, x + viewportPosX, y + viewportPosY);
    return;
  }
  SetChar(tile, x + MiniMapPosX, y + MiniMapPosY);
  return;
  #endif
  SetChar(tile, x + MiniMapPosX, y + MiniMapPosY);
}

void createPoint(byte index, byte x, byte y)
{
  struct vector2 *temp,*ptr;
  temp=(struct vector2 *)malloc(sizeof(struct vector2));

  if(temp==NULL)
    return;

  temp->x = x;
  temp->y = y;

  temp->next=NULL;
  if(points==NULL)
    points=temp;
  else
  {
    ptr=points;
    while(ptr->next!=NULL)
    {
      ptr=ptr->next;
    }
    ptr->next=temp;
  }
  ++totalPoints;

  mapQuads[x + (mapMatrixWidth * y)] = index;
  if(Display)
    DrawPoint(x,y);
  
  lastPointX = x;
  lastPointY = y;
}

struct vector2 *getPoint(byte index)
{
  byte i = 0;
  struct vector2 *tmp = points;
  while (tmp != NULL)
  {
    if(i == index)
    {
      return tmp;
    }
    tmp = tmp->next;
    ++i;
  }
}

void deletePoint(int pos)
{
  byte i;
  struct vector2 *temp,*ptr;
  temp = NULL;

  if(points==NULL)
    return;
  else
  {
    if(pos==0)
    {
      ptr=points;
      points=points->next ;
    }
    else
    {
      ptr=points;
      for(i=0;i<pos;i++)
      {
        temp=ptr;
        ptr=ptr->next;
        if(ptr==NULL)
        {
          WriteLineMessageWindow("Position not Found:", 0);
          return;
        }
      }
      temp->next =ptr->next ;
    }
    free(ptr);
    --totalPoints;
  }
}

void clearPoints()
{
  while (points != NULL)
    deletePoint(0);
  totalPoints = 0;
}

void clampPoint(struct vector2 *clmpt)
{
  if (clmpt->x < 0)
    clmpt->x = mapMatrixWidth - 1;
  if (clmpt->y < 0)
    clmpt->y = mapMatrixHeight - 1;

  if (clmpt->x >= mapMatrixWidth)
    clmpt->x = 0;
  if (clmpt->y >= mapMatrixHeight)
    clmpt->y = 0;
}

byte countAdjacent(byte x, byte y)
{
  struct vector2 pointAdj;
  byte i = 0;
  byte z = 0;
  byte adjX[4];
  byte adjY[4];
  adjX[0] = x;
  adjX[1] = x;
  adjX[2] = x - 1;
  adjX[3] = x + 1;
  adjY[0] = y - 1;
  adjY[1] = y + 1;
  adjY[2] = y;
  adjY[3] = y;

  for (;z < 4; ++z)
  {
    pointAdj.x = adjX[z];
    pointAdj.y = adjY[z];
    clampPoint(&pointAdj);
    adjX[z] = pointAdj.x;
    adjY[z] = pointAdj.y;
    if ((mapQuads[adjX[z] + (mapMatrixWidth * adjY[z])] != miniMapWater) && (mapQuads[adjX[z] + (mapMatrixWidth * adjY[z])] != miniMapWaterTravel))
      ++i;
  }
  return i;
}

void checkLandlocked()
{
  byte i, x, y, index;
  for (i = 0; i < totalPoints; ++i)
  {
    struct vector2 *tmpt = getPoint(i);
    x = tmpt->x;
    y = tmpt->y;

    if (countAdjacent(x, y) >= 4)
    {
      if (forrestCount < 5)
      {
        index = miniMapForest;
        ++forrestCount;
      }
      else
      {
        index  = miniMapMountain;
        forrestCount = 0;
      }
      deletePoint(i);
      if(mapQuads[x + (mapMatrixWidth * y)] == miniMapGrass)
      {
        mapQuads[x + (mapMatrixWidth * y)] = index;
        if(Display)
          DrawPoint(x,y);
      }
    }
  }
}

void attachRandomPoint(byte index, byte antiIndex)
{
  sbyte x, y;
  byte i;
  bool exit = false;
  byte failure = 0;

  while (1)
  {
    byte dir = rand() % 4;
    if (totalPoints >= 1)
      i = rand() % (totalPoints);
    else
      i = 0;

    x = getPoint(i)->x;
    y = getPoint(i)->y;

    switch (dir)
    {
      case 0:
        --y;
        if (y < 0)
          y = mapMatrixHeight - 1;
        break;
      case 1:
        ++y;
        if (y >= mapMatrixHeight)
          y = 0;
        break;
      case 2:
        --x;
        if (x < 0)
          x = mapMatrixWidth - 1;
        break;
      case 3:
        ++x;
        if (x >= mapMatrixWidth)
          x = 0;
        break;
      default:
        break;
    }

    if ((mapQuads[x + (mapMatrixWidth * y)] == antiIndex))
      exit = true;

    if (exit)
      break;

    ++failure;
    if (failure == 48)
    {
      sprintf(strTemp, "Removing point (%d), continent %d can't add", i, countContinents);
      WriteLineMessageWindow(strTemp, 0);
      deletePoint(i);
      if (totalPoints >= 1)
        i = rand() % (totalPoints);
      else
        i = 0;
      x = getPoint(i)->x;
      y = getPoint(i)->y;
      failure = 0;
    }
    if (points == NULL)
    {
      sprintf(strTemp, "Moving on");
      WriteLineMessageWindow(strTemp, 0);
      return;
    }
  }
  createPoint(index, x, y);
}

void GenerateOverworld(byte seed)
{
  byte x, y;
  byte totalPointsPlaced = 0;
  UploadCharPage((byte*)OverworldGeoMorphSet, 7);
  clearPoints();
  countContinents = 0;
  forrestCount = 0;
  memset (&mapQuads[0], miniMapWater, mapMatrixHeight*mapMatrixWidth);
  if(Display)
  {
    DrawMiniMap(false);
    for (y = 0; y < mapMatrixHeight; ++y)
      for (x = 0; x < mapMatrixWidth; ++x)
        DrawPoint(x, y);
  }
  srand(seed);
  sprintf(strTemp, "Overworld Seed:(%3d)", seed);
  SetLineMessageWindow(strTemp, 0);
  DrawScenario();
  checkLandlocked();
  //return;

  for ( y = 0; y < continentsBase; ++y)
  {
    //Create Continent
    {
      sbyte landcount = pointsBase - (y*6);
      //addRandomPoints(1, grass);
      while (landcount && (points != NULL))
      {
        attachRandomPoint(miniMapGrass, miniMapWater);
        --landcount;
        //++totalPointsPlaced;
      }

      checkLandlocked();

      ++countContinents;
    }
    totalPointsPlaced += totalPoints;
    clearPoints();
  }
  TranslateQuadIndices();
  if(Display)
    DrawMiniMap(false);
  sprintf(strTemp, "Points Placed:(%3d)", totalPointsPlaced);
  WriteLineMessageWindow(strTemp, 0);
}

void ClearDoors()
{
  byte x;
  for (x = 0; x < doorCount; ++x)
    {
      Doors.doorActive[x] = false;
      Doors.posX[x] = 0;
      Doors.posY[x] = 0;
      Doors.destMap[x] = 0;
      Doors.destDoor[x] = 0;
    }
}

void PlaceEntrance(byte x, byte y)
{
  byte posX = x * 16 + 8;
  byte posY = y * 16 + 8;
  
  Doors.doorActive[0] = true;
  Doors.posX[0] = posX;
  Doors.posY[0] = posY;
  Doors.destMap[0] = previousMap[MapIndex];
  Doors.destDoor[0] = previousMapDoor[MapIndex];
  
  sprintf(strTemp, "Entrance %d %d", posX, posY);
  WriteLineMessageWindow(strTemp, 0);
  SkipLineMessageWindow();  
}

void PlaceExit(byte x, byte y)
{
  byte posX = x * 16 + 8;
  byte posY = y * 16 + 8;
  Doors.doorActive[1] = true;
  Doors.posX[1] = x * 16 + 8;
  Doors.posY[1] = y * 16 + 8;
  Doors.destMap[1] = nextMap[MapIndex];
  Doors.destDoor[1] = nextMapDoor[MapIndex];

  sprintf(strTemp, "Exit %d %d", posX, posY);
  WriteLineMessageWindow(strTemp, 0);
  SkipLineMessageWindow();  
}

void GenerateTown(byte seed)
{
  srand(seed);
  ClearDoors();
  PlaceEntrance(8, 8);
}

#define miniMapDungeonFloor 14
#define miniMapDungeonWall 15

#define ROOM_SMALL 0
#define ROOM_DOUBLE 2
#define ROOM_TRIPLE 4
#define ROOM_QUAD 6
#define ROOM_LARGE 8
#define ROOM_WINDING 10
#define ROOM_SPECIAL 12
#define ROOM_CELLS 14
#define ROOM_HALLWAY 16

const byte RoomProbability[20] =
{
  ROOM_HALLWAY,
  ROOM_HALLWAY,
  ROOM_LARGE,
  ROOM_CELLS,
  ROOM_HALLWAY,
  ROOM_SMALL,
  ROOM_HALLWAY,
  ROOM_HALLWAY,
  ROOM_SMALL,
  ROOM_SMALL,
  ROOM_LARGE,
  ROOM_HALLWAY,
  ROOM_LARGE,
  ROOM_DOUBLE,
  ROOM_TRIPLE,
  ROOM_QUAD,
  ROOM_SPECIAL,
  ROOM_WINDING,
  ROOM_HALLWAY,
  ROOM_HALLWAY
};

void PlaceRoom()
{
        #define sizeXMin 5
        #define sizeXMax 7
        #define sizeYMin 5
        #define sizeYMax 7
        byte x, y;
        byte sizeX = sizeXMin + (rand() % (sizeXMax - sizeXMin));
        byte sizeY = sizeYMin + (rand() % (sizeYMax - sizeYMin));

        for (y = 0; y < sizeY; ++y)
                {
                        for (x = 0; x < sizeX; ++x)
                        {
                          byte roomNum = RoomProbability[rand() % 20];
                          byte randomadd = rand() % 20;
                          if (roomNum < ROOM_HALLWAY)
                            if(randomadd > 10)
                              ++roomNum;
                          attachRandomPoint(roomNum, miniMapDungeonWall);
                          attachRandomPoint(ROOM_HALLWAY, miniMapDungeonWall);
                        }
                }
}

void GenerateDungeon(byte seed)
{
  byte x,y;
  ClearDoors();
  UploadCharPage((byte*)DungeonGeoMorphSet, 7);
  clearPoints();
  sprintf(strTemp, "Dungeon Seed (%d)", seed);
  SetLineMessageWindow(strTemp, 0);

  for (y = 0; y < mapMatrixWidth; ++y)
    for(x = 0; x < mapMatrixHeight; ++x)
      {
        mapQuads[x + (mapMatrixWidth * y)] = miniMapDungeonWall;
        if(Display)
          DrawPoint(x,y);
      }
  srand(seed);
  createPoint(RoomProbability[rand() % 20], 8, 8);
  PlaceEntrance(lastPointX, lastPointY);
  
  //for (x = 0; x < RoomCount; ++x)
          PlaceRoom();

  for (y = 0; y < 16; ++y)
    for (x = 0; x < 16; ++x)
    {
      byte offset = x + (mapMatrixWidth * y);
      byte index  = mapQuads[offset];
      if ( index == ROOM_HALLWAY)
      {
        mapQuads[offset] += GetFringeMask(x, y, miniMapDungeonWall);
      }
      if(Display)
        DrawPoint(x,y);
    }
  PlaceExit(lastPointX, lastPointY);
}

byte seed;

void GetSeed()
{
  //#define menuWidth 5
  //#define menuCount 4
  ResetMenu("Seed", 6, true);
  SetMenuItem(0, "Next");
  SetMenuItem(1, "Last");
  SetMenuItem(2, "Overworld");
  SetMenuItem(3, "Town");
  SetMenuItem(4, "Dungeon");
  SetMenuItem(5, "End");

  //sprintf(strTemp, "Seed (%d)", seed);
  //SetLineMessageWindow(strTemp, 0);
  while(1)
  {
    GenerateOverworld(seed);
    //GenerateDungeon(seed);
    //WaitForInput();
    //WriteLineMessageWindow("", 0);
    //++seed;
    return;
  }

  while (1)
  {
    sprintf(strTemp, "Seed (%d)", seed);
    SetLineMessageWindow(strTemp, 0);
    switch (GetMenuSelection())
    {
      case 0:
        ++seed;
        break;
      case 1:
        --seed;
        break;
      case 2:
        GenerateOverworld(seed);
        break;
      case 3:
        GenerateTown(seed);
        break;
      case 4:
        GenerateDungeon(seed);
        break;
      case 5:
        //exit = true;
        return;
    }
  }
}

const char *mapTypeName[] = 
{
  "OVERWORLD", "TOWN", "DUNGEON", "FINAL"
};

void GenerateMap(byte index)
{
  seed = MapDescriptions.seed[index];
  sprintf(strTemp, "Generating %s from seed %d", mapTypeName[mapType[index]], seed);
  WriteLineMessageWindow(strTemp, 0);
  SkipLineMessageWindow();
  Display = false;
  
  switch (mapType[index])
    {
      case 0:
        GenerateOverworld(seed);
        break;
      case 1:
        GenerateTown(seed);
        break;
      case 2:
        GenerateDungeon(seed);
        break;
      case 3:
        //GenerateBoss()
        break;
    }
}

screenName Update_MapGen()
{
  seed = 20;
  FillMapDescriptions(seed);
  Display = true;
  Entering = true;
  EnteringDoor = 0;
  //ClearScreen();
        MiniMapPosX = viewportPosX;
        MiniMapPosY = viewportPosY;
  MiniMapWidth = 16;
  MiniMapHeight = 16;
  //ResizeMessageWindow();
  ScreenFadeIn();
  //ClearInterface();
  FillViewport(' ', viewportWidth, viewportHeight);
  GetSeed();
  //StoreMap();
  //ScreenFadeOut();
  //ClearScreen();
  //ClearInterface();
  return Map;
}
