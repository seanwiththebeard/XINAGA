#include "Xinaga.h"
#include "GameData.h"

#if defined(__APPLE2__)
//#pragma code-name (push, "LC")
#endif

#define height 9
#define width 16
#define posX 1
#define posY 1
#define pointsCount 12
//#define grass 0x88
//#define water 0x84
#define grass 36
#define water 34
void DrawMapGenTiles(void);
byte map[height][width] = {};

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
  //bool landlocked;
  struct vector2 *next;
};

struct vector2 *points = NULL;

byte totalPoints = 0;
/*byte CountPoints()
{
  struct vector2 *temp = points;
  totalPoints = 0;
  while(temp != NULL)
  {
    ++totalPoints;
    temp = temp->next;
  }
  return totalPoints;
}*/

void createPoint(byte x, byte y)
{
  struct vector2 *temp,*ptr;
  temp=(struct vector2 *)malloc(sizeof(struct vector2));  

  if(temp==NULL)
    exit(0);

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

  if(points==NULL)
    exit(0);
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
    //sprintf(str, "Deleted element:%d",ptr->character.NAME);
    //WriteLineMessageWindow(str, 0);
    free(ptr);
    --totalPoints;
  }
}

/*bool checkPoints(byte index, byte w, byte h)
{
  byte i = 0;
  //for (i = 0; i < CountPoints(); ++i)
  {
    if (map[h][w] == index)
    {
      sprintf(strTemp, "Failed%d(%d,%d)@", i, w, h);
      WriteLineMessageWindow(strTemp, 0);
      return true;
    }
  }
  return false;
}*/

void clearPoints()
{
  while (points != NULL)
    deletePoint(0);
  totalPoints = 0;
}

void clampPoint(struct vector2 *clmpt)
{
  if (clmpt->x < 0)
    clmpt->x = width - 1;
  if (clmpt->y < 0)
    clmpt->y = height - 1;

  if (clmpt->x >= width)
    clmpt->x == 0;
  if (clmpt->y >= height)
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
    if (map[adjY[z]][adjX[z]] != water)
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
      //tmpt->landlocked = true;
      deletePoint(i);
    }
    //else
      //tmpt->landlocked = false;
  }
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

void addRandomPoints(byte count, int index)
{
  byte x;
  for (x = 0; x < count; ++x)
  {
    byte h = rand() % height;
    byte w = rand() % width;

    while (map[h][w] != water)
    {
      h = rand() % height;
      w = rand() % width;
    }
    createPoint(w, h);
    map[h][w] = index;
    //SetChar(index, posX + w, posY + h);
    //SetColor(index + 2, posX + w, posY + h);
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
          y = height - 1;
        break;
      case 1:
        ++y;
        if (y >= height)
          y = 0;
        break;
      case 2:
        --x;
        if (x < 0)
          x = width - 1;
        break;
      case 3:
        ++x;
        if (x >= width)
          x = 0;
        break;
      default:
        break;
    }

    if (map[y][x] == water)
      exit = true;

    if (exit)
      break;

    ++failure;
    if (failure == 48)
    {
      sprintf(strTemp, "Removing point (%d)@", i);
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
  map[y][x] = index;
  DrawTileIndex = index;
  DrawTileX = x;
  DrawTileY = y;
  DrawTileDirect();
  //SetChar(index, posX + x, posY + y);
  //SetColor(index + 2, posX + x, posY + y);  
}

/*void DrawMap()
{
  byte x, y;
  for (y = 0; y < height; ++y)
    for (x = 0; x < width; ++x)
    {
      byte index = map[y][x];
      #if(__APPLE2__)
      if (x % 2 == 1)
        ++index;
      #endif
      SetChar(index, posX + x, posY + y);
    }
}*/

#define MiniMapX 0
#define MiniMapY 0
#define MiniMapSizeX (width << 1)
#define MiniMapSizeY (height << 1)

void DrawMapGenTiles(void)
{
  byte x, y;
  DrawBorder("MapGen@", MiniMapX, MiniMapY, MiniMapSizeX + 2, MiniMapSizeY + 2, true);
  SetTileOrigin(MiniMapX + 1, MiniMapY + 1);
  for (y = 0; y < height; ++y)
    for (x = 0; x < width; ++x)
    {
      DrawTileIndex = map[y][x];
      DrawTileX = x;
      DrawTileY = y;
      DrawTileDirect();
    }
}

byte countContinents = 0;

void createContinent(byte size)
{
  //char index = '0' + countContinents;// grass;
  byte landcount = size + 1;
  
  //sprintf(&index, "%d", countContinents);
  
  addRandomPoints(1, grass);
  while (landcount && (points != NULL))
  {
    attachRandomPoint(grass);
    checkLandlocked();
    --landcount;
  }
  ++countContinents;
  clearPoints();
  //DrawMapGenTiles();
}

void Rotate(direction dir)
{
  byte h;
  byte w;
  byte tempRow[width];
  byte tempCol[height];
  switch (dir)
  {
    case up:
      for (w = 0; w < width; ++w)
        tempRow[w] = map[height - 1][w];
      for (h = height - 1; h > 0; --h)  
        for (w = 0; w < width; ++w)
          map[h][w] = map[h - 1][w];
      for (w = 0; w < width; ++w)
        map[0][w] = tempRow[w];
      break;
    case down:
      for (w = 0; w < width; ++w)
        tempRow[w] = map[0][w];
      for (h = 0; h < height; ++h)  
        for (w = 0; w < width; ++w)
          map[h][w] = map[h + 1][w];
      for (w = 0; w < width; ++w)
        map[height - 1][w] = tempRow[w];
      break;
    case left:
      for (h = 0; h < height; ++h)
        tempCol[h] = map[h][width - 1];
      for (h = 0; h < height; ++h) 
        for (w = width - 1; w > 0; --w)
          map[h][w] = map[h][w - 1];
      for (h = 0; h < height; ++h)
        map[h][0] = tempCol[h];
      break;
    case right:
      for (h = 0; h < height; ++h)
        tempCol[h] = map[h][0];
      for (h = 0; h < height; ++h) 
        for (w = 0; w < width; ++w)
          map[h][w] = map[h][w + 1];
      for (h = 0; h < height; ++h)
        map[h][width - 1] = tempCol[h];
      break;
    default:
      break;
  }
  //DrawMap();
}

void RotateAround()
{
  byte y;
  for (y = 0; y < height; ++y)
  {
    Rotate(left);
    Rotate(up);
    DrawMapGenTiles();
  }
}

void GenerateMap(byte seed)
{
  byte x, y;
  countContinents = 0;
  clearPoints();

  sprintf(strTemp, "Seed (%d)@", seed);
  WriteLineMessageWindow(strTemp, 0);

  for (y = 0; y < height; ++y)
    for (x = 0; x < width; ++x)
    {
      map[y][x] = water;
      //SetChar(map[y][x], posX + x, posY + y);
    }
  DrawMapGenTiles();
  srand(seed);
  for ( y = 4; y > 0; --y)
  {
    createContinent(16 +  8*(y / 4));
  }
  //DrawMapGenTiles();
  //RotateAround();
  //sprintf(strTemp, "Done@");
  //WriteLineMessageWindow(strTemp, 0);
}

void StoreMap()
{
  byte x, y;

  for (y = 0; y < height; ++y)
    for (x = 0; x < width; ++x)
    {
      mapQuads[y][x] = map[y][x];
    }
}
screenName Update_MapGen()
{
  byte seed  = 0;
  bool exit = false;
  ResizeMessageWindow(23, 12, 15, 10);
  //DrawBorder("Map Generator@",posX - 1, posY - 1, width + 2, height + 2, true);
  //DrawMapGenTiles();
  GenerateMap(seed);
  while (!exit)
  {
    UpdateInput();
    if (InputChanged())
    {
      if (InputUp())
      {
        ++seed;
        GenerateMap(seed);
        //break;
      }
      if (InputFire())
      {
        exit = true;
      }
    }
  }
  clearPoints();
  StoreMap();
  return EditParty;
}