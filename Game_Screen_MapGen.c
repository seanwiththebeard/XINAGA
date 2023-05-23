#include "game.h"
#include "xinaga.h"

#include <stdio.h>
#include <stdlib.h>

#define height 20
#define width 20
#define posX 1
#define posY 1
#define pointsCount 32
#define grass 0x88
#define water 0x84
byte map[height][width];

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


*/

typedef struct vector2
{
  int x, y;
  bool landlocked;
  struct vector2 *next;
};

struct vector2 *points = NULL;

byte CountPoints()
{
  struct vector2 *temp = points;
  byte i = 0;
  while(temp != NULL)
  {
    ++i;
    temp = temp->next;
  }
  return i;
}

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

void deletePoint(byte pos)
{
  int i;
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
        ptr=ptr->next ;
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
  }
}

bool checkPoints(byte index, byte w, byte h)
{
  byte i;
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
}

void clearPoints()
{
  while (points != NULL)
    deletePoint(0);
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

byte countAdjacent(byte x, byte y)
{
  byte i = 0;
  //for (i = 0; i < CountPoints(); ++i)
  struct vector2 north, south, east, west;

  north.x = x;
  north.y = y - 1;
  south.x = x;
  south.y = y + 1;
  east.x = x + 1;
  east.y = y;
  west.x = x - 1;
  west.y = y;

  clampPoint(&north);
  clampPoint(&south);
  clampPoint(&east);
  clampPoint(&west);

  if (map[north.y][north.x] != water)
    ++i;
  if (map[south.y][south.x] != water)
    ++i;
  if (map[east.y][east.x] != water)
    ++i;
  if (map[west.y][west.x] != water)
    ++i;
  return i;
}

void checkLandlocked()
{
  byte i;
  for (i = 0; i < CountPoints(); ++i)
  {
    struct vector2 *tmpt = getPoint(i);

    if (countAdjacent(tmpt->x, tmpt->y) == 4)
    {
      tmpt->landlocked = true;
      deletePoint(i);
    }
    else
      tmpt->landlocked = false;
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

void RemoveIslands()
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
}

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
    SetChar(index, posX + w, posY + h);
    //SetColor(index + 2, posX + w, posY + h);
  }
}

void attachRandomPoint(byte index)
{
  int i, x, y = 0;
  bool exit = false;
  byte failure = 0;
  
  while (1)
  {
    byte dir = rand() % 4;
    if (CountPoints() >= 1)
      i = rand() % (CountPoints());
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
      if (CountPoints() >= 1)
        i = rand() % (CountPoints());
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
  SetChar(index, posX + x, posY + y);
  //SetColor(index + 2, posX + x, posY + y);  
}

void DrawMap()
{
  byte x, y;
  DrawBorder("Map Generator@",posX - 1, posY - 1, width + 2, height + 2, false);
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
}

byte countContinents = 0;

void createContinent(byte size)
{
  byte index = grass;//'0' + countContinents;
  byte landcount = size + 1;
  addRandomPoints(1, index);
  while (landcount && (points != NULL))
  {
    attachRandomPoint(index);
    checkLandlocked();
    --landcount;
  }
  ++countContinents;
  clearPoints();
  DrawMap();
}
void Rotate(direction dir)
{
  int h, w = 0;
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
  DrawMap();
}

void GenerateMap(byte seed)
{
  byte x, y, failures = 0;
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
  DrawMap();
  srand(seed);
  for ( y = 6; y > 0; --y)
  {
    createContinent(16 +  8*(y / 4));
  }
  DrawMap();
  sprintf(strTemp, "Done@");
  WriteLineMessageWindow(strTemp, 0);


  //clearPoints();

  //createContinent(16);

  //addRandomPoints(pointsCount);
  //clearPoints();

  //checkLandlocked();
  //FillAdjacent(2, 2);
  //RemoveIslands();

  while(1)
  {
    UpdateInput();
    if (InputUp())
      Rotate(up);
    if (InputDown())
      Rotate(down);
    if (InputLeft())
      Rotate(left);
    if (InputRight())
      Rotate(right);    
    //Rotate(down);
    //Rotate(right);

    if (InputFire())
      return;
  }
  /*for (x = 0; x < pointsCount; ++x)
  {

    if (getPoint(x)->x % 2 == 0)
      map[getPoint(x)->y][getPoint(x)->x] = grass;
    else
      map[getPoint(x)->y][getPoint(x)->x] = grass + 1;
  }

  for (y = 0; y < height; ++y)
    for (x = 0; x < width; ++x)
    {
      SetChar(map[y][x], posX + x, posY + y);
    }*/
}


screenName Update_MapGen()
{
  byte seed  = 0;
  screenName nextScreen = Title;
  bool exit = false;
  BlankMessageWindow();
  DrawBorder("Map Generator@",posX - 1, posY - 1, width + 2, height + 2, true);
  while(1)
  {
    GenerateMap(seed);
    ++seed;
  }
  while (!exit)
  {
    UpdateInput();
    if (InputChanged())
    {
      if (InputUp())
      {
        ++seed;
        GenerateMap(seed);
        break;
        //exit = true;
      }
      if (InputFire())
      {
        exit = true;
      }
    }
  }

  return nextScreen;
}