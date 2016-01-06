# This file demonstrates how to compile the fcnpc project on Linux.
#
# To compile FCNPC do:
#
# make FCNPC
#

GPP = g++
GCC = gcc
FCNPC_OUTFILE = "Bin/FCNPC.so"

COMPILE_FLAGS = -m32 -fPIC -c -O2 -w -D LINUX -D PROJECT_NAME=\"FCNPC\" -I ./FCNPC/SDK/amx/

FCNPC = -D FCNPC $(COMPILE_FLAGS)

all: FCNPC

clean:
	-rm -f *~ *.o *.so

FCNPC: clean
	$(GPP) $(FCNPC) ./FCNPC/SDK/amx/*.c
	$(GPP) $(FCNPC) ./FCNPC/SDK/*.cpp
	$(GCC) $(FCNPC) ./FCNPC*.c
	$(GPP) $(FCNPC) ./FCNPC*.cpp
	$(GPP) -m32 -O2 -fshort-wchar -shared -o $(FCNPC_OUTFILE) *.o
	rm *.o
