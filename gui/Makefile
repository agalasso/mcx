UNAME_O = $(shell uname -o)
ifeq ($(UNAME_O), Msys)
  OS := win
  WX := ~/opt/wxWidgets-2.9.3
  MCX_RES := mcx.res
  DBGVIEW := Dbgview
  EXEEXT := .exe
else ifeq ($(UNAME_O), GNU/Linux)
  OS := linux
  WX := /opt/wxWidgets-2.9.3
endif

WXCONFIG := $(WX)/bin/wx-config
CXXFLAGS := `$(WXCONFIG) --cxxflags` -fPIC -g
WXLIBS := `$(WXCONFIG) --libs --debug=yes`
TARGET := $(OS)

all: mcx$(EXEEXT)

clean:
	 rm -f mcx$(EXEEXT) *.o mcx.res

fb:
	/home/andy/Incoming/wxformbuilder/trunk/output/bin/wxformbuilder MCXx.fbp

mcxgui.o: mcxgui.cpp
	`$(WXCONFIG) --cxx` -c $(CXXFLAGS) -o $@ $<

main.o: main.cpp
	`$(WXCONFIG) --cxx` -c $(CXXFLAGS) -o $@ $<

crc16.o: crc16.cpp
	`$(WXCONFIG) --cxx` -c $(CXXFLAGS) -o $@ $<

mcxcomm.o: mcxcomm.cpp
	`$(WXCONFIG) --cxx` -c $(CXXFLAGS) -o $@ $<

mcxcomm_$(TARGET).o: mcxcomm_$(TARGET).cpp
	`$(WXCONFIG) --cxx` -c $(CXXFLAGS) -o $@ $<

mcx.res: mcx.rc mcx.exe.manifest
	windres --input $< --output $@ --output-format=coff

mcx$(EXEEXT): main.o mcxgui.o crc16.o mcxcomm.o mcxcomm_$(TARGET).o $(MCX_RES)
	`$(WXCONFIG) --ld` $@ $^ $(WXLIBS)

run: mcx$(EXEEXT)
	LD_LIBRARY_PATH=$(WX)/lib ./mcx$(EXEEXT)
