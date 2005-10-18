Sound Example using Apex Audio System v1.08 and libgba.

This example was built with the Apex Audio header files placed in the libgba include directory and the lib file placed in the libgba lib directory. conv2aas was copied to the devkitARM/bin folder. These files may be obtained from the AAS Website at http://www.apex-designs.net/tools_aas.html

The special linkscript and crt0 supplied with AAS is not necessary with libgba & devkitARM. The libgba interrupt dispatcher allows nested interrupts although it behaves as a standard single interrupt dispatcher normally. In intensive interrupt handlers which may delay the AAS timer interrupt just add REG_IME = 1; to the code. This will allow other interrupts to be serviced before the handler returns.



