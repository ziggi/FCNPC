# This file demonstrates how to compile the fcnpc project on Linux.
#
# To compile FCNPC do:
#
# make FCNPC
#

GCC = gcc
GPP = g++
FCNPC_OUTFILE = "LinuxBin/FCNPC.so"

COMPILE_FLAGS = -m32 -fPIC -c -O2 -w -D LINUX -D PROJECT_NAME=\"FCNPC\" -I ./FCNPC/SDK/amx/

FCNPC = -D FCNPC $(COMPILE_FLAGS)

all: FCNPC

clean:
	-rm -f *~ *.o *.so

FCNPC: clean
	$(GCC) $(FCNPC) ./FCNPC/subhook/{subhook,subhook_linux,subhook_x86}.c
	$(GPP) $(FCNPC) ./FCNPC/SDK/amx/*.c
	$(GPP) $(FCNPC) ./FCNPC/SDK/*.cpp
	$(GPP) $(FCNPC) ./FCNPC/*.cpp
	$(GPP) -m32 -O2 -fshort-wchar -shared -o $(FCNPC_OUTFILE) *.o
	rm *.o
