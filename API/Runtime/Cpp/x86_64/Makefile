ROOT=../
ARCH=x86_64

CC=c++
CFLAGS=-c -Wall -I$(ROOT)/../C

SYSTEM=linux

AR=ar
ARFLAGS=-rc

RM=rm -f


libhasp_cpp_$(SYSTEM)_$(ARCH).a:
	$(CC) $(ROOT)*.cpp $(CFLAGS)
	$(AR) $(ARFLAGS) $@ *.o
	
clean:
	$(RM) *.o libhasp_cpp_$(SYSTEM)_$(ARCH).a
