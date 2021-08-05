# Modified the makefile of an old DriftPointsCounter version, as I got
# no idea how to write one myself.

GPP = g++
GCC = gcc
RNPC_OUTFILE = "RNPC.so"

COMPILE_FLAGS = -m32 -fPIC -c -O3 -w -D LINUX -D PROJECT_NAME=\"RNPC\" -I ./SDK/amx/

RNPC21 = -D RNPC21 $(COMPILE_FLAGS)

all: RNPC

clean:
	-rm -f *~ *.o *.so

RNPC: clean
	$(GPP) $(RNPC21) ./SDK/amx/*.c
	$(GPP) $(RNPC21) ./SDK/*.cpp
	$(GPP) $(RNPC21) ./ThirdPartySource/MapAndreas.cpp
	
	$(GPP) $(RNPC21) *.cpp
	$(GPP) -m32 -O2 -fshort-wchar -shared -o $(RNPC_OUTFILE) *.o
