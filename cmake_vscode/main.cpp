// For playing the sound
#include <gba_interrupt.h>
#include <maxmod.h>
#include "soundbank.h"
#include "soundbank_bin.h"

mm_sound_effect soundEffects[] = {{{SFX_SOUND}, 1 << 10, 0, 255, 128}};

// For drawing the logo
#include <gba_video.h>
#include <gba_dma.h>
#include <gba_systemcalls.h>
#include "memory.h"
#include "./data/dkp_logo.h"

int main()
{
	// MaxMOD requires the vblank interrupt to reset sound DMA, so link the VBlank interrupt to mmVBlank, and enable it.
	irqInit();
	irqSet(IRQ_VBLANK, mmVBlank);
	irqEnable(IRQ_VBLANK);
	// initialise MaxMOD with soundbank and 4 channels
	mmInitDefault((mm_addr)soundbank_bin, 4);
	// Set graphics to mode 4 and enable background 2 
	REG_DISPCNT = MODE_4 | BG2_ON;
	// copy data to palette and backbuffer
	Memory::memcpy32(BG_PALETTE, dkp_logo_palette, DKP_LOGO_PALETTE_SIZE >> 2);
	Memory::memcpy32(MODE5_BB, dkp_logo, DKP_LOGO_SIZE >> 2);
	// swap backbuffer
	REG_DISPCNT = REG_DISPCNT | BACKBUFFER;
	// start plaing sound and make sure MaxMOD keeps filling buffers
	mmEffectEx(&soundEffects[0]);
	do
	{	
		VBlankIntrWait();
		mmFrame();
	} while (true);
}
