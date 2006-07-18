#include <stdio.h>
#include <string.h>
#include <gba.h>

#include "krawall.h"
#include "mtypes.h"
#include "modules.h"
#include "samples.h"

void Vblank() {
	kramWorker();
}

int main() {

	// Initialise the interrupt handlers
	irqInit();

	// Install the krawall timer1 interrupt
	irqSet( IRQ_TIMER1, kradInterrupt);
	irqSet( IRQ_VBLANK, Vblank );

	kragInit( KRAG_INIT_STEREO );					// init krawall
	krapPlay( &mod_secondpm, KRAP_MODE_LOOP, 0 );	// play module


	// the vblank interrupt must be enabled to use VBlankIntrWait
	// no handler is required since the libgba dispatcher handles the bios flags
	irqEnable(IRQ_TIMER1 | IRQ_VBLANK);
	REG_IME = 1;

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
	iprintf("\x1b[0;8HKrawall Audio demo");
	VBlankIntrWait();
	iprintf("\x1b[3;0HPress A for Thrack");
	iprintf("\x1b[4;0HPress B for Klack");

	while(1) {
		VBlankIntrWait();
		scanKeys();

		int keys_pressed = keysDown();

		if ( keys_pressed & KEY_A ) {
			kramPlay( samples[ SAMPLE_MECH_THRACK ], 1, 0 );
		}

		if ( keys_pressed & KEY_B ) {
			kramPlay( samples[ SAMPLE_MECH_KLACK ], 1, 0 );
		}


		if ( keys_pressed & KEY_L ) {
			chandle x = kramPlay( samples[ SAMPLE_HIT_GUIT ], 1, 0 );
			kramSetPan( x, 64 );
		}

		if ( keys_pressed & KEY_R ) {
			chandle x = kramPlay( samples[ SAMPLE_HIT_SKIP ], 1, 0 );
			kramSetPan( x, -64 );
		}


	}


	return 0;
}

