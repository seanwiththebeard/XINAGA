#include "game.h"
#include "xinaga.h"

#include <stdio.h>
#include <stdlib.h>

#define height 16
#define width 16
#define posX 1
#define posY 1
#define pointsCount 64
byte map[height][width];

typedef struct vector2
{
  char x, y;
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

bool checkPoints(byte w, byte h)
{
  byte i;
  for (i = 0; i < CountPoints(); ++i)
  {
    if (getPoint(i)->x == w)
      if (getPoint(i)->y == h)
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
  while (CountPoints())
    deletePoint(0);
}

void clampPoint(struct vector2 *clmpt)
{
  if (clmpt->x >= width)
    clmpt->x = 0;
  if (clmpt->y >= height)
    clmpt->y = 0;
}
void checkLandlocked()
{
  byte i;
  for (i = 0; i < CountPoints(); ++i)
  {
    struct vector2 *tmpt = getPoint(i);
    struct vector2 north, south, east, west;
    north.x = tmpt->x;
    north.y = tmpt->y - 1;
    south.x = tmpt->x;
    south.y = tmpt->y + 1;
    east.x = tmpt->x + 1;
    east.y = tmpt->y;
    west.x = tmpt->x - 1;
    west.y = tmpt->y;
    
    clampPoint(&north);
    clampPoint(&south);
    clampPoint(&east);
    clampPoint(&west);
    
    if (checkPoints(north.x, north.y))
      tmpt->landlocked = true;
    else
      tmpt->landlocked = false;
  }
}

void GenerateMap(byte seed)
{
  byte x, y, failures = 0;
  #define grass 0x88
  #define water 0x84
  
  clearPoints();
  
  sprintf(strTemp, "Seed (%d)@", seed);
      WriteLineMessageWindow(strTemp, 0);
  
  for (y = 0; y < height; ++y)
    for (x = 0; x < width; ++x)
    {
      if (x % 2 == 0)
        map[y][x] = water;
      else
        map[y][x] = water + 1;
      
      SetChar(map[y][x], posX + x, posY + y);
    }
  srand(seed);
  for (x = 0; x < pointsCount; ++x)
  {
    byte h = rand() % height;
    byte w = rand() % width;

    while (checkPoints(w, h))
    {
      h = rand() % height;
      w = rand() % width;
      failures++;
    }
    createPoint(w, h);
    SetChar(grass, posX + w, posY + h);
    
    sprintf(strTemp, "Count (%d)@", x);
      WriteLineMessageWindow(strTemp, 0);
  }
  
  sprintf(strTemp, "failures (%d)@", failures);
      WriteLineMessageWindow(strTemp, 0);

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