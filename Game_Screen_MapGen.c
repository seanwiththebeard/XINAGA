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

#define pointsBase 48
#define continentsBase 4
#define water 0xE0
#define grass 0xF0
byte countContinents;

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
*/

typedef struct vector2
{
  sbyte x;
  sbyte y;
  struct vector2 *next;
};

struct vector2 *points;

byte totalPoints;

void createPoint(byte x, byte y)
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

struct vector2 pointAdj;
byte countAdjacent(byte x, byte y)
{
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
    if (mapQuads[adjX[z] + (mapMatrixWidth * adjY[z])] != water)
      ++i;
  }
  return i;
}

void checkLandlocked()
{
  byte i;
  for (i = 0; i < totalPoints; ++i)
  {
    struct vector2 *tmpt = getPoint(i);

    if (countAdjacent(tmpt->x, tmpt->y) == 4)
    {
      deletePoint(i);
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

    while (mapQuads[w + (mapMatrixWidth * h)] != water)
    {
      h = rand() % mapMatrixHeight;
      w = rand() % mapMatrixWidth;
    }
    createPoint(w, h);
    mapQuads[w+ (mapMatrixWidth * h)] = index;
  }
}

void DrawPoint(byte x, byte y)
{
  byte tile = mapQuads[x + (mapMatrixWidth * y)];
  //tile = (tile << 1) + ((tile >> 3) << 4);
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

    if (mapQuads[x + (mapMatrixWidth * y)] == water)
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
  createPoint(x, y);
  mapQuads[x + (mapMatrixWidth * y)] = index;
  DrawPoint(x,y);
}

void createContinent(byte size)
{
  byte landcount = size + 1;  
  addRandomPoints(1, grass);
  while (landcount && (points != NULL))
  {
    attachRandomPoint(grass);
    --landcount;
  }
  
  checkLandlocked();
  
  ++countContinents;
  clearPoints();
}

void ClearMap()
{
  byte x, y;
  clearPoints();
  countContinents = 0;
  
  for (y = 0; y < mapMatrixHeight; ++y)
    for (x = 0; x < mapMatrixWidth; ++x)
    {
      mapQuads[x + (mapMatrixWidth * y)] = water;
      DrawPoint(x, y);
    }
}

void GenerateMap(byte seed)
{
  byte y;
  ClearMap();
  //DrawMiniMap(false);
  srand(seed);
  for ( y = continentsBase; y > 0; --y)
  {
    createContinent(pointsBase -  16*(y - 1));
  }
}

#define menuWidth 5
#define menuCount 4
void GetSeed()
{
  byte seed  = 0;
  bool exit = false;
  ResetMenu("Seed@", COLS - menuWidth - 3, consolePosY, menuWidth, consoleHeight, menuCount);
  SetMenuItem(0, "Next@");
  SetMenuItem(1, "Last@");
  SetMenuItem(2, "Go@");
  SetMenuItem(3, "End@");
  
  while(1)
  {
    sprintf(strTemp, "Seed (%d)@", seed);
    SetLineMessageWindow(strTemp, 0);
    GenerateMap(seed);
    ++seed;
  }

  while (!exit)
  {
    sprintf(strTemp, "Seed (%d)@", seed);
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
        GenerateMap(seed);
        //GenerateDungeon(seed);
        break;
      case 3:
        exit = true;
        break;
    }
  }
}

screenName Update_MapGen()
{
  ClearScreen();
  ClearMap();
  DrawMiniMap(false);  
  ResizeMessageWindow(consolePosX, consolePosY, consoleWidth - 12, consoleHeight);
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
