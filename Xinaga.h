#ifndef _XINAGA_H
#define _XINAGA_H

#include <string.h> //For memcpy
#include <stdint.h> //For byte and sbyte
#include <stdlib.h> //For srand

typedef uint8_t byte;
typedef int8_t sbyte;	// 8-bit signed
#define bool byte
#define false 0
#define true 1
#define direction byte
#define up 0
#define down 1
#define left 2
#define right 3

extern uint16_t randseed;
#define ReadBit(b, n) (((b) >> (n)) & 1)

extern const byte characterset[2048];
extern const byte attributeset[256];

//Screen Layout
#if defined(__APPLE2__)
#define COLS 40
#define ROWS 24

#define viewportPosX 1
#define viewportPosY 1
#define viewportWidth 9
#define viewportHeight 9

#define consolePosX 1
#define consolePosY 2 + 2*viewportHeight
#define consoleWidth 38
#define consoleHeight (ROWS - viewportHeight * 2 - 3)

#define selectionPosX consoleWidth + 2
#define selectionPosY consolePosY
#define selectionWidth 6
#define selectionHeight consoleHeight
#define selectionCount 4

#define contextMenuPosX viewportPosX + viewportWidth * 2 + 1
#define contextMenuPosY viewportPosY
#define contextMenuWidth (COLS - (viewportWidth * 2) - 3)
#define contextMenuHeight (2 * viewportHeight)

#include <conio.h> //for cgetc() and kbhit()
#define STROBE(addr) __asm__ ("sta %w", addr)
#pragma warn (remap-zero, push, off)
#pragma charmap ('^', 0x7E)
#pragma warn (remap-zero, pop)
extern char HGR[0x2000];
extern unsigned int RowsHGR[192];
extern const byte const charset[2048];
extern byte* CharRam;
void A2Pixel(byte x, byte y, byte color);
#endif

#if defined(__C64__)
#define COLS 40
#define ROWS 25

#define viewportPosX 1
#define viewportPosY 1
#define viewportWidth 9
#define viewportHeight 9

#define consolePosX 1
#define consolePosY 20 //2 + 2*viewportHeight
#define consoleWidth 38
#define consoleHeight 4//(ROWS - viewportHeight * 2 - 3)

#define selectionWidth 6
#define selectionPosX 33//consoleWidth - selectionWidth + 1
#define selectionPosY consolePosY

#define selectionHeight consoleHeight
#define selectionCount 4

#define contextMenuPosX 20//viewportPosX + viewportWidth * 2 + 1
#define contextMenuPosY viewportPosY
#define contextMenuWidth 19//(COLS - (viewportWidth * 2) - 3)
#define contextMenuHeight 18//(2 * viewportHeight)

#include <cbm_petscii_charmap.h>
#pragma charmap(0x41, 0x61)
#pragma charmap(0x42, 0x62)
#pragma charmap(0x43, 0x63)
#pragma charmap(0x44, 0x64)
#pragma charmap(0x45, 0x65)
#pragma charmap(0x46, 0x66)
#pragma charmap(0x47, 0x67)
#pragma charmap(0x48, 0x68)
#pragma charmap(0x49, 0x69)
#pragma charmap(0x4a, 0x6a)
#pragma charmap(0x4b, 0x6b)
#pragma charmap(0x4c, 0x6c)
#pragma charmap(0x4d, 0x6d)
#pragma charmap(0x4e, 0x6e)
#pragma charmap(0x4f, 0x6f)
#pragma charmap(0x50, 0x70)
#pragma charmap(0x51, 0x71)
#pragma charmap(0x52, 0x72)
#pragma charmap(0x53, 0x73)
#pragma charmap(0x54, 0x74)
#pragma charmap(0x55, 0x75)
#pragma charmap(0x56, 0x76)
#pragma charmap(0x57, 0x77)
#pragma charmap(0x58, 0x78)
#pragma charmap(0x59, 0x79)
#pragma charmap(0x5a, 0x7a)

#pragma charmap(0x61, 0x41)
#pragma charmap(0x62, 0x42)
#pragma charmap(0x63, 0x43) //c
#pragma charmap(0x64, 0x44) //d
#pragma charmap(0x65, 0x45)
#pragma charmap(0x66, 0x46)
#pragma charmap(0x67, 0x47)
#pragma charmap(0x68, 0x48)
#pragma charmap(0x69, 0x49) //i
#pragma charmap(0x6a, 0x4a)
#pragma charmap(0x6b, 0x4b)
#pragma charmap(0x6c, 0x4c)
#pragma charmap(0x6d, 0x4d)
#pragma charmap(0x6e, 0x4e)
#pragma charmap(0x6f, 0x4f)
#pragma charmap(0x70, 0x50)
#pragma charmap(0x71, 0x51)
#pragma charmap(0x72, 0x52)
#pragma charmap(0x73, 0x53)
#pragma charmap(0x74, 0x54)
#pragma charmap(0x75, 0x55)
#pragma charmap(0x76, 0x56)
#pragma charmap(0x77, 0x57)
#pragma charmap(0x78, 0x58)
#pragma charmap(0x79, 0x59)
#pragma charmap(0x7a, 0x5a)

#endif

#if defined(__NES__)
#include <peekpoke.h>

#define COLS 32
#define ROWS 30

#define consolePosX 2
#define consolePosY (viewportPosY + (viewportHeight * 2) + 1)
#define consoleWidth ROWS - selectionWidth - 3
#define consoleHeight (ROWS - consolePosY - 3)

#define selectionPosX COLS - selectionWidth - 2
#define selectionPosY consolePosY
#define selectionWidth 6
#define selectionHeight consoleHeight
#define selectionCount 4

#define viewportPosX 2
#define viewportPosY 2
#define viewportWidth 9
#define viewportHeight 9

#define contextMenuPosX viewportPosX + viewportWidth * 2 + 1
#define contextMenuPosY viewportPosY
#define contextMenuWidth (COLS - (viewportWidth * 2) - 5)
#define contextMenuHeight (2 * viewportHeight)

#define MMC_MODE 0x00
#define MMC3_SET_REG(r,n)\
	POKE(0x8000, MMC_MODE|(r));\
	POKE(0x8001, (n));
#define MMC3_CHR_0000(n) MMC3_SET_REG(0,n)
#define MMC3_CHR_0800(n) MMC3_SET_REG(1,n)
#define MMC3_CHR_1000(n) MMC3_SET_REG(2,n)
#define MMC3_CHR_1400(n) MMC3_SET_REG(3,n)
#define MMC3_CHR_1800(n) MMC3_SET_REG(4,n)
#define MMC3_CHR_1C00(n) MMC3_SET_REG(5,n)
#define MMC3_PRG_8000(n) MMC3_SET_REG(6,n)
#define MMC3_PRG_A000(n) MMC3_SET_REG(7,n)
#define MMC3_MIRROR(n) POKE(0xa000, (n))
//; WRAM_OFF $40
//; WRAM_ON $80
//; WRAM_READ_ONLY $C0
#define MMC3_WRAM_DISABLE() POKE(0xA001, 0x40)
#define MMC3_WRAM_ENABLE() POKE(0xA001, 0x80)
#define MMC3_WRAM_READ_ONLY() POKE(0xA001, 0xC0)
extern byte ScreenChars[ROWS*COLS];
extern char ATTRIBUTE_TABLE[0x40];
extern const char PALETTE_0[16];
extern const char PALETTE_1[16];
extern const char PALETTE_2[16];
extern const char PALETTE_3[16];
#endif

#if defined(MSX)
#include "msxbios.h"
#define COLS 32
#define ROWS 24
#define consolePosX 2
#define consolePosY 2*viewportHeight
#define consoleWidth 28
#define consoleHeight (ROWS - (viewportHeight * 2) - 2)
#define selectionPosX COLS - selectionWidth
#define selectionPosY consolePosY
#define selectionWidth 6
#define selectionHeight consoleHeight
#define selectionCount 4
#define viewportPosX 1
#define viewportPosY 1
#define viewportWidth 9
#define viewportHeight 9
#define contextMenuPosX viewportPosX + (viewportWidth * 2) + 1
#define contextMenuPosY viewportPosY
#define contextMenuWidth (COLS - (viewportWidth * 2) - 3)
#define contextMenuHeight (2 * viewportHeight)
#endif

//Graphics
//	System
#define ColorBG 0
#define ColorBorder ColorBG
#define ColorText 1
void InitializeGraphics(void);
void wait_vblank(byte frames);
void SetBorder(byte color);
void SetBG(byte color);
void UpdateAttributes(void);
void ScreenFadeOut(void);
void ScreenFadeIn(void);

extern int YColumnIndex[ROWS];

//	Drawing
extern byte SetCharIndex;
extern byte SetCharX;
extern byte SetCharY;
void _SetChar(void);
#define SetChar(index, x, y) do {SetCharIndex = (index); SetCharX = (x); SetCharY = (y); _SetChar();}while(0)
void ClearScreen(void);
void DrawLineH(byte index, byte x, byte y, byte length);
void DrawLineV(byte index, byte x, byte y, byte length);
void DrawBorder(char *text, byte xPos, byte yPos, byte width, byte height, bool fill);
void DrawCharset(void);

//	Tiles
#define TileCount 64
#define TileSize 16
extern byte DrawTileX;
extern byte DrawTileY;
extern byte DrawTileIndex;
//extern byte DrawTilePalette;
extern byte tilesBlocked[TileCount];
extern byte tilesOpaque[TileCount];
extern byte tilesPalette[TileCount];
void SetTileOrigin(byte x, byte y);
extern byte tilePosX;
extern byte tilePosY;
#if defined(__C64__) || (__APPLE2__) || (__NES__)
#pragma zpsym ("tilePosX")
#pragma zpsym ("tilePosY")
#endif
//void DrawTileBuffer(bool drawChars);
void DrawTileDirect(void);
void DrawTileDirectXY(byte index, byte x, byte y);
void DrawTileSeq(byte index);
void MapFadeOut(void);
void MapFadeIn();
void FillViewport(byte index, byte width, byte height);
void ReadyArrow(byte x, byte y);
void DrawArrow(byte x, byte y);
void ClearArrow(void);

//	Buffer
//void SetBuffer(bool value);
//void SwapBuffer(void);
//void UpdateColors(void);
//void StoreBuffer(void);

//Audio
//extern void PlaySong(byte index);
//extern void StopSong(void);
//extern void PlaySound(int length, int freq);

//File I/O
void DiskSave(char *filename, byte *source, int size);
void DiskLoad(char *filename, byte *dest, int size);

//Input
void InitializeInput(void);
void UpdateInput(void);
bool InputChanged(void);
bool NoInput(void);
bool InputUp(void);
bool InputDown(void); 
bool InputLeft(void);
bool InputRight(void);
bool InputFire(void);

//	Text
int sprintf (char* buf, const char* format, ...);
void PrintString(char *text, byte posx, byte posy, bool fast);

//Console
#define ConsoleBufferLength 128
extern byte strTemp[ConsoleBufferLength];
extern const char* const Messages[];
void ResizeMessageWindow (byte x, byte y, byte w, byte h);
void WriteLineMessageWindow(char *message, byte delay);
void SetLineMessageWindow(char *message, byte delay);
void ConsoleBufferReset(void);
void ConsoleBufferAdd(char *message);
//void ConsoleBufferBackspace(void);
//void ConsoleBufferAddNumber(int value);
void ConsoleBufferPrint(byte x, byte y);
//void ConsoleBufferPrintConsole(byte delay);
//void DrawConsoleContent(void);
void DrawCharStats(void);

//Selection Menu
void ResetMenu(char *title, byte posX, byte posY, byte w, byte h, byte c);
void SetMenuItem(byte index, char *value);
void HighlightMenuItem(byte index);
void DrawMenu(void);
void ClearMenu(void);
byte IsMenuItemHighlighted(byte index);
byte GetMenuSelection(void);
#endif
