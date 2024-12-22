#ifndef _XINAGA_H
#define _XINAGA_H

#include <string.h> //For memcpy
#include <stdint.h> //For byte and sbyte
#include <conio.h> //for cgetc() and kbhit()
#include <stdio.h>
#include <stdlib.h>
#include <peekpoke.h>


//Viewport and Console
#define consolePosX  2
#define consolePosY (ROWS - (consoleHeight + 3))
#define consoleWidth (COLS - (contextMenuWidth - 3))
#define consoleHeight 5

#define selectionPosX consoleWidth + 3
#define selectionPosY consolePosY
#define selectionWidth 6
#define selectionHeight consoleHeight
#define selectionCount 4

#define viewportPosX 2
#define viewportPosY 2
#define viewportWidth 7//((COLS - (contextMenuWidth + 2) - 1) / 2)
#define viewportHeight 7

#define contextMenuPosX 17
#define contextMenuPosY 2
#define contextMenuWidth 13


#if defined(__APPLE2__)
#define COLS 40
#define ROWS 24
#define STROBE(addr) __asm__ ("sta %w", addr)
#pragma warn (remap-zero, push, off)
#pragma charmap ('^', 0x7E)
#pragma warn (remap-zero, pop)
#endif

#if defined(__C64__)
#include <cbm_petscii_charmap.h>
#define COLS 40
#define ROWS 25
#endif

#if defined (__NES__)
//#pragma data-name (push, "XRAM")
//#pragma bss-name (push, "XRAM")
//#include <cbm_petscii_charmap.h>
#define COLS 32
#define ROWS 30

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
//#pragma bss-name ("XRAM")
#endif

typedef uint8_t byte;
typedef int8_t sbyte;	// 8-bit signed

//typedef enum { false, true } bool;
#define bool byte
#define false 0
#define true 1
//typedef enum { up, down, left, right } direction;
#define direction byte
#define up 0
#define down 1
#define left 2
#define right 3

extern uint16_t randseed;
byte ReadBit(byte byteToRead, char bit);
void LoadMap(void);

#if defined (__C64__)
extern const byte const charset[2048];
extern const byte const attributeset[256];
extern byte* CharRam;
#endif

#if defined (__APPLE2__)
extern const byte const charset[2048];
extern const byte const attributeset[256];
extern byte* CharRam;
#endif

#if defined (__NES__)
//#pragma rodata-name (push, "BANK2")
//extern const byte const *charset;
//extern const byte const *attributeset;
//extern byte* CharRam;
//#pragma rodata-name (pop)
#endif


//Graphics
//	System
#define ColorBG 0
#define ColorBorder ColorBG
#define ColorText 1
void InitializeGraphics(void);
void wait_vblank(byte frames);
#if defined(__C64__)
void raster_wait(byte line);
void SetBorder(byte color);
void SetBG(byte color);
#endif

extern int YColumnIndex[ROWS];
//extern byte* MapSetInfo;

//	Drawing
void ClearScreen(void);
//void SetChar();
extern char SetCharIndex;
extern byte SetCharX;
extern byte SetCharY;
void _SetChar(void);
#define SetChar(index, x, y) do {SetCharIndex = (index); SetCharX = (x); SetCharY = (y); _SetChar();}while(0)
void SetColor(byte x, byte y, byte index);
byte GetChar(byte x, byte y);
//void SetCharBuffer(byte x, byte y, byte index);
void DrawLineH(byte index, byte x, byte y, byte length);
void DrawLineV(byte index, byte x, byte y, byte length);
void DrawBorder(char *text, byte xPos, byte yPos, byte width, byte height, bool fill);
void DrawCharset(void);
//	Scrolling
//void ScrollReset(void);
//void Scroll(direction);

//	Tiles
void SetTileOrigin(byte x, byte y);
extern byte DrawTileX;
extern byte DrawTileY;
extern byte DrawTileIndex;
void DrawTileBuffer();
void DrawTileDirect();
void DrawTileDirectXY(byte index, byte x, byte y);
void ReadyArrow(byte x, byte y);
void DrawArrow(byte x, byte y);
void ClearArrow(void);

//	Text
void PrintString(char *text, byte posx, byte posy, bool fast, bool buffer);

//	Buffer
void SetBuffer(bool value);
void SwapBuffer(void);
void UpdateColors(void);
void CopyBuffer(void);
void StoreBuffer(void);
//void CopyBufferArea(byte posX, byte posY, byte sizeX, byte sizeY);
//void ReverseBufferArea(byte posX, byte posY, byte sizeX, byte sizeY);

//Debug
void DrawCharacterSet(byte destX, byte destY);

//Audio
extern void PlaySong(byte index);
extern void StopSong(void);
extern void PlaySound(int length, int freq);


//File I/O
void DiskSave(char filename[], int source, int length);
void DiskLoad(char filename[], int dest);

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

//Console
void ResizeMessageWindow (byte x, byte y, byte w, byte h);

void WriteLineMessageWindow(char *message, byte delay);
void SetLineMessageWindow(char *message, byte delay);
extern char *Messages[4];
void DrawCharStats(void);
#define ConsoleBufferLength 128
extern byte strTemp[ConsoleBufferLength];
void ConsoleBufferReset();
void ConsoleBufferAdd(char *message);
void ConsoleBufferAddNumber(int value);
void ConsoleBufferPrint(byte x, byte y);
void ConsoleBufferPrintConsole(byte delay);

//Selection Menu
void ResetMenu(char *title, byte posX, byte posY, byte w, byte h, byte c);
void SetMenuItem(byte index, char *value);
byte GetMenuSelection(void);
void HighlightMenuItem(byte index);
byte IsMenuItemHighlighted(byte index);
void DrawMenu(void);
void ClearMenu(void);
#endif