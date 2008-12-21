/*---------------------------------------------------------------------------------

	Some emulators need the bios from your gba in order to use SWI calls
	
	This little piece of code will read the bios and save it to your
	SD/CF card device using the magic of libfat.
	
	Some cards are supported be default, the binary will need patched with
	the appropriate DLDI file for newer cards.

---------------------------------------------------------------------------------*/


#include <gba.h>
#include <fat.h>
#include <stdio.h>
#include <stdlib.h>


//---------------------------------------------------------------------------------
void waitForever() {
//---------------------------------------------------------------------------------
	while (1)
		VBlankIntrWait();
}

//---------------------------------------------------------------------------------
// Program entry point
//---------------------------------------------------------------------------------
int main(void) {
//---------------------------------------------------------------------------------


	// the vblank interrupt must be enabled for VBlankIntrWait() to work
	// since the default dispatcher handles the bios flags no vblank handler
	// is required
	irqInit();
	irqEnable(IRQ_VBLANK);

	consoleDemoInit();

	iprintf("GBA Bios Dumper\n\n");

	if (fatInitDefault()) {
		iprintf("FAT system initialised\n");
	} else {
		iprintf("FAT system failed!\n");
		waitForever();
	}
	
	u32 *bios = (u32 *)malloc(0x4000);
	
	if ( bios ) {
		iprintf("Memory allocated\n");
	} else {
		iprintf("Memory allocation failure!\n");
		waitForever();
	}

	int i;
	
	iprintf("dumping ");

	for (i=0; i<0x4000; i+=4)
	{
		// The MidiKey2Freq bios call allows us to read from bios
		// the lower bits are inaccurate, so just get it four times :)
		u32 a = MidiKey2Freq((WaveData *)(i-4), 180-12, 0) * 2;
		u32 b = MidiKey2Freq((WaveData *)(i-3), 180-12, 0) * 2;
		u32 c = MidiKey2Freq((WaveData *)(i-2), 180-12, 0) * 2;
		u32 d = MidiKey2Freq((WaveData *)(i-1), 180-12, 0) * 2;

		// rebuild a 32bit word from the 4 words we read
		u32 abcd =	( a & 0xff000000 ) |
					( d & 0xff000000 ) >> 8 |
					( c & 0xff000000 ) >> 16 |
					( b & 0xff000000 ) >> 24;
		bios[i/4] = abcd;

		//print a dot every 256 bytes
		if ( (i & 0xff) == 0 ) iprintf("."); 

	}
	
	iprintf("\nBios dumped, saving file\n");
	
	FILE *biosFile = fopen("biosgba.rom","wb");
	if (biosFile ) {
		fwrite(bios,16384,1,biosFile);
		fclose(biosFile);
		iprintf("bios saved!");
	} else {
		iprintf("file creation failed!");
	}

	waitForever();

	return 0;
}


