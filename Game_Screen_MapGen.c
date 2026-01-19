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
#pragma code-name (push, "LC")
#endif

#if defined (__NES__)
#pragma code-name (push, "MAPGEN")
#pragma rodata-name (push, "MAPGEN")
#endif

#if defined (__C64__)
#pragma code-name (push, "SCREEN_MAPGEN")
#pragma rodata-name (push, "SCREEN_MAPGEN")
#endif

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

typedef struct vector2
{
  sbyte x;
  sbyte y;
  struct vector2 *next;
};

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
static sbyte distX[4] = {0, 0, 1, -1};
static sbyte distY[4] = {-1, 1, 0, 0};

const static char dirChar[4] = {"NSEW"};
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
#define scenForrest 44

//Minimap Glyphs
#define miniMapWater 0xF0
#define road 0xF1
#define grass 0xF2
#define forrest 0xF3
#define mountain 0xF4
#define waterTravel 0xF5
//Town
//Dungeon

const byte traversal[8] = {scenGrass, scenForrest, scenGrass, scenForrest, scenGrass, scenGrass, scenGrass, scenWater};
//A location has the same land type as the previous traversal
const byte locations[8] = {scenDungeon, scenCave, scenRuins, scenRuins, scenDungeon, scenRuins, scenDungeon, scenDungeon};

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

  DrawBorder("Scenario Path@", viewportPosX - 1, viewportPosY- 1 +mapMatrixHeight + 2 , 20, 6, false);

  scenPos.x = originX;
  scenPos.y = originY;
  createPoint('S', scenPos.x, scenPos.y);

  for (x = 0; x < 9; ++x)
  {
    scenPos.x = originPos.x;
    scenPos.y = originPos.y;
    pathIndex = road;
    scenChar = '0' + x;

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

    scenarioDist[x] = rand() %5;
    distTravel = dist[scenarioDist[x]];
    if (scenarioPoints[x] == scenWater)
      distTravel += 5;
    SetChar('0'+x, viewportPosX  + 2*x, viewportPosY + mapMatrixHeight + 2);

    DrawTileDirectXY(scenarioPoints[x], viewportPosX  + 2*x,  viewportPosY + mapMatrixHeight + 3);
    SetChar(dirChar[scenarioDir[x]], viewportPosX  + 2*x, viewportPosY + mapMatrixHeight + 2 + 3);

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
    SetChar('0' + distTravel, viewportPosX  + 2*x + 1, viewportPosY + mapMatrixHeight + 2 + 3);

    while ((originPos.x != scenPos.x) || (originPos.y != scenPos.y))
      //for (i = 0; i < scenarioDist[x]; ++i)
    {
      originPos.x += (distX[scenarioDir[x]]);
      originPos.y += (distY[scenarioDir[x]]);
      clampPoint(&originPos);

      //Draw line from last point to this one using the terrain type
      //Unless it's water, in which case we don't want to draw water over an existing critical path
      if (scenarioPoints[x] == scenWater)
      {
        pathIndex = waterTravel;
        createPoint(pathIndex, originPos.x, originPos.y);
        deletePoint(totalPoints - 1);
        continue;
      }

      //Skip any existing points along the way
      if(!CheckOverlap(originPos.x, originPos.y))
      {
        createPoint(pathIndex, originPos.x, originPos.y);
      }
    }
    originPos.x = scenPos.x;
    originPos.y = scenPos.y;

    createPoint(scenChar, scenPos.x, scenPos.y);
  }
}

void DrawPoint(byte x, byte y)
{
  byte tile = mapQuads[x + (mapMatrixWidth * y)];
  #if defined(__APPLE2__)
  if (x % 2)
  {
    SetChar(tile + 1, x + viewportPosX, y + viewportPosY);
    return;
  }
  SetChar(tile, x + viewportPosX, y + viewportPosY);
  return;
  #endif
  SetChar(tile, x + viewportPosX, y + viewportPosY);
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
  DrawPoint(x,y);
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
          WriteLineMessageWindow("Position not Found:@", 0);
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
    if (mapQuads[adjX[z] + (mapMatrixWidth * adjY[z])] != miniMapWater)
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
        index = forrest;
        ++forrestCount;
      }
      else
      {
        index  = mountain;
        forrestCount = 0;
      }
      deletePoint(i);
      if(mapQuads[x + (mapMatrixWidth * y)] == grass)
      {
        mapQuads[x + (mapMatrixWidth * y)] = index;
        DrawPoint(x,y);
      }
    }
  }
}

void addRandomPoints(byte count, int index)
{
  byte x;
  for (x = 0; x < count; ++x)
  {
    byte h = rand() % mapMatrixHeight;
    byte w = rand() % mapMatrixWidth;

    while (mapQuads[w + (mapMatrixWidth * h)] != miniMapWater)
    {
      h = rand() % mapMatrixHeight;
      w = rand() % mapMatrixWidth;
    }
    createPoint(index, w, h);
  }
}

void attachRandomPoint(byte index)
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
    
    if ((mapQuads[x + (mapMatrixWidth * y)] == miniMapWater))
      exit = true;

    if (exit)
      break;

    ++failure;
    if (failure == 48)
    {
      sprintf(strTemp, "Removing point (%d), continent %d can't add@", i, countContinents);
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
      sprintf(strTemp, "Moving on@");
      WriteLineMessageWindow(strTemp, 0);
      return;
    }
  }
  createPoint(index, x, y);

}

void GenerateMap(byte seed)
{
  byte x, y;
  byte totalPointsPlaced = 0;
  clearPoints();
  countContinents = 0;
  forrestCount = 0;
  memset (&mapQuads[0], miniMapWater, mapMatrixHeight*mapMatrixWidth);
  DrawMiniMap(false);
  for (y = 0; y < mapMatrixHeight; ++y)
    for (x = 0; x < mapMatrixWidth; ++x)
      DrawPoint(x, y);


  srand(seed);
  sprintf(strTemp, "Generating Seed:(%3d)@", seed);
  DrawScenario();
  checkLandlocked();
  WriteLineMessageWindow(strTemp, 0);
  //return;

  for ( y = 0; y < continentsBase; ++y)
  {
    //Create Continent
    {
      sbyte landcount = pointsBase - (y*6);  
      //addRandomPoints(1, grass);
      while (landcount && (points != NULL))
      {
        attachRandomPoint(grass);
        --landcount;
        //++totalPointsPlaced;
      }

      checkLandlocked();

      ++countContinents;
    }
    totalPointsPlaced += totalPoints;
    clearPoints();
  }
  
  //Translate to quad indexes...
  //TO-DO: Change the minimap to draw with an offset to translate quad references into minimap glyphs
  //Need a place for 64 glyphs to correspond to 64 tile geomorphs, probably 0xE0
  //Minimap glyph will be (mapQuads[x + (mapMatrixWidth * y)] + MiniMapGlypBase)
  //8 types of terrain with 8 variations each = 64
  for (y = 0; y < 16; ++y)
    for (x = 0; x < 16; ++x)
    {
      if (mapQuads[x + (mapMatrixWidth * y)] == miniMapWater)
        mapQuads[x + (mapMatrixWidth * y)] = 63;
      
      if (mapQuads[x + (mapMatrixWidth * y)] == grass)
        mapQuads[x + (mapMatrixWidth * y)] = 62;
    }
  
  sprintf(strTemp, "Points Placed:  (%3d)@", totalPointsPlaced);
  WriteLineMessageWindow(strTemp, 0);
}

byte seed  = 20;
void GetSeed()
{
  #define menuWidth 5
  #define menuCount 4
  ResetMenu("Seed@", COLS - menuWidth - 3, consolePosY, menuWidth, consoleHeight, menuCount);
  SetMenuItem(0, "Next@");
  SetMenuItem(1, "Last@");
  SetMenuItem(2, "Go@");
  SetMenuItem(3, "End@");

  //sprintf(strTemp, "Seed (%d)@", seed);
  //SetLineMessageWindow(strTemp, 0);
  //while(1)
  {

    //GenerateMap(seed);
    //++seed;
  }

  while (1)
  {
    //sprintf(strTemp, "Seed (%d)@", seed);
    //SetLineMessageWindow(strTemp, 0);
    switch (GetMenuSelection())
    {
      case 0:
        ++seed;
        break;
      case 1:
        --seed;
        break;
      case 2:
        GenerateMap(seed);
        //GenerateDungeon(seed);
        break;
      case 3:
        //exit = true;
        return;
    }
  }
}

screenName Update_MapGen()
{
  ClearScreen();
  MiniMapPosX = 1;
  MiniMapPosY = 1;
  ResizeMessageWindow(COLS - 22, viewportPosY, 21, 16);
  ScreenFadeIn();
  GetSeed();
  //StoreMap();
  ScreenFadeOut();
  ClearScreen();
  return Map;
}

/*void FillAdjacent(byte passes, byte threshold)
{
  byte x, y, i;
  for (i = 0; i < passes; ++i)
  {
    for (y = 0; y < height; ++y)
      for (x = 0; x < width; ++x)
      {
        if (map[y][x] != grass)
          if (countAdjacent(x, y) >= threshold)
          {
            createPoint(x, y);
            map[y][x] = grass;
            SetChar(grass, posX + x, posY + y);
            //sprintf(strTemp, "Filled (%d, %d)@", x, y);
            //WriteLineMessageWindow(strTemp, 0);
          }
      }
    sprintf(strTemp, "Pass %d done@", i + 1);
    WriteLineMessageWindow(strTemp, 0);
  }
}*/

/*void RemoveIslands()
{
  byte x, y;
  for (y = 0; y < height; ++y)
    for (x = 0; x < width; ++x)
    {
      if (map[y][x] == grass)
        if (countAdjacent(x, y) == 0)
        {
          map[y][x] = water;
          SetChar(water, posX + x, posY + y);
          sprintf(strTemp, "Removed (%d, %d)@", x, y);
          WriteLineMessageWindow(strTemp, 0);
        }
    }
  sprintf(strTemp, "Islands Removed@");
  WriteLineMessageWindow(strTemp, 0);
}*/

/*void Rotate(direction dir)
{
  byte h;
  byte w;
  byte tempRow[mapMatrixWidth];
  byte tempCol[mapMatrixHeight];
  switch (dir)
  {
    case up:
      for (w = 0; w < mapMatrixWidth; ++w)
        tempRow[w] = mapQuads[mapMatrixHeight - 1][w];
      for (h = mapMatrixHeight - 1; h > 0; --h)  
        for (w = 0; w < mapMatrixWidth; ++w)
          mapQuads[h][w] = mapQuads[h - 1][w];
      for (w = 0; w < mapMatrixWidth; ++w)
        mapQuads[0][w] = tempRow[w];
      break;
    case down:
      for (w = 0; w < mapMatrixWidth; ++w)
        tempRow[w] = mapQuads[0][w];
      for (h = 0; h < mapMatrixHeight; ++h)  
        for (w = 0; w < mapMatrixWidth; ++w)
          mapQuads[h][w] = mapQuads[h + 1][w];
      for (w = 0; w < mapMatrixWidth; ++w)
        mapQuads[mapMatrixHeight - 1][w] = tempRow[w];
      break;
    case left:
      for (h = 0; h < mapMatrixHeight; ++h)
        tempCol[h] = mapQuads[h][mapMatrixWidth - 1];
      for (h = 0; h < mapMatrixHeight; ++h) 
        for (w = mapMatrixWidth - 1; w > 0; --w)
          mapQuads[h][w] = mapQuads[h][w - 1];
      for (h = 0; h < mapMatrixHeight; ++h)
        mapQuads[h][0] = tempCol[h];
      break;
    case right:
      for (h = 0; h < mapMatrixHeight; ++h)
        tempCol[h] = mapQuads[h][0];
      for (h = 0; h < mapMatrixHeight; ++h) 
        for (w = 0; w < mapMatrixWidth; ++w)
          mapQuads[h][w] = mapQuads[h][w + 1];
      for (h = 0; h < mapMatrixHeight; ++h)
        mapQuads[h][mapMatrixWidth - 1] = tempCol[h];
      break;
    default:
      break;
  }
  //DrawMap();
}

void RotateAround()
{
  byte y;
  for (y = 0; y < mapMatrixHeight; ++y)
  {
    Rotate(left);
    Rotate(up);
    DrawMiniMap(false);
  }
}*/
