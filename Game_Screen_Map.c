#include "Xinaga.h"
#include "GameData.h"

//Prototypes
//      Map Functions
void InitializeMapData();
void LoadMap();
void DrawEntireMap();
void wrapX(sbyte *posX); //Used in map positions
void wrapY(sbyte *posY);
void DrawSquare(sbyte xOrigin, sbyte yOrigin, sbyte xSize, sbyte ySize);
void ApplyLOS();
void CameraFollow();
void BufferCharacters();
void MoveCharacter(byte index, byte dir);
bool CheckCollision(byte charIndex, byte Direction);
void DrawCharacterCoordinates(byte index);
//      Quad Functions
void FillQuadBuffer();
void LoadQuadrant(byte quadIndex, byte quad);
void LoadMapQuads();
byte GetPlayerQuad(); //Returns the viewport quadrant of the player character
byte GetQuadInRelation(sbyte v, sbyte h);
void QuadScroll(byte direction);

//Globals
#define consolePosX 1
#define consolePosY 18
#define consoleWidth 29
#define consoleHeight 5
//#define consoleDelay 1

//Viewport
#define viewportPosX 1
#define viewportPosY 1
#define viewportWidth 11
#define viewportHeight 7
#define playerX ((viewportWidth - 1) >> 1) //Viewport Center used in line-of-sight calculations
#define playerY ((viewportHeight - 1) >> 1) //Viewport Center used in line-of-sight calculations
#define viewportSize viewportHeight * viewportWidth
byte viewportBuffer[viewportWidth][viewportHeight] = {};
byte viewportBufferLast[viewportWidth][viewportHeight] = {};
byte followIndex = 0;

//Camera Position
sbyte offsetX = 0;
sbyte offsetY = 0;

//Map Data
bool LOSEnabled = true;
#define EmptyTile 7
#define mapHeight 32
#define mapWidth 32
byte mapData[mapWidth][mapHeight] = {};

//Quad Data
#define mapQuadWidth 8
#define mapQuadHeight 8
#define mapMatrixWidth 8
#define mapMatrixHeight 8
byte mapQuads[mapMatrixHeight][mapMatrixWidth] =  //These are the quad-tile references that make up the map
{
  { 0,  1,  2,  3,  4,  5,  6,  7},
  { 8,  9, 10, 11, 12, 13, 14, 15},
  {16, 17, 18, 19, 20, 21, 22, 23},
  {24, 25, 26, 27, 28, 29, 30, 31},
  {32, 33, 34, 35, 36, 37, 38, 39},
  {40, 41, 42, 43, 44, 45, 46, 47},
  {48, 49, 50, 51, 52, 53, 54, 55},
  {56, 57, 58, 59, 60, 61, 62, 63}
};
struct
{ //These are the quad indexes referenced in mapQuads[y][x]
  #define ScreenQuadCount 64
  byte CharIndex[ScreenQuadCount][4]; //The graphic characters that make up the tile placement
  byte Chars[ScreenQuadCount][2]; //Which tiles for a zero or a 1 in the bits of a CharIndex
  byte ScatterIndex[ScreenQuadCount]; //Which fluff arrangement to add on top of above?
}ScreenQuad = {};
byte quadBuffer[4] = {0,0,0,0};

#define quadWidth 8
#define quadHeight 8
#define quadWidthDouble quadWidth << 1
#define quadHeightDouble quadHeight << 1
#define yQuadHeight mapQuadHeight << 1

//Tile Data
struct
{
  #define TileCount 64
  byte blocked[TileCount];
  byte opaque[TileCount];
} tiles = {};

struct
{
  #define charactersCount 8
  byte tile[charactersCount];
  byte combat[charactersCount];
  byte message[charactersCount];
  bool visible[charactersCount];
  bool collide[charactersCount];
  sbyte posX[charactersCount];
  sbyte posY[charactersCount];
  sbyte quadPosX[charactersCount];
  sbyte quadPosY[charactersCount];
} characters = {};

void CameraFollow()
{
  byte byte_x;
  byte byte_y;
  offsetX = characters.posX[followIndex];
  offsetY = characters.posY[followIndex];

  for(byte_x = 0; byte_x < playerX; ++byte_x)
  {
    --offsetX;
    wrapX(&offsetX);
  }

  for(byte_y = 0; byte_y < playerY; ++byte_y)
  {
    --offsetY;
    wrapY(&offsetY);
  }
}

void BufferCharacters()
{
  byte byte_x;
  byte byte_y;
  byte byte_i;
  for(byte_i = 0; byte_i < charactersCount; ++byte_i)
  {
    if(characters.visible[byte_i])
    {
      byte_x = characters.posX[byte_i];
      if(byte_x < offsetX)
        byte_x = (byte_x - offsetX + mapWidth);
      else
        byte_x = byte_x - offsetX;

      if (byte_x < viewportWidth)
      {
        byte_y = characters.posY[byte_i];
        if (byte_y < offsetY)
          byte_y = byte_y - offsetY + mapHeight;
        else
          byte_y = byte_y - offsetY;
        if (byte_y < viewportHeight)
          viewportBuffer[byte_x][byte_y] = characters.tile[byte_i];
      }
    }
  }
}

void FillQuadBuffer()
{
  byte byte_x;
  byte byte_y;
  byte quadX = characters.quadPosX[followIndex];
  byte quadY = characters.quadPosY[followIndex];

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
static const byte quadOriginsX[4] = {0, quadWidthDouble, 0, quadWidthDouble};
static const byte quadOriginsY[4] = {0, 0, quadHeightDouble, quadHeightDouble};
static const byte quadOffsetX[4] = {0, quadWidth, 0, quadWidth};
static const byte quadOffsetY[4] = {0, 0, quadHeight, quadHeight};

void LoadQuadrant(byte quadIndex, byte quad)
{
  int chardata;
  byte byte_x;
  byte byte_y;
  byte byte_z;
  byte charIndex;
  byte xPos;
  byte yPos;
  byte QuadOriginX;
  byte QuadOriginY;

  //sprintf(str, "Tile%d to Quad%d@", index, quad);
  //WriteLineMessageWindow(str, 1);

  quadBuffer[quad] = quadIndex;

  for (byte_z = 0; byte_z < 4; ++byte_z)
  {
    /*switch (quad)
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
    }*/
    

    /*switch (byte_z)
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
    }*/
    
    QuadOriginX = quadOriginsX[quad] + quadOffsetX[byte_z];
    QuadOriginY = quadOriginsY[quad] + quadOffsetY[byte_z];
    
    chardata = (int)&MapSetInfo[0] + 8*ScreenQuad.CharIndex[quadIndex][byte_z];
    for (byte_y = 0; byte_y < quadHeight; ++byte_y)
    {
      yPos = byte_y + QuadOriginY;
      for (byte_x = 0; byte_x < quadWidth; ++byte_x)
      {
        xPos = byte_x + QuadOriginX;
        if (ReadBit(PEEK(chardata + byte_y), 7 - byte_x) > 0)
          charIndex = 1;
        else
          charIndex = 0;
        mapData[xPos][yPos] = ScreenQuad.Chars[quadIndex][charIndex];
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
  if (characters.posX[followIndex] < quadWidthDouble)
  {
    if (characters.posY[followIndex] < yQuadHeight)
      return 0;
    else
      return 2;
  }
  else
  {
    if (characters.posY[followIndex] < yQuadHeight)
      return 1;
    else
      return 3;
  }
}

byte GetQuadInRelation(sbyte v, sbyte h)
{
  sbyte int_x = characters.quadPosX[followIndex];
  sbyte int_y = characters.quadPosY[followIndex];

  if (v < 0)
  {
    --int_y;
    if (int_y < 0)
      int_y = mapMatrixHeight - 1;
  }
  if (v > 0)
  {
    int_y++;
    if (int_y == mapMatrixHeight)
      int_y = 0;
  }
  if (h < 0)
  {
    int_x--;
    if (int_x < 0)
      int_x = mapMatrixWidth - 1;
  }
  if (h > 0)
  {
    ++int_x;
    if (int_x == mapMatrixWidth)
      int_x = 0;
  }
  return (mapQuads[int_y][int_x]);  
}

//left -UP DOWN LEFT RIGHT right UP DOWN LEFT RIGHT
static const byte quadRelationAV[8] = {-1,  1,  0,  0, -1, 1,  0, 0}; //vA
static const byte quadRelationBV[8] = {-1,  1, -1, -1, -1, 1,  1, 1}; //vB
static const byte quadRelationAH[8] = { 0,  0, -1,  1,  0, 0, -1, 1}; //hA
static const byte quadRelationBH[8] = {-1, -1, -1,  1,  1, 1, -1, 1}; //hB
static const byte CompareQuadValueA[8] = {2, 3, 0, 1, 1, 0, 3, 2};
static const byte CompareQuadValueB[8] = {3, 2, 1, 0, 3, 2, 1, 0};
void QuadScroll(direction dir)
{
  byte quadA; //Entering quad
  byte quadB; //Diagonal quad
  byte indexA;
  byte indexB;
  byte relH;
  byte relV;
  sbyte vA = 0;
  sbyte hA = 0;
  sbyte vB = 0;
  sbyte hB = 0;
  bool charPosX = (characters.posX[followIndex] % 16) < quadWidth;
  bool charPosY = (characters.posY[followIndex] % 16) < quadHeight;
  byte compareQuad = GetPlayerQuad();
  
  byte p = GetChar(COLS - 1, ROWS - 1);
  SetChar('Q', COLS - 1, ROWS - 1);
  
  
  /*switch(dir) // REPLACE THIS WITH A MATRIX
  {
    case 0:
      vA = -1;
      vB = -1;
      if (charPosX) //Left
        hB = -1;
      else
        hB = 1; //Right
      break;
    case 1:
      vA = 1;
      vB = 1;
      if (charPosX)
        hB = -1;
      else
        hB = 1;
      break;
    case 2:
      hA = -1;
      hB = -1;
      if (charPosY)
        vB = -1;
      else
        vB = 1;
      break;
    case 3:
      hA = 1;
      hB = 1;
      if (charPosY)
        vB = -1;
      else
        vB = 1;
      break;
  }*/
  
  relH = dir;
  relV = dir;
  if (!charPosX)
    relH += 4;
  if (!charPosY)
    relV += 4;
  vA = quadRelationAV[relV];
  vB = quadRelationBV[relV];
  hA = quadRelationAH[relH];
  hB = quadRelationBH[relH];
  
  indexA = GetQuadInRelation(vA, hA);
  indexB = GetQuadInRelation(vB, hB);
  
  /*if (dir < 2) // Vertical
    switch (compareQuad) //THESE TWO SWITCHES CAN BE REPLACED WITH A PRECALCULATED MATRIX
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
  else //Horizontal
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
    }*/
  //byte CompareQuadValueA[8] = {2, 3, 0, 1, 1, 0, 3, 2};
  //byte CompareQuadValueB[8] = {3, 2, 1, 0, 3, 2, 1, 0};
  if (dir > 1)
    compareQuad += 4;
  quadA = CompareQuadValueA[compareQuad];
  quadB = CompareQuadValueB[compareQuad];
  
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
  byte byte_x;
  byte byte_y;
  byte byte_i;
  byte byte_index;
  byte byte_offset;
  
  for (byte_y = 0; byte_y < 8; ++byte_y)
    for (byte_x = 0; byte_x < 8; ++byte_x)
    {
      byte_index = byte_x + byte_y*8;
      byte_offset = byte_x * 2 + 32*byte_y;

      tiles.blocked[byte_index] = 0;
      ScreenQuad.CharIndex[byte_index][0] = byte_offset; // Init screen quad prefabs for 8x8
      ScreenQuad.CharIndex[byte_index][1] = byte_offset + 1;
      ScreenQuad.CharIndex[byte_index][2] = byte_offset + 16;
      ScreenQuad.CharIndex[byte_index][3] = byte_offset + 17;
      ScreenQuad.Chars[byte_index][0] = 32;
      ScreenQuad.Chars[byte_index][1] = byte_index;
      ScreenQuad.ScatterIndex[byte_index] = 0;
    }
  tiles.opaque[44] = true; //Trees
  ScreenQuad.Chars[2][0] = 36; // Set the wizard to grass on 0
  ScreenQuad.Chars[2][1] = 44; // Set the wizard to trees on 1
  //Init Characters
  for (byte_i = 0; byte_i < charactersCount; ++byte_i)
  {
    characters.tile[byte_i] = byte_i;
    characters.posX[byte_i] = byte_i;
    characters.posY[byte_i] = byte_i;
    characters.quadPosX[byte_i] = byte_i;
    characters.quadPosX[byte_i] = byte_i;
    characters.visible[byte_i] = false;
    characters.collide[byte_i] = false;
  }
  characters.visible[0] = true;
  characters.posX[0]  = 8;
  characters.posY[0]  = 8;
  characters.quadPosX[0]  = 2;
  characters.quadPosY[0]  = 0;
  characters.tile[0] = 2;

  characters.visible[1] = true;
  characters.collide[1] = true;
  characters.message[1] = 0;

  characters.tile[2] = signpost;
  characters.visible[2] = true;
  characters.collide[2] = true;
  characters.message[2] = 1;
  characters.posX[2] = 8;
  characters.posY[2] = 6;
  characters.quadPosX[2]  = 2;
  characters.quadPosY[2]  = 0;

  LoadMapQuads();
}

void wrapX(sbyte *posX) //Used in map positions
{
  if (*posX < 0)
    *posX = mapWidth - 1;
  if (*posX >= mapWidth)
    *posX = 0;
}

void wrapY(sbyte *posY)
{
  if (*posY < 0)
    *posY = mapHeight - 1;
  if (*posY >= mapHeight)
    *posY = 0;
}

bool CheckCollision(byte charIndex, direction dir)
{
  byte byte_i;
  sbyte xPos = characters.posX[charIndex];
  sbyte yPos = characters.posY[charIndex]; //These need to be signed because they can wrap around the map

  //Check the tile we're already standing on
  if(ReadBit(tiles.blocked[mapData[xPos][yPos]], dir))
  {
    //WriteLineMessageWindow("Standing on blocked@", 0);
    return true;
  }

  switch (dir)
  {
    case up:
      --yPos;
      wrapY(&yPos);
      break;
    case down:
      ++yPos;
      wrapY(&yPos);
      break;
    case left:
      --xPos;
      wrapX(&xPos);
      break;
    case right:
      ++xPos;
      wrapX(&xPos);
      break;
    default:
      return false;
  }

  if(ReadBit(tiles.blocked[mapData[xPos][yPos]], dir))
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
    if(characters.collide[byte_i])
      if (characters.posX[byte_i] == xPos)
        if (characters.posY[byte_i] == yPos)
        {
          WriteLineMessageWindow(Messages[characters.message[byte_i]], 1);
          return true;
        }

  return false;
}

void DrawSquare(sbyte xOrigin, sbyte yOrigin, sbyte xSize, sbyte ySize) //LOS Blocking
{
  byte x;
  byte y;
  
  if (xOrigin + xSize > viewportWidth)
    --xSize;
  if (yOrigin + ySize > viewportHeight)
    --ySize;
  for(y = 0; y < ySize; ++y)
  {
    for(x = 0; x < xSize; ++x)
      viewportBuffer[x + xOrigin][y + yOrigin] = EmptyTile;
  }
}

void ApplyLOS()
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

  byte x;
  byte y;
  //Quad 0
  for(y = playerY - 1; y > 0; --y)
    for(x = playerX - 1; x > 0; --x)
      if (tiles.opaque[viewportBuffer[x][y]])
      {
        //DrawSquare(0, y, x, 1);
        DrawSquare(0, 0, x, y);
      }
  //Quad 1
  for(y = playerY - 1; y > 0; --y)
    for(x = playerX + 1; x < viewportWidth; ++x)
      if (tiles.opaque[viewportBuffer[x][y]])
      {
        //DrawSquare(x + 1, y, viewportWidth - x, 1);
        DrawSquare(x, 0, viewportWidth - x, y);
      }
  //Quad 2
  for(y = playerY + 1; y < viewportHeight; ++y)
    for(x = playerX + 1; x < viewportWidth; ++x)
      if (tiles.opaque[viewportBuffer[x][y]])
      {
        //DrawSquare(x + 1, y, viewportWidth - x, 1);
        DrawSquare(x + 1, y + 1, viewportWidth - x, viewportHeight - y);
      }
  //Quad 3
  for(y = playerY + 1; y < viewportHeight; ++y)
    for(x = playerX - 1; x > 0; --x)
      if (tiles.opaque[viewportBuffer[x][y]])
      {
        //DrawSquare(0, y, x, 1);
        DrawSquare(0, y + 1, x, viewportHeight - y);
      }
  //Horizontal
  for(x = playerX - 1; x > 0; --x)
    for(y = playerY - 1; y <= playerY + 1; ++y)
      if (tiles.opaque[viewportBuffer[x][y]])
        DrawSquare(0, y, x, 1);
  for(x = playerX + 1; x < viewportWidth; ++x)
    for(y = playerY - 1; y <= playerY + 1; ++y)
      if (tiles.opaque[viewportBuffer[x][y]])
        DrawSquare(x + 1, y, viewportWidth - x - 1, 1);
  //Vertical
  for(y = playerY - 1; y > 0; --y)
    for(x = playerX -1 ; x <= playerX + 1; ++x)
      if (tiles.opaque[viewportBuffer[x][y]])
        DrawSquare(x, 0, 1, y);
  for(y = playerY + 1; y < viewportHeight; ++y)
    for(x = playerX -1 ; x <= playerX + 1; ++x)
      if (tiles.opaque[viewportBuffer[x][y]])
        DrawSquare(x, y + 1, 1, viewportHeight - y);
}

void DrawEntireMap()
{
  sbyte int_a;
  sbyte int_b;
  byte byte_x;
  byte byte_y;
  #if defined(__C64__)
  StoreBuffer();
  #endif
  //Buffer the matrix of tiles for our viewport
  CameraFollow();
  int_a = offsetX;
  int_b = offsetY;
  for(byte_y = 0; byte_y < viewportHeight; ++byte_y)
  {
    wrapY(&int_b); //Wrap the map data y reference
    for(byte_x = 0; byte_x < viewportWidth; ++byte_x)
    {
      wrapX(&int_a); //Wrap the map data X reference
      viewportBuffer[byte_x][byte_y] = mapData[int_a][int_b];
      int_a++;
    }
    int_a = offsetX;
    ++int_b;
  }
  BufferCharacters();
  if(LOSEnabled)
    ApplyLOS();

  for(byte_y = 0; byte_y < viewportHeight; ++byte_y)
  {      
    for(byte_x = 0; byte_x < viewportWidth; ++byte_x)
    { //Only draw tiles that are different from the last draw; minimal effect on smaller screen sizes
      byte lastIndex = viewportBufferLast[byte_x][byte_y];
      byte newIndex = viewportBuffer[byte_x][byte_y];
      if (lastIndex!=newIndex)
      {
        DrawTileIndex = newIndex;
        DrawTileX = byte_x;
        DrawTileY = byte_y;
        DrawTileBuffer();
      }
    }
  }
  #if defined(__C64__)
  SwapBuffer();
  #endif
  memcpy(&viewportBufferLast[0][0], &viewportBuffer[0][0], viewportSize);
  DrawCharacterCoordinates(followIndex);
}

void MoveCharacter(byte index, byte dir)
{
  bool scrollQuads = false;
  byte checkCollision = CheckCollision(index, dir);

  TickMoonPhase();
  if(!checkCollision)
  {
    switch (dir)
    {
      case up:
        --characters.posY[index];
        if (characters.posY[index] < 0)
          characters.posY[index] = mapHeight - 1;
        if (characters.posY[index] == 15 || characters.posY[index] == 31)
        {
          --characters.quadPosY[index];
          if(characters.quadPosY[index] < 0)
            characters.quadPosY[index] = mapMatrixHeight - 1;
        }
        break;
      case down:
        ++characters.posY[index];
        if (characters.posY[index] >= mapHeight)
          characters.posY[index] = 0;
        if (characters.posY[index] == 0 || characters.posY[index] == 16)
        {
          ++characters.quadPosY[index];
          if(characters.quadPosY[index] == mapMatrixHeight)
            characters.quadPosY[index] = 0; 
        }
        break;
      case left:
        --characters.posX[index];
        if (characters.posX[index] < 0)
          characters.posX[index] = mapWidth - 1;
        if (characters.posX[index] == 15 || characters.posX[index] == 31)
        {
          --characters.quadPosX[index];
          if(characters.quadPosX[index] < 0)
            characters.quadPosX[index] = mapMatrixWidth - 1; 
        }
        break;
      case right:
        ++characters.posX[index];
        if (characters.posX[index] >= mapWidth)
          characters.posX[index] = 0;
        if (characters.posX[index] == 0 || characters.posX[index] == 16)
        {
          ++characters.quadPosX[index];
          if(characters.quadPosX[index] == mapMatrixWidth)
            characters.quadPosX[index] = 0; 
        }
        break;
      default:
        break;
    }
    
    if (index == followIndex)
    {
      byte edgeCheckX = characters.posX[index] % 16;
      byte edgeCheckY = characters.posY[index] % 16;

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
      if (scrollQuads)
        QuadScroll(dir);
    }
    DrawEntireMap();
  }
}

void DrawCharacterCoordinates(byte index)
{
  sprintf(strTemp,"(%2d,%2d)@", characters.posX[index], characters.posY[index]);
  PrintString(strTemp, viewportPosX + 6, viewportPosY - 1, true, false);
}

void LoadMap()
{
  InitializeMapData();
}

screenName MapUpdate()
{
  bool exit = false;
  ClearScreen();
  DrawBorder("Map@", viewportPosX - 1, viewportPosY - 1, viewportWidth* 2 + 2, viewportHeight * 2 + 2, true);
  ResizeMessageWindow(consolePosX, consolePosY, consoleWidth, consoleHeight);
  DrawCharStats();

  //Initialize Viewport
  memset(&viewportBuffer, EmptyTile, viewportSize);
  memset(&viewportBufferLast, EmptyTile, viewportSize);
  SetTileOrigin(viewportPosX, viewportPosY);
  DrawEntireMap();

  while (!exit)
  {
    UpdateInput();
    //if (InputChanged())
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
        MoveCharacter(followIndex, Dir);        
    }
  }
  return EditParty;
}