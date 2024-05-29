#ifndef _XINAGA_H
#define _XINAGA_H

#include <string.h> //For memcpy
#include <stdint.h> //For byte and sbyte

#define COLS 40
#if defined(__APPLE2__)
#define ROWS 24
#define STROBE(addr) __asm__ ("sta %w", addr)
#endif
#if defined(__C64__)
#include <cbm_petscii_charmap.h>
#define ROWS 25
#endif


typedef uint8_t byte;
typedef int8_t sbyte;	// 8-bit signed

typedef enum { false, true } bool;
typedef enum { up, down, left, right } direction;

void LoadMap();

//Graphics
//	System
#define ColorBG 0
#define ColorBorder ColorBG
#define ColorText 1
void InitializeGraphics(void);
#if defined(__C64__)
void wait_vblank(byte frames);
void raster_wait(byte line);
void SetBorder(byte color);
void SetBG(byte color);
#endif

extern int YColumnIndex[25];
extern byte* MapSetInfo;
extern byte MapOriginX, MapOriginY;

//	Drawing
void ClearScreen(void);
void SetChar(byte x, byte y, byte index);
void SetColor(byte x, byte y, byte index);
byte GetChar(byte x, byte y);
//void SetCharBuffer(byte x, byte y, byte index);
void DrawLineH(char index, byte x, byte y, byte length);
void DrawLineV(char index, byte x, byte y, byte length);
void DrawBorder(char text[20], byte xPos, byte yPos, byte width, byte height, bool fill);

//	Scrolling
void ScrollReset();
void Scroll(direction);

//	Tiles
void SetTileOrigin(byte x, byte y);
void DrawTileFast(byte index, byte x, byte y);
void DrawTileDirect(byte index, byte x, byte y);
void ReadyArrow(byte x, byte y);
void DrawArrow(byte x, byte y);
void ClearArrow();

//	Text
void PrintString(char text[20], byte posx, byte posy, bool fast, bool buffer);

//	Buffer
void SetBuffer(bool value);
void SwapBuffer(void);
void UpdateColors(void);
void CopyBuffer(void);
void StoreBuffer(void);
void CopyBufferArea(byte posX, byte posY, byte sizeX, byte sizeY);
void ReverseBufferArea(byte posX, byte posY, byte sizeX, byte sizeY);

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
//void DrawMessageWindow();
//void BlankMessageWindow();
void WriteLineMessageWindow(char *message, byte delay);
void SetLineMessageWindow(char *message, byte delay);
extern char Messages[4][20];
void DrawCharStats();

//Selection Menu
void ResetMenu(char *title, byte posX, byte posY, byte w, byte h, byte c);
void SetMenuItem(byte index, char *value);
byte GetMenuSelection();

//Common Functions
//byte ReadBit(byte byteToRead, char bit); //These are old
//void WriteBit(byte *byteToSet, char bit, bool value); //These are old
//bool CheckBit(byte source, byte position); //These are old

//byte SetBit(byte byteToSet , byte k); //These work right
//byte ClearBit(byte byteToSet , byte k); //These work right
#endif