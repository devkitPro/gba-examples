//---------------------------------------------------------------------------------
// GBA sample code for devkitARM - http://www.devkitpro.org
// xboo hello world
//
//---------------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include "gba_systemcalls.h"


// for mbv2 use mbv2.h
// for vba/mappy use mappy.h

#include "xcomms.h"

void u8loop(u8 *src, u8 *dst)  {

	for ( u8 i = 0; i< 32; i++) {
		dst[i] = src[i];
	}
}

void intloop(u8 *src, u8 *dst)  {

	for ( int i = 0; i< 32; i++) {
		dst[i] = src[i];
	}
}

//---------------------------------------------------------------------------------
// Program entry point
//---------------------------------------------------------------------------------
int main(void)
//---------------------------------------------------------------------------------
{
	xcomms_init();

	dprintf("Hello World!\n");

	while (1);
}


