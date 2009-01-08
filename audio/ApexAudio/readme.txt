Sound Example using Apex Audio System v1.08 and libgba.

In order to build this example a new directory called 'libAAS' was created
in the devkitPro folder and the AAS files copied here in the following
structure.

Once you have obtained these files then delete these lines from the makefile.

default:
	@echo "This example requires files from the Apex Audio site."
	@echo "Please see the readme.txt in this folder for details"

davem@NEUROMANCER /e/devkitPro/libAAS
$ ls -R -al
.:
total 0
drwxr-xr-x    5 davem    Administ        0 May 29 18:56 .
drwxr-xr-x   19 davem    Administ        0 May 29 18:55 ..
drwxr-xr-x    2 davem    Administ        0 May 29 18:57 bin
drwxr-xr-x    3 davem    Administ        0 May 29 18:56 include
drwxr-xr-x    2 davem    Administ        0 May 29 18:56 lib

./bin:
total 17
drwxr-xr-x    2 davem    Administ        0 May 29 18:57 .
drwxr-xr-x    5 davem    Administ        0 May 29 18:56 ..
-rwxr-xr-x    1 davem    Administ    33793 Oct 19  2003 conv2aas.exe

./include:
total 3
drwxr-xr-x    3 davem    Administ        0 May 29 18:56 .
drwxr-xr-x    5 davem    Administ        0 May 29 18:56 ..
-rw-r--r--    1 davem    Administ     4424 Nov 11  2003 AAS.h
drwxr-xr-x    2 davem    Administ        0 Oct 20  2003 LowLevelAccess

./include/LowLevelAccess:
total 1
drwxr-xr-x    2 davem    Administ        0 Oct 20  2003 .
drwxr-xr-x    3 davem    Administ        0 May 29 18:56 ..
-rw-r--r--    1 davem    Administ     1562 Oct 18  2003 AAS_Mixer.h

./lib:
total 29
drwxr-xr-x    2 davem    Administ        0 May 29 18:56 .
drwxr-xr-x    5 davem    Administ        0 May 29 18:56 ..
-rw-r--r--    1 davem    Administ    57392 Nov 11  2003 libAAS.a
		
These files may be obtained from the AAS Website at
http://www.apex-designs.net/tools_aas.html

The special linkscript and crt0 supplied with AAS is not necessary with
libgba & devkitARM. The libgba interrupt dispatcher allows nested interrupts
although it behaves as a standard single interrupt dispatcher normally. In
intensive interrupt handlers which may delay the AAS timer interrupt just
add REG_IME = 1; to the code. This will allow other interrupts to be serviced
before the handler returns.



