#include "Xinaga.h"
#include "GameData.h"
#define consolePosX 1
#define consolePosY 18
#define consoleWidth 29
#define consoleHeight 5
#define consoleDelay 1

//Prototypes
//byte ReadBit(byte byteToRead, char bit);//These are old
void CameraFollow();
void BufferCharacters();
void FillQuadBuffer();
void LoadQuadrant(byte quadIndex, byte quad);
void LoadMapQuads();
byte GetPlayerQuad(); //Returns the viewport quadrant of the player character
byte GetQuadInRelation(bool d_up, bool d_down, bool d_left, bool d_right);
void QuadScroll(byte direction);
void InitializeMapData();
int wrapX(int posX); //Used in map positions
int wrapY(int posY);
bool CheckCollision(byte charIndex, byte Direction);
void DrawSquare(sbyte xOrigin, sbyte yOrigin, sbyte xSize, sbyte ySize);
void ApplyLOS();
void DrawEntireMap();
void MoveCharacter(byte index, byte dir, bool cameraUpdate);
void LoadMap();
void DrawCharacterCoordinates(byte index);

//Globals
#define EmptyTile 7
byte checkCollision = 0;
bool scrollQuads = false;
bool changedQuad = false;
//byte byte_index = 0;
//byte byte_offset = 0;
byte playerX = 0; //Player Position in line-of-sight calculations
byte playerY = 0; //Player Position in line-of-sight calculations

//Map Data
bool LOSEnabled = true;
#define mapHeight 32
#define mapWidth 32
byte mapData[mapWidth][mapHeight] = {};
#define mapQuadWidth 8
#define mapQuadHeight 8
#define mapMatrixWidth 8
#define mapMatrixHeight 8
byte mapQuads[mapMatrixHeight][mapMatrixWidth] = { //Map Data
  { 0,  1,  2,  3,  4,  5,  6,  7},
  { 8,  9, 10, 11, 12, 13, 14, 15},
  {16, 17, 18, 19, 20, 21, 22, 23},
  {24, 25, 26, 27, 28, 29, 30, 31},
  {32, 33, 34, 35, 36, 37, 38, 39},
  {40, 41, 42, 43, 44, 45, 46, 47},
  {48, 49, 50, 51, 52, 53, 54, 55},
  {56, 57, 58, 59, 60, 61, 62, 63}
};
byte quadBuffer[4] = {0,0,0,0};
int quadX = 0;
int quadY = 0;
#define quadWidth 8
#define quadHeight 8
const byte quadWidthDouble = quadWidth * 2;
const byte quadHeightDouble = quadHeight * 2;
const byte yQuadHeight = 2*mapQuadHeight;

//Viewport
#define viewportPosX 1
#define viewportPosY 1
#define viewportWidth 11
#define viewportHeight 7
byte viewportSize = viewportHeight * viewportWidth;

byte viewportBuffer[viewportWidth][viewportHeight] = {};
byte viewportBufferLast[viewportWidth][viewportHeight] = {};

byte followIndex = 0;

//QuadScroll
byte QuadOriginX = 0;
byte QuadOriginY = 0;
byte quadA = 0; //Entering quad
byte quadB = 0; //Diagonal quad
byte indexA = 0;
byte indexB = 0;
byte compareQuad = 0;

//Camera Position
int offsetX = 0;
int offsetY = 0;
byte cameraOffsetX = 0;
byte cameraOffsetY = 0;

//Tile Data
struct Tile
{
  byte chars[4];
  //byte colors[4];
  //byte index;
  byte blocked;
  byte trigger;
  byte opaque;
} tiles[64] = {};

struct
{
  int CharIndex[4];
  byte Chars[2];
  byte ScatterIndex;
  byte NPCIndex;
  byte MusicIndex;
}ScreenQuad[64] = {};

#define charactersCount 16
struct Character
{
  byte tile;
  byte chars[4];
  byte colors[4];
  byte trigger;
  byte combat;
  int posX;
  int posY;
  int quadPosX;
  int quadPosY;
  bool visible;
  bool collide;
  byte message;
} characters[charactersCount] = {};

byte ReadBit(byte byteToRead, char bit)//These are old
{
  bit = 1 << bit;
  return(bit & byteToRead);
}

void CameraFollow()
{
  byte byte_x, byte_y;
  offsetX = characters[followIndex].posX;
  offsetY = characters[followIndex].posY;

  for(byte_x = 0; byte_x < cameraOffsetX; ++byte_x)
  {
    --offsetX;
    if(offsetX >= mapWidth)
      offsetX = 0;
    if (offsetX < 0)
      offsetX = mapWidth - 1;
  }

  for(byte_y = 0; byte_y < cameraOffsetY; ++byte_y)
  {
    --offsetY;
    if(offsetY >= mapHeight)
      offsetY = 0;
    if (offsetY < 0)
      offsetY = mapHeight - 1;
  }
}

void BufferCharacters()
{
  byte byte_x, byte_y, byte_i;
  for(byte_i = 0; byte_i < charactersCount; ++byte_i)
  {
    if(characters[byte_i].visible)
    {
      byte_x = characters[byte_i].posX;
      if(byte_x < offsetX)
        byte_x = (byte_x - offsetX + mapWidth);
      else
        byte_x = byte_x - offsetX;

      if (byte_x < viewportWidth)
      {
        byte_y = characters[byte_i].posY;
        if (byte_y < offsetY)
          byte_y = byte_y - offsetY + mapHeight;
        else
          byte_y = byte_y - offsetY;
        if (byte_y < viewportHeight)
          viewportBuffer[byte_x][byte_y] = characters[byte_i].tile;
      }
    }
  }
}

void FillQuadBuffer()
{
  byte byte_x, byte_y;

  quadX = characters[followIndex].quadPosX;
  quadY = characters[followIndex].quadPosY;

  if (quadX + 1 == mapQuadWidth)
    byte_x = 0;
  else
    byte_x = quadX + 1;

  if (quadY + 1 == mapQuadHeight)
    byte_y = 0;
  else
    byte_y = quadY + 1;

  quadBuffer[0] = mapQuads[quadY][quadX];
  quadBuffer[1] = mapQuads[quadY][byte_x];
  quadBuffer[2] = mapQuads[byte_y][quadX];
  quadBuffer[3] = mapQuads[byte_y][byte_x];
}



void LoadQuadrant(byte quadIndex, byte quad)
{
  byte byte_x, byte_y, byte_z;
  int chardata;

  //sprintf(str, "Tile%d to Quad%d@", index, quad);
  //WriteLineMessageWindow(str, 1);

  quadBuffer[quad] = quadIndex;

  for (byte_z = 0; byte_z < 4; ++byte_z)
  {
    switch (quad)
    {
      case 0:
        QuadOriginX = 0;
        QuadOriginY = 0;
        break;
      case 1:
        QuadOriginX = quadWidthDouble;
        QuadOriginY = 0;
        break;
      case 2:
        QuadOriginX = 0;
        QuadOriginY = quadHeightDouble;
        break;
      case 3:
        QuadOriginX = quadWidthDouble;
        QuadOriginY = quadHeightDouble;
        break;
      default:
        break;
    }

    switch (byte_z)
    {
      case 0:
        break;
      case 1:
        QuadOriginX += quadWidth;
        break;
      case 2:
        QuadOriginY += quadHeight;
        break;
      case 3:
        QuadOriginX += quadWidth;
        QuadOriginY += quadHeight;
        break;
      default:
        break;
    }

    chardata = (int)&MapSetInfo[0] + 8*ScreenQuad[quadIndex].CharIndex[byte_z];
    for (byte_y = 0; byte_y < quadHeight; ++byte_y)
    {
      for (byte_x = 0; byte_x < quadWidth; ++byte_x)
      {
        if (ReadBit(PEEK(chardata + byte_y), 7 - byte_x) > 0)
        {
          mapData[byte_x + QuadOriginX][byte_y + QuadOriginY] = ScreenQuad[quadIndex].Chars[1];
        }
        else
        {
          mapData[byte_x + QuadOriginX][byte_y + QuadOriginY] = ScreenQuad[quadIndex].Chars[0];
        }
      }
    }
  }
}


void LoadMapQuads()
{
  byte x;
  FillQuadBuffer();
  for (x = 0; x < 4; ++x)
    LoadQuadrant(quadBuffer[x], x);
}

byte GetPlayerQuad() //Returns the viewport quadrant of the player character
{
  if (characters[followIndex].posX < quadWidthDouble)
  {
    if (characters[followIndex].posY < yQuadHeight)
      return 0;
    else
      return 2;
  }
  else
  {
    if (characters[followIndex].posY < yQuadHeight)
      return 1;
    else
      return 3;
  }
}

byte GetQuadInRelation(bool d_up, bool d_down, bool d_left, bool d_right)
{
  int int_x, int_y;
  int_x = characters[followIndex].quadPosX;
  int_y = characters[followIndex].quadPosY;
  if (d_up)
  {
    --int_y;
    if (int_y < 0)
      int_y = mapMatrixHeight - 1;
  }
  if (d_down)
  {
    int_y++;
    if (int_y == mapMatrixHeight)
      int_y = 0;
  }
  if (d_left)
  {
    int_x--;
    if (int_x < 0)
      int_x = mapMatrixWidth - 1;
  }
  if (d_right)
  {
    ++int_x;
    if (int_x == mapMatrixWidth)
      int_x = 0;
  }
  return (mapQuads[int_y][int_x]);  
}

void QuadScroll(byte dir)
{
  byte p = GetChar(COLS - 1, ROWS - 1);
  bool charPosX, charPosY;
  SetChar('Q', COLS - 1, ROWS - 1);
  QuadOriginX = characters[followIndex].quadPosX;
  QuadOriginY = characters[followIndex].quadPosY;
  compareQuad = GetPlayerQuad();

  charPosX = characters[followIndex].posX % 16 < quadWidth;
  charPosY = characters[followIndex].posY % 16 < quadHeight;

  switch(dir)
  {
    case 0:
      indexA = GetQuadInRelation(true, false, false, false);
      if (charPosX)
        indexB = GetQuadInRelation(true, false, true, false);
      else
        indexB = GetQuadInRelation(true, false, false, true);
      break;
    case 1:
      indexA = GetQuadInRelation(false, true, false, false);
      if (charPosX)
        indexB = GetQuadInRelation(false, true, true, false);
      else
        indexB = GetQuadInRelation(false, true, false, true);
      break;
    case 2:
      indexA = GetQuadInRelation(false, false, true, false);
      if (charPosY)
        indexB = GetQuadInRelation(true, false, true, false);
      else
        indexB = GetQuadInRelation(false, true, true, false);
      break;
    case 3:
      indexA = GetQuadInRelation(false, false, false, true);
      if (charPosY)
        indexB = GetQuadInRelation(true, false, false, true);
      else
        indexB = GetQuadInRelation(false, true, false, true);
      break;
  }

  if (dir < 2)
    switch (compareQuad)
    {
      case 0:
        quadA = 2;
        quadB = 3;
        break;
      case 1:
        quadA = 3;
        quadB = 2;
        break;
      case 2:
        quadA = 0;
        quadB = 1;
        break;
      case 3:
        quadA = 1;
        quadB = 0;
        break;
    }
  else
    switch (compareQuad)
    {
      case 0:
        quadA = 1;
        quadB = 3;
        break;
      case 1:
        quadA = 0;
        quadB = 2;
        break;
      case 2:
        quadA = 3;
        quadB = 1;
        break;
      case 3:
        quadA = 2;
        quadB = 0;
        break;
    }

  if (quadBuffer[quadA] != indexA)
  {
    LoadQuadrant(indexA, quadA);
  }
  if (quadBuffer[quadB] != indexB)
  {
    LoadQuadrant(indexB, quadB);
  }
  SetChar(p, COLS - 1, ROWS - 1);
}

void InitializeMapData()
{
  #define grass 36
  #define water 34
  #define signpost 35
  byte byte_x, byte_y, byte_i, byte_index, byte_offset;

  cameraOffsetX = viewportWidth / 2;
  cameraOffsetY = viewportHeight / 2;

  for (byte_y = 0; byte_y < 8; ++byte_y)
    for (byte_x = 0; byte_x < 8; ++byte_x)
    {
      byte_index = byte_x + byte_y*8;
      byte_offset = byte_x * 2 + 32*byte_y;

      //tiles[byte_index].index = byte_index; // Init tileset data for 64 tiles
      tiles[byte_index].chars[0] = byte_offset;
      tiles[byte_index].chars[1] = byte_offset + 1;
      tiles[byte_index].chars[2] = byte_offset + 16;
      tiles[byte_index].chars[3] = byte_offset + 17;

      tiles[byte_index].blocked = 0;

      ScreenQuad[byte_index].CharIndex[0] = byte_offset; // Init screen quad prefabs for 8x8
      ScreenQuad[byte_index].CharIndex[1] = byte_offset + 1;
      ScreenQuad[byte_index].CharIndex[2] = byte_offset + 16;
      ScreenQuad[byte_index].CharIndex[3] = byte_offset + 17;
      ScreenQuad[byte_index].Chars[0] = 32;
      ScreenQuad[byte_index].Chars[1] = byte_index;
      ScreenQuad[byte_index].MusicIndex = 0;
      ScreenQuad[byte_index].NPCIndex = 0;
      ScreenQuad[byte_index].ScatterIndex = 0;
    }

  tiles[44].opaque = true;

  ScreenQuad[2].Chars[0] = 36; // Set the wizard to grass on 0
  ScreenQuad[2].Chars[1] = 44; // Set the wizard to trees on 1

  //Init Characters
  for (byte_i = 0; byte_i < charactersCount; ++byte_i)
  {
    characters[byte_i].tile = byte_i;
    characters[byte_i].chars[0] = 8 + byte_i * 16;
    characters[byte_i].chars[1] = 9 + byte_i * 16;
    characters[byte_i].chars[2] = 10 + byte_i * 16;
    characters[byte_i].chars[3] = 11 + byte_i * 16;
    characters[byte_i].colors[0] = byte_i + 1;
    characters[byte_i].colors[1] = byte_i + 1;
    characters[byte_i].colors[2] = byte_i + 1;
    characters[byte_i].colors[3] = byte_i + 1;
    characters[byte_i].posX = byte_i;
    characters[byte_i].posY = byte_i;
    characters[byte_i].quadPosX = byte_i;
    characters[byte_i].quadPosX = byte_i;
    characters[byte_i].visible = false;
    characters[byte_i].collide = false;
  }
  characters[0].visible = true;
  characters[0].posX  = 8;
  characters[0].posY  = 8;
  characters[0].quadPosX  = 2;
  characters[0].quadPosY  = 0;
  characters[0].tile = 2;

  characters[1].visible = true;
  characters[1].collide = true;
  characters[1].message = 0;

  characters[2].tile = signpost;
  characters[2].visible = true;
  characters[2].collide = true;
  characters[2].message = 1;
  characters[2].posX = 8;
  characters[2].posY = 6;
  characters[2].quadPosX  = 2;
  characters[2].quadPosY  = 0;

  LoadMapQuads();
}

int wrapX(int posX) //Used in map positions
{
  if (posX < 0)
    return mapWidth - 1;
  if (posX >= mapWidth)
    return 0;
  return posX;
}

int wrapY(int posY)
{
  if (posY < 0)
    return mapHeight - 1;
  if (posY >= mapHeight)
    return 0;
  return posY;
}

bool CheckCollision(byte charIndex, byte Direction)
{
  byte byte_i;
  int xPos, yPos; //These need to be integers because they can wrap around the map
  xPos = characters[charIndex].posX;
  yPos = characters[charIndex].posY;

  //Check the tile we're already standing on
  if(ReadBit(tiles[mapData[xPos][yPos]].blocked, Direction))
  {
    //WriteLineMessageWindow("Standing on blocked@", 0);
    return true;
  }

  switch (Direction)
  {
    case up:
      yPos = wrapY(yPos - 1);
      Direction = 1;
      break;
    case down:
      yPos = wrapY(yPos + 1);
      Direction = 0;
      break;
    case left:
      xPos = wrapX(xPos - 1);
      Direction = 3;
      break;
    case right:
      xPos = wrapX(xPos + 1);
      Direction = 2;
      break;
    default:
      return false;
  }

  if(ReadBit(tiles[mapData[xPos][yPos]].blocked, Direction))
  {
    /*WriteLineMessageWindow("Entry blocked@", 1);
    sprintf(str, "Index: %d@", tiles[mapData[xPos][yPos]].index);
    WriteLineMessageWindow(str, 1);
    sprintf(str, "Data: %d@", tiles[mapData[xPos][yPos]].blocked);
    WriteLineMessageWindow(str, 1);
    sprintf(str, "position: %d,%d@", xPos, yPos);
    WriteLineMessageWindow(str, 1);*/
    return true;
  }

  //Call Messagebox from NPC
  for (byte_i = 0; byte_i < charactersCount; ++byte_i)
    if(characters[byte_i].collide)
      if (characters[byte_i].posX == xPos)
        if (characters[byte_i].posY == yPos)
        {
          WriteLineMessageWindow(Messages[characters[byte_i].message], 1);
          return true;
        }

  return false;
}

void DrawSquare(sbyte xOrigin, sbyte yOrigin, sbyte xSize, sbyte ySize)
{
  byte x, y;

  if (xOrigin < 0 || yOrigin < 0 || xOrigin >= viewportWidth || yOrigin >= viewportHeight)
    return;

  while (xOrigin + xSize > viewportWidth)
    --xSize;

  while (yOrigin + ySize > viewportHeight)
    --ySize;

  while (xSize < 1)
    ++xSize;

  while (ySize < 1)
    ++ySize;

  for(y = 0; y < ySize; ++y)
  {
    for(x = 0; x < xSize; ++x)
      viewportBuffer[x + xOrigin][y + yOrigin] = EmptyTile;
  }
}

void ApplyLOS()
{
  if (LOSEnabled)
  {
    //Quadrant Layout:
    //        ^
    //        |       ^
    //  000  666  111 |
    //  000  666  111  ->
    //  000  666  111
    //
    //  444  XXX  555
    //  444  XPX  555 ->
    //  444  XXX  555
    //
    //  333  777  222
    //  333  777  222
    //  333  777  222
    //
    //Center adjacent X always visible
    //Diagonal quadrants 0-3 block everything behind the tile
    //Cardinal quadrants 4-7 block only the tiles directly behind them

    byte x, y;
    //Quad 0
    for(y = playerY - 1; y > 0; --y)
      for(x = playerX - 1; x > 0; --x)
        if (tiles[viewportBuffer[x][y]].opaque)
        {
          DrawSquare(0, y, x, 1);
          DrawSquare(0, 0, x + 1, y);
        }
    //Quad 1
    for(y = playerY - 1; y > 0; --y)
      for(x = playerX + 1; x < viewportWidth; ++x)
        if (tiles[viewportBuffer[x][y]].opaque)
        {
          DrawSquare(x + 1, y, viewportWidth - x, 1);
          DrawSquare(x, 0, viewportWidth - x, y);
        }
    //Quad 2
    for(y = playerY + 1; y < viewportHeight; ++y)
      for(x = playerX + 1; x < viewportWidth; ++x)
        if (tiles[viewportBuffer[x][y]].opaque)
        {
          DrawSquare(x + 1, y, viewportWidth - x, 1);
          DrawSquare(x, y + 1, viewportWidth - x, viewportHeight - y);
        }
    //Quad 3
    for(y = playerY + 1; y < viewportHeight; ++y)
      for(x = playerX - 1; x > 0; --x)
        if (tiles[viewportBuffer[x][y]].opaque)
        {
          DrawSquare(0, y, x, 1);
          DrawSquare(0, y + 1, x + 1, viewportHeight - y);
        }
    //Horizontal
    for(x = playerX - 1; x > 0; --x)
      for(y = playerY - 1; y <= playerY + 1; ++y)
        if (tiles[viewportBuffer[x][y]].opaque)
          DrawSquare(0, y, x, 1);
    for(x = playerX + 1; x < viewportWidth; ++x)
      for(y = playerY - 1; y <= playerY + 1; ++y)
        if (tiles[viewportBuffer[x][y]].opaque)
          DrawSquare(x + 1, y, viewportWidth - x - 1, 1);
    //Vertical
    for(y = playerY - 1; y > 0; --y)
      for(x = playerX -1 ; x <= playerX + 1; ++x)
        if (tiles[viewportBuffer[x][y]].opaque)
          DrawSquare(x, 0, 1, y);
    for(y = playerY + 1; y < viewportHeight; ++y)
      for(x = playerX -1 ; x <= playerX + 1; ++x)

        if (tiles[viewportBuffer[x][y]].opaque)
          DrawSquare(x, y + 1, 1, viewportHeight - y);

  }
}

const byte viewportsize = viewportHeight * viewportWidth;
void DrawEntireMap()
{
  byte byte_x, byte_y;
  int int_a, int_b;
  #if defined(__C64__)
  StoreBuffer();
  #endif
  //Buffer the matrix of tiles for our viewport
  CameraFollow();
  int_a = offsetX;
  int_b = offsetY;
  for(byte_y = 0; byte_y < viewportHeight; ++byte_y)
  {
    int_b = wrapY(int_b); //Wrap the map data y reference
    for(byte_x = 0; byte_x < viewportWidth; ++byte_x)
    {
      int_a = wrapX(int_a); //Wrap the map data X reference
      viewportBuffer[byte_x][byte_y] = mapData[int_a][int_b];
      int_a++;
    }
    int_a = offsetX;
    ++int_b;
  }
  BufferCharacters();
  ApplyLOS();

  for(byte_y = 0; byte_y < viewportHeight; ++byte_y)
  {      
    for(byte_x = 0; byte_x < viewportWidth; ++byte_x)
    { //On Apple, only draw tiles that are different from the last draw; minimal effect on smaller screen sizes
      byte lastIndex = viewportBufferLast[byte_x][byte_y];
      byte newIndex = viewportBuffer[byte_x][byte_y];
      #if defined(__APPLE2__)
      //if(viewportBuffer[byte_x][byte_y] != viewportBufferLast[byte_x][byte_y]); //It's acrtually faster to skip this on Commodore
      #endif
      if (lastIndex!=newIndex)
        //DrawTileDirect(newIndex, byte_x, byte_y);
      {
        DrawTileIndex = newIndex;
        DrawTileX = byte_x;
        DrawTileY = byte_y;
        DrawTileBuffer();
      }
    }
  }
  #if defined(__APPLE2__)
  //memcpy(&viewportBufferLast[0][0], &viewportBuffer[0][0], viewportsize); //It's acrtually faster to skip this on Commodore
  #endif
  #if defined(__C64__)
  SwapBuffer();
  #endif
  memcpy(&viewportBufferLast[0][0], &viewportBuffer[0][0], viewportSize);

  DrawCharacterCoordinates(followIndex);
}

void MoveCharacter(byte index, byte dir, bool cameraUpdate)
{
  checkCollision = CheckCollision(index, dir);
  scrollQuads = false;
  changedQuad = false;

  TickMoonPhase();

  if(!checkCollision)
  {
    switch (dir)
    {
      case up:
        --characters[index].posY;
        if (characters[index].posY < 0)
          characters[index].posY = mapHeight - 1;
        if (characters[index].posY == 15 || characters[index].posY == 31)
        {
          changedQuad = true;
          --characters[index].quadPosY;
          if(characters[index].quadPosY < 0)
            characters[index].quadPosY = mapMatrixHeight - 1;
        }
        break;
      case down:
        ++characters[index].posY;
        if (characters[index].posY >= mapHeight)
          characters[index].posY = 0;
        if (characters[index].posY == 0 || characters[index].posY == 16)
        {
          changedQuad = true;
          ++characters[index].quadPosY;
          if(characters[index].quadPosY == mapMatrixHeight)
            characters[index].quadPosY = 0; 
        }
        break;
      case left:
        --characters[index].posX;
        if (characters[index].posX < 0)
          characters[index].posX = mapWidth - 1;
        if (characters[index].posX == 15 || characters[index].posX == 31)
        {
          changedQuad = true;
          --characters[index].quadPosX;
          if(characters[index].quadPosX < 0)
            characters[index].quadPosX = mapMatrixWidth - 1; 
        }
        break;
      case right:
        ++characters[index].posX;
        if (characters[index].posX >= mapWidth)
          characters[index].posX = 0;
        if (characters[index].posX == 0 || characters[index].posX == 16)
        {
          changedQuad = true;
          ++characters[index].quadPosX;
          if(characters[index].quadPosX == mapMatrixWidth)
            characters[index].quadPosX = 0; 
        }
        break;
      default:
        break;
    }

    if (index == followIndex)
    {
      byte edgeCheckX = characters[index].posX % 16;
      byte edgeCheckY = characters[index].posY % 16;

      switch (dir)
      {
        case 0:
          if (edgeCheckY == 6)
            scrollQuads = true;
          break;
        case 1:
          if (edgeCheckY == 10)
            scrollQuads = true;
          break;
        case 2:
          if (edgeCheckX == 6)
            scrollQuads = true;
          break;
        case 3:
          if (edgeCheckX == 10)
            scrollQuads = true;
          break;
      }
      /*
      if ((direction == 0 && edgeCheck == 6)
          || (direction == 1  && edgeCheck == 10)
          || (direction == 2 && edgeCheck == 6)
          || (direction == 3 && edgeCheck == 10))
        scrollQuads = true;*/

      if(cameraUpdate)
        CameraFollow();

      DrawEntireMap();

      if (scrollQuads)
        QuadScroll(dir);
    }
  }
}

void DrawCharacterCoordinates(byte index)
{
  sprintf(strTemp,"(%2d,%2d)@", characters[index].posX, characters[index].posY);
  PrintString(strTemp, viewportPosX + 6, viewportPosY - 1, true, false);
}

void LoadMap()
{
  /*viewportPosX = MapOriginX;
  viewportPosY = MapOriginY;

  while (viewportPosX + (2*viewportWidth) >= COLS)
    --viewportPosX;
  while (viewportPosY + (2*viewportHeight) >= ROWS)
    --viewportPosY;

  while (viewportPosX < 1)
    ++viewportPosX;
  while (viewportPosY < 1)
    ++viewportPosY;*/

  SetTileOrigin(viewportPosX, viewportPosY);

  InitializeMapData();
  playerX = (viewportWidth - 1) / 2;
  playerY = (viewportHeight - 1) / 2;

  viewportSize = viewportHeight * viewportWidth;
}

screenName MapUpdate()
{
  bool exit = false;
  ClearScreen();
  //screenName nextScreen;
  DrawBorder("Map@", viewportPosX - 1, viewportPosY - 1, viewportWidth* 2 + 2, viewportHeight * 2 + 2, true);
  ResizeMessageWindow(consolePosX, consolePosY, consoleWidth, consoleHeight);
  //DrawMessageWindow();
  DrawEntireMap();
  DrawCharStats();

  while (!exit)
  {
    //if (InputChanged())
    UpdateInput();
    {
      direction Dir = 4;
      if (InputUp())
      	Dir = up;
      if (InputDown()) 
        Dir = down;
      if (InputLeft())
        Dir = left;
      if (InputRight())
        Dir = right;
      if (InputFire())
        exit = true;
      
      if (Dir < 4)
        MoveCharacter(followIndex, Dir, true);        
    }
  }
  return EditParty;
  //return nextScreen;
}