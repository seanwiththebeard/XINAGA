#ifndef _XINAGA_H
#define _XINAGA_H

#include <string.h> //For memcpy
#include <stdint.h> //For byte and sbyte
#include <conio.h> //for cgetc() and kbhit()
#include <stdio.h>
#include <stdlib.h>
#include <peekpoke.h>

#define COLS 40

#if defined(__APPLE2__)
#define ROWS 24
#define STROBE(addr) __asm__ ("sta %w", addr)
#pragma warn (remap-zero, push, off)
#pragma charmap ('^', 0x7E)
#pragma warn (remap-zero, pop)
#endif

#if defined(__C64__)
#include <cbm_petscii_charmap.h>
#define ROWS 25
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
void LoadMap();
extern byte charset[2048];
extern byte attributeset[256];
extern byte* CharRam;


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

extern int YColumnIndex[25];
extern byte* MapSetInfo;

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
void PrintString(char text[ROWS], byte posx, byte posy, bool fast, bool buffer);

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