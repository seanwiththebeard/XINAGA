#include "Xinaga.h"

#if defined(__APPLE2__)
//#pragma code-name (push, "LOWCODE")
#endif

#if defined (__NES__)
#pragma code-name (push, "STARTUP")
//#pragma rodata-name (push, "STARTUP")
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

int YColumnIndex[ROWS];

void getYCols()
{
  byte y;
  for (y = 0; y < ROWS; ++y)
    YColumnIndex[y] = y * COLS;
}


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

#if defined (__NES__)
//const byte const *charset = 0x0;
//const byte const *attributeset = 0x0;
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
byte attributeset[256];
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
  memset(ScreenChars, ' ', ROWS * COLS); // Clear Chars
  memset(ScreenCharBuffer, ' ', ROWS * COLS); // Clear Buffer
  memset(ScreenColors, 1, ROWS * COLS); // clear Colors
  memset(ScreenColorBuffer, 1, ROWS * COLS); // clear Color Buffer
  #endif

  #if defined(__APPLE2__)
  byte i;
  for (i = 0; i < 192; ++i)
    memset(&HGR[RowsHGR[i]], 0, 40); //Or just disable screen and clear it?
  memset(ScreenChars, ' ', 0x0400); // Clear Chars (text page 1 on Apple II)
  //memset(HGR, 0, 0x2000); // clear HGR page 1
  #endif
  
  #if defined (__NES__)
  //byte x, y;
  ppu_off();
  vrambuf_flush();
  vram_adr(NTADR_A(0, 0));
  //for (y = 0; y < ROWS; ++y)
    //for (x = 0; x < COLS; ++x)
      //SetChar(' ', x, y);
  vram_fill(' ', ROWS*COLS);
  //vram_adr(NTADR_A(0, 30));	// start address ($2000)
  //vram_fill(0, 64);
  ppu_on_all();
  #endif
  
  #if defined (MSX)
  CLS();
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
void SelectBank(byte bank)
{
  bank;
}
void SelectScreenPos(byte pos)
{
  int* regd018 = (int*)0xD018;
  byte a = pos * 16;
  raster_wait(255);
  regd018[0] = (regd018[0] & 15) | a;
  //POKE (0xD018,(PEEK(0xD018) & 15) | a);
}
void SelectCharPos(byte charpos)
{
  charpos;
}
void SetMulticolors(byte color1, byte color2)
{
  byte *colorReg = (byte*)0xD022;
  byte *vicReg = (byte*)0xD016;
  colorReg[0] = color1;
  colorReg[1] = color2;
  vicReg[0] = vicReg[0] | 16;
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
  
  int* charfile = (int*)0x0840;
  int* attribfile = (int*)0x1040;
  
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
  memcpy(&attributeset[0], &attribfile[0], 256);
  

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
  
  // set palette colors
  pal_col(0,0x0f);
  pal_col(1,0x10);
  pal_col(2,0x10);
  pal_col(3,0x10);
  vrambuf_clear();
  set_vram_update(updbuf);
  //vrambuf_flush();
  // enable PPU rendering (turn on screen)
  ppu_on_all();
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
const byte blanksA[7] = 
{
  0b11111100,
  0b11110011,
  0b11001111,
  0b10111111,
  0b11111001,
  0b11100111,
  0b10011111,
};
const byte paletteTable[6] = {
  0b00,
  0b11,
  0b01,
  0b10,
  0b01,
  0b10,
};
const byte shiftTable[7] = {
  0,
  2,
  4,
  0,
  1,
  3,
  5,
};
void A2Pixel(byte x, byte y, byte color)
{
  int offset = RowsHGR[y] + (x<<1) / 7;
  byte index = color;
  byte xPixel = x % 7; //Which pixel of 7 in a 2-byte pair;
  index  = index % 6;
  color = paletteTable[index];
  
  HGR[offset] &= blanksA[xPixel]; //Blank Pixels
  HGR[offset] &= 0b01111111; //Clear Palette
  
  if (xPixel == 3)
  { 
    //HGR[offset] |= ((color << 7) >> 1); //Last
    HGR[offset] |= ((color << 6) & 0b01000000); //Last
    HGR[offset + 1] &= 0b11111110;
    HGR[offset + 1] |= (color >> 1); //First
  }
  else
    HGR[offset] |= color << shiftTable[xPixel];
  
  if (index > 3)
    HGR[offset] |= 0b10000000; //Set Palette  
}

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

char SetCharIndex;
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
  ScreenColors[offset] = attributeset[SetCharIndex];
  #endif
  
  #if defined (__NES__)
  ScreenChars[offset] = SetCharIndex;
  //vram_adr(NTADR_A(SetCharX,SetCharY));		// set address
  vrambuf_put(NTADR_A(SetCharX,SetCharY), &SetCharIndex, 1);
   //vrambuf_end();
  // wait for next frame to flush update buffer
  // this will also set the scroll registers properly
  //ppu_wait_frame();
  // clear the buffer
  ++charsDrawn;
  if (charsDrawn % 21 == 0)
    vrambuf_flush();
  //vram_put(SetCharIndex);
  //ppu_on_all();
  #endif
  
  #if defined(MSX)
  //POSIT(SetCharY+1+(SetCharX<<8));
  //CHPUT((int)SetCharIndex);
  //SETWRT();
  WRTVRM(0x1800 + (SetCharY * 32 + SetCharX),SetCharIndex);
  
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
}

void SetCharBuffer(byte index, byte x, byte y)
{
  #if defined(__APPLE2__)
  ScreenChars[x + YColumnIndex[y]] = index;
  #endif
  #if defined(__C64__)
  int offset = x + YColumnIndex[y];
  ScreenCharBuffer[offset] = index;
  ScreenColorBuffer[offset] = attributeset[index];
  #endif
  #if defined(__NES__)
  //ScreenChars[x + YColumnIndex[y]] = index;
  index;x;y;
  #endif
  #if defined(MSX)
  index;x;y;
  #endif
}

byte GetChar(byte x, byte y)
{
 // #if defined (__C64__)
 // return ScreenChars[x + YColumnIndex[y]];
  //#endif
 // #if defined (__APPLE2__)
  return ScreenChars[x + YColumnIndex[y]];
 // #endif
 // #if defined (__NES__)
  
  //char value;
  //x;y;
  //ppu_off();
  //vram_adr(NTADR_A(x,y));
  //vram_read(&value, 1);
  //ppu_on_all();
  //return value;
  //#endif
}

//Buffer
/*void CopyBuffer(void)
{
  #if defined(__APPLE2__)
  int i = 0;
  byte x = 0;
  byte y = 0;
  for (y = 0; y < ROWS; ++y)
    for (x = 0; x < COLS; ++x)
    {
      DrawChar(ScreenChars[i],x, y);
      ++i;
    }
  #endif
  #if defined(__C64__)
  memcpy(&ScreenChars[0], &ScreenCharBuffer[0], 0x400);
  memcpy(&ScreenColors[0], &ScreenColorBuffer[0], 0x400);
  #endif
}*/

void StoreBuffer(void)
{
  #if defined(__APPLE2__)
  #endif
  #if defined(__C64__)
  memcpy(&ScreenCharBuffer[0], &ScreenChars[0], 0x400);
  memcpy(&ScreenColorBuffer[0], &ScreenColors[0], 0x400);
  #endif
}
/*
void CopyBufferArea(byte origin_x, byte origin_y, byte width, byte height)
{
  #if defined(__APPLE2__)
  byte x = 0;
  byte y = 0;
  int i = x + y * COLS;
  for (y = origin_y; y < origin_y + height; ++y)
  {
    i = y * COLS;
    for (x = origin_x; x < origin_x + width; ++x)
    {
      DrawChar(ScreenChars[i],x, y);
      ++i;
    }
  }
  #endif

  #if defined(__C64__)
  int offset = origin_x + YColumnIndex[origin_y];
  byte column;
  for (column = 0; column < height; ++column)
  {
    memcpy(&ScreenChars[offset], &ScreenCharBuffer[offset], width);
    memcpy(&ScreenColors[offset], &ScreenColorBuffer[offset], width);
    offset += COLS;
  }
  #endif
}*/

void PrintString(char *text, byte posx, byte posy, bool fast, bool buffer)
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
    if (buffer)
      SetCharBuffer(text[i], posx + i, posy);
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
  
  #if defined(__C64__)
  offset1 = YColumnIndex[DrawTileY] + DrawTileX + originOffset;
  #endif
}
void DrawTile()
{
  byte x = DrawTileX + MapOriginX;
  byte y = DrawTileY + MapOriginY;
  
  #if defined(__C64__)
  memcpy(destinationChar, &indexes[0], 2);
  memcpy(destinationColor, &attributeset[indexes[0]], 2);
  destinationChar += COLS;
  destinationColor += COLS;
  //offset1 += COLS;
  memcpy(destinationChar, &indexes[2], 2);
  memcpy(destinationColor, &attributeset[indexes[2]], 2);
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
void DrawTileBuffer(void)
{
  DrawTileSetup();
  
  #if defined(__C64__)
  destinationChar = &ScreenCharBuffer[offset1];
  destinationColor = &ScreenColorBuffer[offset1];
  #endif

  DrawTile();
}
void DrawTileDirect()
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
  
  SetTileOrigin(x, y);
  DrawTileIndex = index;
  DrawTileX = 0;
  DrawTileY = 0;
  DrawTileDirect();
  
  SetTileOrigin(tempX, tempY);
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
  PrintString(text, xPos1, yPos, true, false);
  
  wait_vblank(1);
}
