Sound Example using krawall and libgba.

In order to build this example a new directory called 'Krawall' was created
in the devkitPro folder and the krawall files copied there in the following
structure.

Once you have obtained these files then delete these lines from the makefile.

default:
	@echo "This example requires files from the Krawall site."
	@echo "Please see the readme.txt in this folder for details"

krawall.h was modified for gcc 4 by replacing

#elif __GNUC__ == 3
	#define LONG_CALL __attribute__ ((long_call))
#else
	#error "This version of Krawall is for GCC2/3 only!!"
#endif

with

#elif __GNUC__ >= 3
	#define LONG_CALL __attribute__ ((long_call))
#endif


davem@NEUROMANCER /e/devkitPro_test/Krawall
$ ls -R -al
.:
total 0
drwxr-xr-x    5 davem    Administ        0 May 29 20:07 .
drwxr-xr-x   20 davem    Administ        0 May 29 20:07 ..
drwxr-xr-x    2 davem    Administ        0 May 29 20:07 bin
drwxr-xr-x    2 davem    Administ        0 Sep  6  2002 include
drwxr-xr-x    2 davem    Administ        0 May 29 20:07 lib

./bin:
total 41
drwxr-xr-x    2 davem    Administ        0 May 29 20:07 .
drwxr-xr-x    5 davem    Administ        0 May 29 20:07 ..
-rwxr-xr-x    1 davem    Administ    82944 Nov 15  2002 converter.exe

./include:
total 5
drwxr-xr-x    2 davem    Administ        0 Sep  6  2002 .
drwxr-xr-x    5 davem    Administ        0 May 29 20:07 ..
-rw-r--r--    1 davem    Administ     7814 May 29 20:20 krawall.h
-rw-r--r--    1 davem    Administ     1281 Jun 19  2002 mtypes.h

./lib:
total 34
drwxr-xr-x    2 davem    Administ        0 May 29 20:07 .
drwxr-xr-x    5 davem    Administ        0 May 29 20:07 ..
-rw-r--r--    1 davem    Administ     1622 Nov 15  2002 info.txt
-rw-r--r--    1 davem    Administ    30888 Nov 15  2002 libkrawall.a
-rw-r--r--    1 davem    Administ    35304 Nov 15  2002 libkrawall30.a		

These files may be obtained from the krawall Website at
http://synk.at/krawall/

libkrawall.a and libkrawall30.a are krawall.lib and krawall30.lib renamed
to the standard gcc libary naming scheme. This makes it easier to link in
a standard way.

The latest devkitARM toolchain needs krawall rebuilt in order to link but
until Sebastian gets around to doing it the change-eabi scripts provide
an interim solution.

http://www.devkitpro.org/downloads/change-eabi-scripts/

