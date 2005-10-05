//---------------------------------------------------------------------------------
// GBA PCXView sample code for devkitARM - http://www.devkit.tk
// Suitable for a splash screen for a game
//---------------------------------------------------------------------------------
#include "gba_video.h"
#include "gba_systemcalls.h"
#include "gba_input.h"
#include "gba_interrupt.h"
#include "pcx.h"
#include "fade.h"

#include <stdlib.h>

//---------------------------------------------------------------------------------
// header for binary data generated by bin2o macro in makefile
//---------------------------------------------------------------------------------
#include "splash_pcx.h"

//---------------------------------------------------------------------------------
// storage space for palette data
//---------------------------------------------------------------------------------
u16 PaletteBuffer[256];

unsigned int frame;

//---------------------------------------------------------------------------------
void VblankInterrupt()
//---------------------------------------------------------------------------------
{
	frame += 1;
	ScanKeys();
}

//---------------------------------------------------------------------------------
// Program entry point
//---------------------------------------------------------------------------------
int main(void)
//---------------------------------------------------------------------------------
{
	// Set up the interrupt handlers
	InitInterrupt();

	SetInterrupt( IE_VBL, VblankInterrupt);

	// Enable Vblank Interrupt to allow VblankIntrWait
	EnableInterrupt(IE_VBL);

	// Allow Interrupts
	REG_IME = 1;

	SetMode( MODE_4 | BG2_ON );		// screen mode & background to display

	DecodePCX(splash_pcx, (u16*)VRAM , PaletteBuffer);

	FadeToPalette( PaletteBuffer, 60);

	while (1)
	{
		VBlankIntrWait();
	}
}


