#include "Xinaga.h"

//Charset Desciption (16x32 blocks)
//Map Characters 		Row 0
//Punctuation/Numbers		Row 1
//Uppercase Alphabet		Row 2 (NES/MSX/Apple II), Row 7 (C64)
//Lowercase Alphabet		Row 3 (NES/MSX/Apple II), Row 2 (C64)
//Map Tiles			Row 4,5,6 (NES/MSX/Apple II), Row 3, 4, 5 (C64)
//Interface/Minimap		Row 7


#if defined(__APPLE2__)
//#pragma code-name (push, "LOWCODE")
#endif

#if defined (__NES__)
#pragma code-name (push, "XINAGA_GRAPHICS")
#pragma rodata-name (push, "XINAGA_GRAPHICS")
//#pragma data-name (push, "XRAM")
//#pragma bss-name (push, "XRAM")
#include "neslib.h"
// VRAM buffer module
#include "vrambuf.h"
//#link "vrambuf.c"

#endif

#if defined (MSX)
#include "msxbios.h"
#endif

#if defined(__ATARI__)
#include <conio.h>
#include <atari.h>
#endif

#if defined(__C64__)
#include <conio.h>
#endif


int YColumnIndex[ROWS];

void getYCols()
{
  byte y;
  for (y = 0; y < ROWS; ++y)
    YColumnIndex[y] = y * COLS;
}

const int ScreenCharSize = ROWS*COLS;


#if defined(__APPLE2__)
byte* ScreenChars = (byte*)(0x0400);
#pragma data-name (push, "HGR")
char HGR[0x2000] = {};
#pragma data-name (pop)
//byte* HGR = (byte*)0x2000;
byte* HGRBuffer = (byte*)0x4000;
unsigned int RowsHGR[192];
//int* RowsHGR = (int*)0xD400;
#endif

#define fadeFrames 5
#define mapFadeFrames 2


void FadePalette(byte pals, byte delay)
{
  #if defined(__NES__)
  //byte *tempPal = (byte*)malloc(16);
  byte tempPal[16];
  byte y, z;

  memcpy(&tempPal[0], &PALETTE_0[0], 16);
  for (y = 3; y > 0; --y)
  {
    for (z = 0; z < 4; ++z)
    {
      if (pals & (1 << z))
        tempPal[y + (z*4)] = 0x0f;
    }

    pal_bg(tempPal);
    wait_vblank(delay);
  }
  //free(tempPal);
  #endif
  pals;delay;
}

void UnFadePalette(byte pals, byte delay)
{
  #if defined(__NES__)
  byte tempPal[16];
  //byte *tempPal = (byte*)malloc(16);
  byte y, z;
  //memcpy(&tempPal[0], &PALETTE_0[0], 16);

  for (z = 0; z < 4; ++z)
  {
    if (pals & (1 << z))
    {
      memset(&tempPal[(z*4)], 0x0f, 3);
    }
  }

  for (y = 0; y < 4; ++y)
  {
    for (z = 0; z < 4; ++z)
    {
      if (pals & (1 << z))
      {
        tempPal[y + (z*4)] = PALETTE_0[y+(z*4)];
      }
    }

    pal_bg(tempPal);
    wait_vblank(delay);
  }
  //free(tempPal);
  #endif
  pals;delay;
}

bool screenFaded;

void MapFadeOut()
{
  FadePalette(0b1110, mapFadeFrames);
}
void MapFadeIn()
{
  UnFadePalette(0b1110, mapFadeFrames);
}
void ScreenFadeOut(void)
{
  #if defined (__NES__)
  if (!screenFaded)
  {
    FadePalette(0b1111, fadeFrames);
    //pal_bg(PALETTE_1);
    //wait_vblank(fadeFrames);
    //pal_bg(PALETTE_2);
    //wait_vblank(fadeFrames);
    //pal_bg(PALETTE_3);
  }
  #endif
  screenFaded = true;
}
void ScreenFadeIn(void)
{
  #if defined (__NES__)
  if (screenFaded)
  {
    UnFadePalette(0b1111, fadeFrames);

    //pal_bg(PALETTE_2);
    //wait_vblank(fadeFrames);
    //pal_bg(PALETTE_1);
    //wait_vblank(fadeFrames);
    //pal_bg(PALETTE_0);
  }
  #endif
  screenFaded = false;
}

const byte MOD_4[32] = { //Lookup tables for %4
  0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3,
  0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3
  };

void SetAttrib(byte x, byte y, byte pal, bool direct)
{
  #if defined (__NES__)
  byte offset = (y / 4) * 8 + (x / 4); //Which byte of the attribute table?
  byte pairX = (MOD_4[x]) > 1 ? 2 : 0;
  byte pairY = (MOD_4[y]) > 1 ? 4 : 0;

  byte shift = pairX + pairY;
  byte mask = ~(0b11 << shift);

  ATTRIBUTE_TABLE[offset] = (ATTRIBUTE_TABLE[offset] & mask) | (pal << shift);
  if (direct)
  {
    vrambuf_put(NTADR_A(0,0)+(ScreenCharSize)+offset, &ATTRIBUTE_TABLE[offset], 1);
    vrambuf_flush();
  }
  #endif
  x;y;pal;direct;
}

void UpdateAttributes(void)
{
  #if defined (__NES__)
  vrambuf_put(NTADR_A(0,0)+(ScreenCharSize), &ATTRIBUTE_TABLE[0], 64);
  wait_vblank(1);
  #endif
}


#if defined (__NES__)
//const byte const *charset = 0x0;
//const byte const *attributeset = 0x0;
byte ScreenChars[ROWS*COLS];
byte attributeset[256];


#endif

#if defined (__ATARI__)
byte ScreenChars[ROWS*COLS];
#endif

#if defined (MSX)
byte ScreenChars[ROWS*COLS];
#endif

#if defined(__C64__)
const byte const charset[2048];
byte *ScreenCharBuffer = (byte *)0x0400;
byte *ScreenColorBuffer = (byte *)0xF400;
byte *ScreenChars = (byte *)0x0400;
byte *ScreenColors = (byte *)0xD800;
bool bufferselect = false;
//byte attributeset[256];
#endif

#if defined(__C64__)
byte *bgReg = (byte*)0xD020;
void SetBorder(byte color)
{
  bgReg[0] = color;
}
void SetBG(byte color)
{
  bgReg[1] = color;
}
#endif

byte ReadBit(byte byteToRead, char bit)
{
  bit = 1 << bit;
  return(bit & byteToRead);
}

void ClearScreen(void)
{

  #if defined(__C64__)
  memset(ScreenChars, ' ', ScreenCharSize); // Clear Chars
  memset(ScreenCharBuffer, ' ', ScreenCharSize); // Clear Buffer
  memset(ScreenColors, 1, ScreenCharSize); // clear Colors
  memset(ScreenColorBuffer, 1, ScreenCharSize); // clear Color Buffer
  #endif

  #if defined(__APPLE2__)
  byte i;
  for (i = 0; i < 192; ++i)
    memset(&HGR[RowsHGR[i]], 0, 40); //Or just disable screen and clear it?
  memset(ScreenChars, ' ', 0x0400); // Clear Chars (text page 1 on Apple II)
  //memset(HGR, 0, 0x2000); // clear HGR page 1
  #endif

  #if defined (__NES__)
  ppu_off();
  vrambuf_flush();
  memset(ScreenChars, ' ', ScreenCharSize); // Clear Chars
  vram_adr(NTADR_A(0, 0));
  vram_write(&ScreenChars[0], ScreenCharSize);
  memset(&ATTRIBUTE_TABLE[0], 0, 64);
  vram_write(&ATTRIBUTE_TABLE[0], 64);
  ppu_on_all();
  #endif

  #if defined (MSX)
  FILVRM(0x1800, 768, ' ');
  #endif
  
  #if defined (__ATARI__)
  clrscr();
  #endif
}

#if defined(__C64__)
byte* RASTERCOUNT = (byte*)0xD012;
#endif
void raster_wait(byte line)
{
  #if defined(__C64__)
  while ((RASTERCOUNT[0] < line)){}
  #endif

  #if defined(__APPLE2__)
  int x = 0;
  while (x < 4096 * line)
    ++x;
  #endif

  #if defined(__NES__)
  line;
  #endif

  #if defined(MSX)
  __asm__("HALT");
  line;
  #endif

  #if defined(__ATARI__)
  line;
  #endif
}

void wait_vblank(byte frames) 
{
  byte count = frames;
  for (count = frames; count; --count)
  {
    //UpdateInput();

    #if defined (__C64__)
    raster_wait(255);    
    #endif

    #if defined (__APPLE2__)
    int x = 0;
    while (x < 512)
      ++x;
    #endif

    #if defined (__NES__)
    vrambuf_flush();
    #endif
  }
}

#if defined(__C64__)
void SelectScreenPos(byte pos)
{
  int* regd018 = (int*)0xD018;
  byte a = pos * 16;
  raster_wait(255);
  regd018[0] = (regd018[0] & 15) | a;
  //POKE (0xD018,(PEEK(0xD018) & 15) | a);
}

#endif

void InitializeGraphics(void)
{
  #if defined(__APPLE2__)
  byte y = 0;
  getYCols();
  ClearScreen();  
  STROBE(0xc052); // turn off mixed-mode
  STROBE(0xc054); // page 1
  STROBE(0xc057); // hi-res
  STROBE(0xc050); // set graphics mode
  for (y = 0; y < 192; ++y)
    RowsHGR[y] = (y/64)*0x28 + (y%8)*0x400 + ((y/8)&7)*0x80;
  #endif

  #if defined(__C64__)
  #define bank 3
  #define CharacterRom 0xD000
  #define ColorRam 0xD800
  byte charpos = 7;
  byte screenpos = 2;
  byte vicreg = 0x00;
  int screenposition;
  int* regd018 = (int*)0xD018;
  int* regdd00 = (int*)0xDD00;

  byte* charfile = &characterset[0];//(int*)0x0840;
  //int* attribfile = (int*)0x1040;

  byte* CharRam;  

  getYCols();
  CharRam = 0;

  if (bufferselect)
    ++screenpos;

  SetBG(ColorBG);
  SetBorder(ColorBorder);

  screenposition = (bank * (16<<10) + (screenpos <<10)); // *1024
  ScreenChars = 0;
  ScreenChars += screenposition;
  //CharRam += 2;
  CharRam += (bank * (16<<10) + (charpos <<11)); // *1024, *2048

  //memcpy(&CharRam[0], &charset[0], 2048); // * 8
  memcpy(&CharRam[0], &charfile[0], 2048);
  //memcpy(&attributeset[0], &attribfile[0], 256);


  ScreenCharBuffer = 0;
  ScreenCharBuffer += screenposition;

  if (bufferselect)
    ScreenCharBuffer -= 0x0400; // Buffer location 1024b before the screen position
  else
    ScreenCharBuffer += 0x0400; // Buffer location 1024b after the screen position

  ScreenColorBuffer = 0;
  ScreenColorBuffer += 0x0400; // Use the default screen character space for color buffer
  //Select Bank
  //POKE (0xDD00, (PEEK(0XDD00)&(255 - bank)));
  regdd00[0] = (regdd00[0]&(255 - bank));
  //Set Screen and Character Ram Position
  screenpos = screenpos << 4;
  charpos = charpos << 1;
  vicreg = screenpos + charpos;
  regd018[0] = vicreg;
  //Cursor Position
  //POKE (0x0288, screenposition / 256);
  ClearScreen();
  //SetMulticolors(11, 15);
  #endif

  #if defined(__NES__)
  getYCols();  
  vrambuf_clear();
  set_vram_update(updbuf);  
  pal_bright(4);
  pal_clear();
  pal_bg(PALETTE_0);
  oam_clear();
  ClearScreen();  
  // enable PPU rendering (turn on screen)
  //ppu_on_all();
  #endif
  
  #if defined(__ATARI__)
  getYCols();  
  ClearScreen();
  #endif
}

void SwapBuffer(void)
{
  #if defined(__C64__)
  if (bufferselect)
  {
    bufferselect = false;
    SelectScreenPos(2);
    ScreenChars -= 0x400;
    ScreenCharBuffer += 0x400;
  }
  else
  {
    bufferselect = true;
    SelectScreenPos(3);
    ScreenChars += 0x400;
    ScreenCharBuffer -= 0x400;    
  }
  memcpy(&ScreenColors[0], &ScreenColorBuffer[0], 0x400);
  #endif
}

#if defined(__APPLE2__)
void DrawChar(int index, byte xpos, byte ypos)
{
  byte y;
  int offset;
  int i;

  i = index << 3;
  ypos = ypos << 3;
  offset= RowsHGR[ypos] + xpos;
  for (y = 0; y < 8; ++y)
  {
    HGR[offset] = charset[i];
    offset += 0x400;
    ++i;
  }
}
#endif

byte SetCharIndex;
byte SetCharX;
byte SetCharY;
int charsDrawn;
//Set Char Macro is in XINAGA.h - #define SetChar(index, x, y) do {SetCharIndex = (index); SetCharX = (x); SetCharY = (y); _SetChar();}while(0)
void _SetChar(void)
{
  int offset = SetCharX + YColumnIndex[SetCharY];
  #if defined(__APPLE2__)
  bool draw = false;
  if (ScreenChars[offset] != SetCharIndex)
    draw = true;
  ScreenChars[offset] = SetCharIndex;
  if (draw)
    DrawChar(SetCharIndex, SetCharX, SetCharY);
  #endif
  #if defined(__C64__)
  ScreenChars[offset] = SetCharIndex;
  //gotoxy(SetCharX,SetCharY);
  //cputc(SetCharIndex);
  
  ScreenColors[offset] = attributeset[SetCharIndex];
  #endif

  #if defined (__NES__)
  ScreenChars[offset] = SetCharIndex;
  //vram_adr(NTADR_A(SetCharX,SetCharY));		// set address
  vrambuf_put(NTADR_A(SetCharX,SetCharY), &SetCharIndex, 1);
  //vrambuf_put(NAMETABLE_A + 32*30 + (SetCharX / 2) + ((SetCharY / 2) * (COLS / 2)), &attributeset[SetCharIndex], 1);



  //vrambuf_end();
  // wait for next frame to flush update buffer
  // this will also set the scroll registers properly
  //ppu_wait_frame();
  // clear the buffer
  ++charsDrawn;
  if (charsDrawn % 21 == 0) // if (charsDrawn % 10 == 0)
  {
    wait_vblank(1);
    //vrambuf_flush();
  }
  //vram_put(SetCharIndex);
  //ppu_on_all();
  #endif

  #if defined(MSX)
  //POSIT(SetCharY+1+(SetCharX<<8));
  //CHPUT((int)SetCharIndex);
  SETWRT();
  WRTVRM(0x1800 + SetCharX +(SetCharY*32), SetCharIndex);
  #endif
  
  #if defined(__ATARI__)
  gotoxy(SetCharX,SetCharY);
  cputc(SetCharIndex);
  #endif
}


void SetColor(byte index, byte x, byte y)
{
  #if defined(__APPLE2__)
  index;x;y;
  #endif
  #if defined(__C64__)
  ScreenColors[x + YColumnIndex[y]] = index;
  #endif
  #if defined(__NES__)
  //ScreenColors[(x / 2) + ((y / 2) * 8)] = index;
  index;
  x;
  y;
  #endif
  #if defined(MSX)
  FORCLR = index;
  SetChar(x, y, index);
  #endif

  #if defined(__ATARI__)
  index;x;y;
  #endif
}

byte GetChar(byte x, byte y)
{
  return ScreenChars[x + YColumnIndex[y]];
}

void PrintString(char *text, byte posx, byte posy, bool fast)
{
  byte i;
  while (posy > ROWS - 1)
    posy -= ROWS;
  for(i = 0; i < COLS; ++i)
  {
    if (text[i] == '@' ||  text[i] == '\n')
      break;
    if (!fast)
      wait_vblank(1);
    //if (buffer)
      //SetCharBuffer(text[i], posx + i, posy);
    else
      SetChar(text[i], posx + i, posy);
    #if defined(__C64__)
    SetColor(attributeset[text[i]], posx + i, posy);
    #endif
  }
  wait_vblank(1);
}

int originOffset;
byte MapOriginX;
byte MapOriginY;

void SetTileOrigin(byte x, byte y)
{
  originOffset = YColumnIndex[y] + x;
  MapOriginX = x;
  MapOriginY = y;
}

byte DrawTileX;
byte DrawTileY;
byte DrawTileIndex;
byte DrawTilePalette;
byte indexes[4];
unsigned short offset1;
#if defined(__C64__)
byte *destinationChar;
byte *destinationColor;
#endif

void DrawTileSetup(void)
{

  DrawTileIndex = (DrawTileIndex << 1) + ((DrawTileIndex >> 3) << 4);
  indexes[0] = DrawTileIndex;
  indexes[1] = DrawTileIndex + 1;
  indexes[2] = DrawTileIndex + 16;
  indexes[3] = DrawTileIndex + 17;

  DrawTileX = DrawTileX << 1;
  DrawTileY = DrawTileY << 1;

  SetAttrib(DrawTileX + viewportPosX, DrawTileY+ viewportPosY, DrawTilePalette, false);  

  #if defined(__C64__)
  offset1 = YColumnIndex[DrawTileY] + DrawTileX + originOffset;
  #endif
}

void DrawTile()
{
  byte x = DrawTileX + MapOriginX;
  byte y = DrawTileY + MapOriginY;

  #if defined(__C64__)
  SetChar(indexes[0], x, y);
  SetChar(indexes[1], x + 1, y);
  SetChar(indexes[2], x, y + 1);
  SetChar(indexes[3], x + 1, y + 1);
  
  /*destinationChar[0] = indexes[0];
  destinationChar[1] = indexes[1];
  destinationColor[0] = attributeset[indexes[0]];
  destinationColor[1] = attributeset[indexes[1]];
  destinationChar += COLS;
  destinationColor += COLS;
  destinationChar[0] = indexes[2];
  destinationChar[1] = indexes[3];
  destinationColor[0] = attributeset[indexes[2]];
  destinationColor[1] = attributeset[indexes[3]];*/
  #endif

  #if defined(__APPLE2__)
  SetChar(indexes[0], x, y);
  SetChar(indexes[1], x + 1, y);
  SetChar(indexes[2], x, y + 1);
  SetChar(indexes[3], x + 1, y + 1);
  #endif

  #if defined(__NES__) 
  SetChar(indexes[0], x, y);
  SetChar(indexes[1], x + 1, y);
  SetChar(indexes[2], x, y + 1);
  SetChar(indexes[3], x + 1, y + 1);
  #endif

  #if defined(MSX)
  SetChar(indexes[0], x, y);
  SetChar(indexes[1], x + 1, y);
  SetChar(indexes[2], x, y + 1);
  SetChar(indexes[3], x + 1, y + 1);
  #endif
}
void DrawTileBuffer(bool drawChars)
{
  DrawTileSetup();

  #if defined(__C64__)
  destinationChar = &ScreenCharBuffer[offset1];
  destinationColor = &ScreenColorBuffer[offset1];
  #endif
  if (drawChars)
    DrawTile();
}
void DrawTileDirect(void)
{
  DrawTileSetup();

  #if defined(__C64__)
  destinationChar = &ScreenChars[offset1];
  destinationColor = &ScreenColors[offset1];
  #endif

  DrawTile();
  /*SetChar(indexes[0], DrawTileX + MapOriginX, DrawTileY + MapOriginY);
  SetChar(indexes[1], DrawTileX + MapOriginX + 1, DrawTileY + MapOriginY);
  SetChar(indexes[2], DrawTileX + MapOriginX, DrawTileY + 1 + MapOriginY);
  SetChar(indexes[3], DrawTileX + MapOriginX + 1, DrawTileY + 1 + MapOriginY);*/
}

void DrawTileDirectXY(byte index, byte x, byte y)
{
  byte tempX = MapOriginX;
  byte tempY = MapOriginY;
  SetAttrib(DrawTileX + viewportPosX, DrawTileY+ viewportPosY, DrawTilePalette, false);  

  SetTileOrigin(x, y);
  DrawTileIndex = index;
  DrawTileX = 0;
  DrawTileY = 0;
  DrawTileDirect();

  SetTileOrigin(tempX, tempY);
}

//Tile Data
#define TileCount 64
#define TileSize 16
byte tilesBlocked[TileCount];
byte tilesOpaque[TileCount];
byte tilesPalette[TileCount];

void FillViewport(byte index, byte width, byte height)
{
  byte byte_x, byte_y;
  for(byte_y = 0; byte_y < height; ++byte_y)
    for(byte_x = 0; byte_x < width; ++byte_x)
    {
      DrawTileX = byte_x;
      DrawTileY = byte_y;
      DrawTileIndex = index;
      DrawTilePalette = tilesPalette[index];
      DrawTileBuffer(false);
    }
  UpdateAttributes();
}

byte arrowA;
byte arrowB;
byte arrowX;
byte arrowY;
void ReadyArrow(byte x, byte y)
{
  x = x << 1;
  y = y << 1;

  arrowX = x + MapOriginX;
  arrowY = y + MapOriginY + 2;
  while (arrowX > COLS)
    --arrowX;
  arrowA = GetChar(arrowX, arrowY);
  arrowB = GetChar(arrowX + 1, arrowY);
}
void DrawArrow(byte x, byte y)
{
  ReadyArrow(x, y);

  SetChar('^', arrowX, arrowY);
  SetChar('^', arrowX + 1, arrowY);
  //wait_vblank(1);
}
void ClearArrow(void)
{
  SetChar(arrowA, arrowX, arrowY);
  SetChar(arrowB, arrowX + 1, arrowY);
  //wait_vblank(1);
}

void DrawLineH(byte index, byte x, byte y, byte length)
{
  byte count;
  byte posX;
  byte tempIndex;
  for (count = 0; count < length; ++count)
  {
    tempIndex = index;
    posX = x + count;
    if (posX % 2 == 1 && index != ' ')
      ++tempIndex;
    SetChar(tempIndex, posX, y);
  }
  //wait_vblank(1);
}
void DrawLineV(byte index, byte x, byte y, byte length)
{
  byte count;
  byte tempIndex = index;
  if (x % 2 == 1)
    ++tempIndex;
  for (count = 0; count < length; ++count)
  {
    SetChar(tempIndex, x, y + count);
  }
  //wait_vblank(1);
}

void DrawCorners(byte xPos, byte yPos, byte widthInside1, byte heightInside1)
{
  char corner = 0xEA;
  char corner1 = corner;
  char corner2 = corner;
  if (xPos % 2 == 1)
    ++corner1;
  if ((xPos + widthInside1) % 2 == 1)
    ++corner2;
  SetChar(corner1, xPos, yPos);
  SetChar(corner2, xPos + widthInside1, yPos);
  SetChar(corner1, xPos, yPos + heightInside1);
  SetChar(corner2, xPos + widthInside1, yPos + heightInside1);
  //wait_vblank(1);
}
void DrawBorder(char *text, byte xPos, byte yPos, byte width, byte height, bool fill)
{
  byte x;
  //byte offset = 0;
  byte widthInside2 = width - 2;
  byte widthInside1 = width - 1;
  byte heightInside1 = height - 1;
  byte heightInside2 = height - 2;

  byte yPos1 = yPos + 1;
  byte xPos1 = xPos + 1;


  //for (i = 0; text[i] != '@' && text[i] != '\n'; ++i)
  //++offset;

  if (fill)
  {
    for (x = 0; x < heightInside2; ++x)
    {
      DrawLineH(' ', xPos1, yPos1 + x, widthInside2);
    }
    //wait_vblank(1);
  }


  DrawLineH(0xEC, xPos1, yPos, widthInside2);
  DrawLineH(0xFC, xPos1, yPos + heightInside1, widthInside2);
  DrawLineV(0xEE, xPos, yPos1, heightInside1);
  DrawLineV(0xFE, xPos + widthInside1, yPos1, heightInside2);
  DrawCorners(xPos, yPos, widthInside1, heightInside1);
  PrintString(text, xPos1, yPos, true);

  wait_vblank(1);
}

#if defined (__NES__)
unsigned  char attributeset[256] = {
  0x01, 0x01, 0x0A, 0x0A, 0x0C, 0x0C, 0x01, 0x01, 0x01, 0x01, 0x0C, 0x0C,
  0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x0A, 0x0A, 0x0C, 0x0C, 0x01, 0x01,
  0x01, 0x01, 0x0B, 0x0B, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
  0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
  0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
  0x0E, 0x01, 0x0E, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
  0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
  0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
  0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
  0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
  0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x04, 0x06,
  0x01, 0x01, 0x08, 0x08, 0x05, 0x0D, 0x08, 0x08, 0x02, 0x08, 0x08, 0x0F,
  0x01, 0x01, 0x06, 0x04, 0x01, 0x01, 0x09, 0x09, 0x0D, 0x05, 0x09, 0x09,
  0x08, 0x02, 0x09, 0x09, 0x08, 0x08, 0x0B, 0x0B, 0x0C, 0x0C, 0x0C, 0x0C,
  0x0D, 0x0D, 0x0F, 0x0B, 0x0B, 0x0B, 0x0C, 0x0C, 0x08, 0x08, 0x0A, 0x0B,
  0x0B, 0x0B, 0x0B, 0x0B, 0x05, 0x05, 0x0C, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B,
  0x0C, 0x0C, 0x09, 0x09, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
  0x01, 0x01, 0x01, 0x01, 0x0B, 0x0B, 0x0A, 0x0A, 0x06, 0x06, 0x06, 0x06,
  0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
  0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
  0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
  0x06, 0x06, 0x06, 0x06
  };
#endif
