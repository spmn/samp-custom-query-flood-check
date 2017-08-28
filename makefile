# This file demonstrates how to compile the CQFC project on Linux.
#
# To compile Custom Query Flood Check do:
#
# make CQFC
#

GPP = g++
GCC = gcc
OUTDIR = "./bin/Release/"
OUTFILE = "samp-custom-query-flood.so"
COMPILE_FLAGS = -m32 -fPIC -c -O3 -w -std=c++11 -fpermissive -DLINUX -DHAVE_STDINT_H -I ./lib/
CQFC = $(COMPILE_FLAGS)

all: CQFC

clean:
	-rm -f *~ *.o *.so

CQFC: clean
	$(GPP) $(CQFC) ./lib/samp-plugin-sdk/*.cpp
	$(GPP) $(CQFC) ./src/*.cpp
	mkdir -p $(OUTDIR)
	$(GPP) -m32 -O3 -fshort-wchar -shared -static-libstdc++ -o $(OUTDIR)$(OUTFILE)  *.o