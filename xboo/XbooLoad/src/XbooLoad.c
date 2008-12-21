//---------------------------------------------------------------------------------
// GBA sample code for devkitARM - http://www.devkit.tk
// demonstrates use of Xboo Communicator file server
//---------------------------------------------------------------------------------
#include <gba_video.h>
#include <gba_interrupt.h>
#include <gba_systemcalls.h>
#include <pcx.h>
#include <fade.h>
#include <xcomms.h>
#include <gba_input.h>

#include <stdio.h>
#include <stdlib.h>

//---------------------------------------------------------------------------------
// storage space for palette data
//---------------------------------------------------------------------------------
u16 PaletteBuffer[256];


unsigned int frame = 0;

//---------------------------------------------------------------------------------
void LoadPic(void) {
//---------------------------------------------------------------------------------
	int handle = dfopen("data\\splash.pcx","rb");

	dfseek(handle,0,SEEK_END);
	u32 size = dftell(handle);
	dprintf("File size is %d\n",size);

	void *splash = malloc(size);

	dfseek(handle,0,SEEK_SET);

	dfread(splash, 1, size, handle);

	dfclose(handle);

	SetMode( MODE_4 | BG2_ON );		// screen mode & background to display

	DecodePCX(splash, (u16*)VRAM , PaletteBuffer);

	free(splash);

	FadeToPalette( PaletteBuffer, 60);

}
//---------------------------------------------------------------------------------
// Program entry point
//---------------------------------------------------------------------------------
int main(void) {
//---------------------------------------------------------------------------------

	// Set up the interrupt handlers
	irqInit();

	// Enable Vblank Interrupt to allow VblankIntrWait
	irqEnable(IRQ_VBLANK);

	// Allow Interrupts
	REG_IME = 1;

	xcomms_init();

	LoadPic();

	while (1) {

		VBlankIntrWait();

		u16 keys = keysDown();

		if (keys & KEY_A) {
			FadeToBlack(30);
			LoadPic();
		}
	}
}


