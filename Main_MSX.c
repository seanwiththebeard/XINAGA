#include "Xinaga.h"
#include "GameData.h"

//#link "XINAGA_Graphics.c"
//#link "XINAGA_Input.c"
//#link "XINAGA_FileIO.c"
//#link "XINAGA_Audio.c"
//#link "XINAGA_Console.c"
//#link "Game_Demo.c"
//#link "Game_System.c"
//#link "Game_Screen_Title.c"
//#link "Game_Screen_AddCharacter.c"
//#link "Game_Screen_Map.c"
//#link "Game_Screen_Combat.c"
//#link "Game_Screen_MapGen.c"
//#link "Game_Screen_Scenario.c"

#include "msxbios.h"
//#link "msxbios.c"

unsigned char frp2() __naked {
   __asm
      frp2: ; ----------- @@ROM:
	di
	; Slot primario
	call #0x0138 ; call RSLREG
	rrca
	rrca
	and #0x03
	; Slot secundario
	ld c, a
	ld hl, #0xfcc1
	add a, l
	ld l, a
	ld a, (hl)
	and #0x80
	or c
	ld c, a
	inc l
	inc l
	inc l
	inc l
	ld a, (hl)
	; Definir el identificador de slot
	and #0x0c
	or c
	ld h, #0x80
	; Habilitar permanentemente
	call #0x0024 ; call ENASLT
	ei
	ret
   __endasm;
}

unsigned char find_rom_page_2() __naked {
   __asm
      ld hl, #0x4000
	ld b, (hl)
	xor a
	ld (hl), a
	ld a, (hl)
	or a
	jr nz,frp2 ; jr nz,@@ROM
	; El programa esta en RAM - no buscar
	ld (hl),b
	ret
   __endasm;
}



void main(void)
{
  find_rom_page_2();
  Demo();
}
