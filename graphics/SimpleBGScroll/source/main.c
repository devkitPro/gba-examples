// --------------------------------------------------------------------
//
//  Super Simple Background Text Scroller v1.0
//
//   Not terribly unlike how we did it in the c64 days.  Although this
//   is C and on the c64 we used asm, the concept is very much similar.
//   This source is set up so you can tweak the speed and the row to
//   scroll the text on...  In actuality there are a number of ways
//   to perform a scroller like this, but I tried to break it down
//   in a simple fashion to make it easier to understand (hopefully!)
//
//   r6502 - 2004-04-02
//
//   Note: tabs == 4 on my setup
//
// --------------------------------------------------------------------

#include <gba_base.h>
#include <gba_video.h>
#include <gba_systemcalls.h>
#include <gba_interrupt.h>

#include "r6502_portfont_bin.h"

// --------------------------------------------------------------------

#define MAPADDRESS		MAP_BASE_ADR(31)	// our base map address
#define DELAY			2			// slow things down
#define TILEWIDTH		8			// how much to scroll
#define ROW				10			// what row to place text at

// --------------------------------------------------------------------

const u16 palette[] = {
	RGB8(0x40,0x80,0xc0),
	RGB8(0xFF,0xFF,0xFF),
	RGB8(0xF5,0xFF,0xFF),
	RGB8(0xDF,0xFF,0xF2),
	RGB8(0xCA,0xFF,0xE2),
	RGB8(0xB7,0xFD,0xD8),
	RGB8(0x2C,0x4F,0x8B)
};

// --------------------------------------------------------------------

const u8 message[] = {
	"                                " \
	"Hello, this is an example of an oldschool simple tile scroller " \
	"not unlike how it was done in days of yore.  The '@' symbol " \
	"at the top of your screen is intentional, to dispel the illusion " \
	"of this scroller, to demonstrate the simple concept behind it. " \
	"Check out the source to learn how it works.  It is very simple! " \
	"This exercise brought to you by r6502...          " \
	"Text is about to restart... "
};

// --------------------------------------------------------------------


void updatescrolltext(u32 idx)
{
	u32 i;
	u16 *temppointer;

	temppointer = (u16 *)MAPADDRESS + (ROW * 32);

	// write out a whole row of text to the map
	for(i=0; i<32; i++)
	{
		// check for end of message so we can wrap around properly
		if(message[idx] == 0) idx = 0;

		// write a character - we subtract 32, because the font graphics
		// start at tile 0, but our text is in ascii (starting at 32 and up)
		// in other words, tile 0 is a space in our font, but in ascii a
		// space is 32 so we must account for that difference between the two.
		*temppointer++ = message[idx++] - 32;
	}
}


int main() {
	// Set up the interrupt handlers
	irqInit();
	// Enable Vblank Interrupt to allow VblankIntrWait
	irqEnable(IRQ_VBLANK);

	// Allow Interrupts
	REG_IME = 1;

	u32 i, scrollx, scrolldelay, textindex;
	u16 *temppointer;


	// load the palette for the background, 7 colors
	temppointer = BG_COLORS;
	for(i=0; i<7; i++) {
		*temppointer++ = palette[i];
	}

	// load the font into gba video mem (48 characters, 4bit tiles)

	CpuFastSet(r6502_portfont_bin, (u16*)VRAM,(r6502_portfont_bin_size/4) | COPY32);

	// clear screen map with tile 0 ('space' tile) (256x256 halfwords)

	*((u32 *)MAP_BASE_ADR(31)) =0;
	CpuFastSet( MAP_BASE_ADR(31), MAP_BASE_ADR(31), FILL | COPY32 | (0x800/4));

	// set screen H and V scroll positions
	BG_OFFSET[0].x = 0; BG_OFFSET[0].y = 0;

	// initialize our variables
	scrollx = 0;
	textindex = 0;
	scrolldelay = 0;

	// put the '@' symbol on the top of the screen to show how
	// the screen is only scrolling 7 pixels - to reveal the
	// illusion of how the scroller works
	*((u16 *)MAPADDRESS + 1) = 0x20;	// 0x20 == '@'

	// draw a row of text from beginning of message
	updatescrolltext(0);

	// set the screen base to 31 (0x600F800) and char base to 0 (0x6000000)
	BGCTRL[0] = SCREEN_BASE(31);

	// screen mode & background to display
	SetMode( MODE_0 | BG0_ON );

	while(1) {
		VBlankIntrWait();

		// check if we reached our delay
		if(scrolldelay == DELAY) {
			// yes, the delay is complete, so let's reset it
			scrolldelay = 0;

			// check if we reached our scrollcount
			if(scrollx == (TILEWIDTH-1)) {
				// yes, we've scrolled enough, so let's reset the count
				scrollx = 0;

				// check if we reached the end of our scrolltext
				// and if so we need to restart our index
				if(message[textindex] == 0) textindex = 0;
				else textindex++;

				// finally, let's update the scrolltext with the current text index
				updatescrolltext(textindex);
			}
			else scrollx++;
		}
		else scrolldelay++;

		// update the hardware horizontal scroll register
		BG_OFFSET[0].x = scrollx;
	}

}
