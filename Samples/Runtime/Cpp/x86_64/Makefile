ROOT=..
SYSTEM=linux
ARCH=_x86_64
CPP=c++
CPPFLAGS=-c -I$(ROOT)

HASP_C_DIR=$(ROOT)/../../../API/Runtime/C/x86_64
HASP_CPP_DIR=$(ROOT)/../../../API/Runtime/Cpp/x86_64
HASP_CPP_LIB=libhasp_cpp_$(SYSTEM)$(ARCH).a

LFLAGS=-L$(ROOT) -L$(HASP_CPP_DIR) 
LINPUT=-lhasp_cpp_$(SYSTEM)$(ARCH) $(HASP_C_DIR)/libhasp_$(SYSTEM)$(ARCH)_demo.a -lpthread
RM=rm -f

all: hasp_update hasp_demo
	$(NULL)

errorprinter.o : $(ROOT)/errorprinter.cpp $(ROOT)/errorprinter.h
	$(CPP) $(CPPFLAGS) $(ROOT)/errorprinter.cpp -o $@

hasp_demo.o : $(ROOT)/hasp_demo.cpp
	$(CPP) $(CPPFLAGS) $(ROOT)/hasp_demo.cpp -o $@

hasp_update.o : $(ROOT)/hasp_update.cpp
	$(CPP) $(CPPFLAGS) $(ROOT)/hasp_update.cpp -o $@

hasp_demo : errorprinter.o hasp_demo.o $(HASP_CPP_DIR)/$(HASP_CPP_LIB)
	$(CPP) errorprinter.o hasp_demo.o $(LFLAGS) $(LINPUT) -o $@
	
hasp_update : errorprinter.o hasp_update.o  $(HASP_CPP_DIR)/$(HASP_CPP_LIB)
	$(CPP) errorprinter.o hasp_update.o $(LFLAGS) $(LINPUT) -o $@
	
$(HASP_CPP_DIR)/$(HASP_CPP_LIB):
	$(MAKE) -C $(HASP_CPP_DIR)

clean:
	$(RM) errorprinter.o hasp_demo.o hasp_update.o
	$(RM) hasp_demo hasp_update

