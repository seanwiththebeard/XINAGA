#ifndef _XINAGA_H
#define _XINAGA_H

#include <string.h>
#include <conio.h>
#include <stdint.h>

#define COLS 40
#define ROWS 24

typedef uint8_t byte;
typedef enum { false, true } bool;

//Graphics
void InitializeGraphics(void);

//	Drawing
void ClearScreen(void);
void SetChar(byte x, byte y, byte index);
void SetCharBuffer(byte x, byte y, byte index);
void DrawLineH(char index, byte x, byte y, byte length);
void DrawLineV(char index, byte x, byte y, byte length);
void DrawBorder(byte xPos, byte yPos, byte width, byte height, bool fill);
void SetTileOrigin(byte x, byte y);
void DrawTileFast(byte index, byte x, byte y);

//	Text
void PrintString(char text[16], byte posx, byte posy, bool fast, bool buffer);

//	Buffer
void UpdateColors(void);
void CopyBuffer(void);
void CopyBufferArea(byte posX, byte posY, byte sizeX, byte sizeY);
void ReverseBufferArea(byte posX, byte posY, byte sizeX, byte sizeY);

//	Scrolling
void ScrollReset(void);
void MoveScreenUp(void);
void MoveScreenDown(void);

//	Debug
void DrawCharacterSet(byte destX, byte destY);

//Audio
extern void PlaySong(void);
extern void StopSong(void);


//File I/O
void DiskSave(char filename[], int source, int length);
void DiskLoad(char filename[], int dest);

//Input
void InitializeInput();
void UpdateInput();
bool InputChanged();
bool NoInput();
bool InputUp();
bool InputDown(); 
bool InputLeft();
bool InputRight();
bool InputFire();


#endif