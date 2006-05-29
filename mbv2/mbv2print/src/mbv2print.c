//---------------------------------------------------------------------------------
// GBA sample code for devkitARM - http://www.devkitpro.org
// mbv2 hello world
//
// mb -s mbv2print_mb.gba -w 25 -x 150 -c
//---------------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include "gba_systemcalls.h"


// for Xboo Communicator use xcomms.h
// for vba/mappy use mappy.h

#include "mbv2.h"

//---------------------------------------------------------------------------------
// Program entry point
//---------------------------------------------------------------------------------
int main(void)
//---------------------------------------------------------------------------------
{
	
	dprintf("Hello World!\n");

	while (1);
}


