// Apex Audio demo using libgba

#include <AAS.h>
#include <AAS_Data.h>

#include <gba_interrupt.h>
#include <gba_input.h>
#include <gba_systemcalls.h>
#include <gba_video.h>
#include <gba_console.h>

#include <stdio.h>


int main() {
	int keys_pressed, keys_released;

	// Initialise the interrupt handlers
	irqInit();

	// Install the apex audio timer1 interrupt
	irqSet( IRQ_TIMER1, AAS_Timer1InterruptHandler);

	// Initialise AAS
	// AAS_SetConfig appears to set REG_IE to IE_TIMER1, requiring other interrupts to be enabled later.
	AAS_SetConfig( AAS_CONFIG_MIX_32KHZ, AAS_CONFIG_CHANS_8, AAS_CONFIG_SPATIAL_STEREO, AAS_CONFIG_DYNAMIC_OFF );


	// the vblank interrupt must be enabled to use VBlankIntrWait
	// no handler is required since the libgba dispatcher handles the bios flags
	irqEnable(IRQ_VBLANK);
	REG_IME = 1;

	// Start playing MOD
	AAS_MOD_Play( AAS_DATA_MOD_FlatOutLies );

	// Show AAS Logo (required for non-commercial projects)
	AAS_ShowLogo();

	// initialise the console
	// setting NULL & 0 for the font address & size uses the default font
	// The font should be a complete 1bit 8x8 ASCII font
	consoleInit(	0,		/* charbase */
					4,		/* mapbase */
					0,		/* background number */
					NULL,	/* font */
					0, 		/* font size */
					15		/* 16 color palette */);

	// set the screen colors, color 0 is the background color
	// the foreground color is index 1 of the selected 16 color palette
	BG_COLORS[0]=RGB8(58,110,165);
	BG_COLORS[241]=RGB5(31,31,31);

	SetMode(MODE_0 | BG0_ON);

	// ansi escape sequence to clear screen and home cursor
	// /x1b[line;columnH
	iprintf("\x1b[2J");

	// ansi escape sequence to set print co-ordinates
	// /x1b[line;columnH
	iprintf("\x1b[0;8HApex Audio demo");
	VBlankIntrWait();
	iprintf("\x1b[3;0HHold A for ambulance sound");
	iprintf("\x1b[4;0HPress B for boom sound");
	// Main loop
	do {
		VBlankIntrWait();
		scanKeys();

		keys_pressed = keysDown();
		keys_released = keysUp();

		// Play looping ambulance sound effect out of left speaker if A button is pressed, stop when released
		if ( keys_pressed & KEY_A ) {
			AAS_SFX_Play( 0, 64, 16000, AAS_DATA_SFX_START_Ambulance, AAS_DATA_SFX_END_Ambulance, AAS_DATA_SFX_START_Ambulance );
		}

		if ( keys_released & KEY_A ) {
			AAS_SFX_Stop( 0 );
		}

		// Play explosion sound effect out of right speaker if B button is pressed
		if ( keys_pressed & KEY_B ) {
			AAS_SFX_Play( 1, 64, 8000, AAS_DATA_SFX_START_Boom, AAS_DATA_SFX_END_Boom, AAS_NULL );
		}
	}
	while( 1 );
}
