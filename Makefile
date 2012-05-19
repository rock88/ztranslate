# This Makefile writed by Ant-ON
#.
# App can compile for platform:
#	PLATFORM=EZX-Z6 (default)
#	PLATFORM=EZX-Z6W
#	PLATFORM=EZX-U9
#	PLATFORM=EZX-ZN5
#	PLATFORM=EZX-V8
#	PLATFORM=EZX-E8
#	PLATFORM=EZX-VE66
#	PLATFORM=EZX-EM35
#.
# For example - compile this source for Motorla ZN5:
#.
# cd /PROJECT_FOLDER/
# make clean
# make PLATFORM=EZX-ZN5
#.

######Config######
APPNAME    := ztranslate
UPLOAD_PATH :=  /mmc/mmca1/bin/
##################

TOOLPREFIX := /opt/toolchains/motomagx/magxqt
SDKPATH := $(TOOLPREFIX)

ifeq ($(PLATFORM),)
PLATFORM = EZX-E8
endif

ifeq ($(PLATFORM),EZX-E8)
QTDIR	:=	$(SDKPATH)/qt-e8
EZXDIR	:=	$(SDKPATH)/ezx-e8
LINKLIB := -lm -lqte-mt -lezxappbase -lnapi -lnetexec
DIRECTIV := -DEZX_E8
TARGET	=       $(APPNAME)
endif

CC	    =	arm-linux-gnueabi-gcc
CXX	    =	arm-linux-gnueabi-g++
LD	    =	arm-linux-gnueabi-g++
STRIP   =   arm-linux-gnueabi-strip
CFLAGS	=	-pipe -Wall -W -O2 -fPIC -DNO_DEBUG $(INCPATH)
CXXFLAGS=	-pipe -DQWS -fno-exceptions -fno-rtti -W -O2 -DNO_DEBUG $(DIRECTIV) $(INCPATH)
INCPATH	=	-I$(QTDIR)/include -I$(EZXDIR)/include -I./../libnetexec/include
LDFLAGS	=	-Wl,-rpath-link,$(EZXDIR)/lib
LINK	=	arm-linux-gnueabi-g++
LFLAGS	=	-Wl,-rpath-link,$(EZXDIR)/lib
LIBS	=	$(SUBLIBS) -L$(EZXDIR)/lib -L$(EZXDIR2)/lib -L$(QTDIR)/lib $(LINKLIB) -Llib -L../libnetexec
MOC	    =	$(QTDIR)/bin/moc
UIC	    =	$(QTDIR)/bin/uic

TARGETS = bin/$(TARGET)

all: $(TARGETS)

OBJECTS =   src/main.o \
			src/helper.o \
			src/lngselectdlg.o \
			src/setting.o
			
OBJMOC	= 	src/moc_main.o \
			src/moc_lngselectdlg.o \
			src/moc_setting.o

$(TARGETS): $(platform_echo) $(OBJECTS) $(OBJMOC) 
	$(LINK) $(LDFLAGS) $(LDLIBS) $(LIBS) -o $(TARGETS) $(OBJECTS) $(OBJMOC)
	$(STRIP) $(TARGETS)
	
src/main.o: src/main.cpp src/main.h	
src/helper.o: src/helper.cpp src/helper.h
src/lngselectdlg.o: src/lngselectdlg.cpp src/lngselectdlg.h
src/setting.o: src/setting.cpp src/setting.h

src/moc_main.o: src/moc_main.cpp src/main.h
src/moc_main.cpp: src/main.h
	$(MOC) src/main.h -o src/moc_main.cpp
	
src/moc_lngselectdlg.o: src/moc_lngselectdlg.cpp src/lngselectdlg.h
src/moc_lngselectdlg.cpp: src/lngselectdlg.h
	$(MOC) src/lngselectdlg.h -o src/moc_lngselectdlg.cpp
	
src/moc_setting.o: src/moc_setting.cpp src/setting.h
src/moc_setting.cpp: src/setting.h
	$(MOC) src/setting.h -o src/moc_setting.cpp
	
install: $(TARGETS)
	ncftpput -u root -p "" 192.168.16.2 $(UPLOAD_PATH) $(TARGETS)

    
clean:
	$(RM) $(TARGETS) $(OBJECTS) $(OBJMOC) $(OBJMOC:%.o=%.cpp)
