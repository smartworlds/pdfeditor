#############################################################################
# Makefile for building: guiEva
# Generated by qmake (2.01a) (Qt 4.6.3) on: Wed Nov 17 21:32:18 2010
# Project:  guiEva.pro
# Template: app
# Command: /usr/bin/qmake -unix -o Makefile guiEva.pro
#############################################################################

####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = -DQT_NO_DEBUG -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED
CFLAGS        = -pipe -Wall -W -D_REENTRANT $(DEFINES)
CXXFLAGS      = -pipe -Wall -W -D_REENTRANT $(DEFINES)
INCPATH       = -I/usr/share/qt4/mkspecs/linux-g++ -I. -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui -I/usr/include/qt4 -I. -I. -I.
LINK          = g++
LFLAGS        = -Wl,-O1 -Wl,-rpath,/usr/lib/qt4
LIBS          = $(SUBLIBS)  -L/usr/lib/qt4 -lQtGui -L/usr/lib/qt4 -L/usr/X11R6/lib -lQtCore -lgthread-2.0 -lrt -lglib-2.0 -lpthread 
AR            = ar cqs
RANLIB        = 
QMAKE         = /usr/bin/qmake
TAR           = tar -cf
COMPRESS      = gzip -9f
COPY          = cp -f
SED           = sed
COPY_FILE     = $(COPY)
COPY_DIR      = $(COPY) -r
STRIP         = strip
INSTALL_FILE  = install -m 644 -p
INSTALL_DIR   = $(COPY_DIR)
INSTALL_PROGRAM = install -m 755 -p
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p

####### Output directory

OBJECTS_DIR   = ./

####### Files

SOURCES       = insertpagerange.cpp \
		main.cpp \
		MyWidget.cpp \
		openpdf.cpp \
		page.cpp \
		pdfgui.cpp \
		rotatepagerange.cpp \
		TabPage.cpp moc_insertpagerange.cpp \
		moc_MyWidget.cpp \
		moc_openpdf.cpp \
		moc_page.cpp \
		moc_pdfgui.cpp \
		moc_rotatepagerange.cpp \
		moc_TabPage.cpp \
		qrc_pdfgui.cpp
OBJECTS       = insertpagerange.o \
		main.o \
		MyWidget.o \
		openpdf.o \
		page.o \
		pdfgui.o \
		rotatepagerange.o \
		TabPage.o \
		moc_insertpagerange.o \
		moc_MyWidget.o \
		moc_openpdf.o \
		moc_page.o \
		moc_pdfgui.o \
		moc_rotatepagerange.o \
		moc_TabPage.o \
		qrc_pdfgui.o
DIST          = /usr/share/qt4/mkspecs/common/g++.conf \
		/usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/release.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		/usr/share/qt4/mkspecs/features/include_source_dir.prf \
		guiEva.pro
QMAKE_TARGET  = guiEva
DESTDIR       = 
TARGET        = guiEva

first: all
####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

all: Makefile $(TARGET)

$(TARGET): ui_insertpagerange.h ui_OpenPdf.h ui_page.h ui_pdfgui.h ui_rotatepagerange.h ui_showPage.h $(OBJECTS)  
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)

Makefile: guiEva.pro  /usr/share/qt4/mkspecs/linux-g++/qmake.conf /usr/share/qt4/mkspecs/common/g++.conf \
		/usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/release.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		/usr/share/qt4/mkspecs/features/include_source_dir.prf \
		/usr/lib/qt4/libQtGui.prl \
		/usr/lib/qt4/libQtCore.prl
	$(QMAKE) -unix -o Makefile guiEva.pro
/usr/share/qt4/mkspecs/common/g++.conf:
/usr/share/qt4/mkspecs/common/unix.conf:
/usr/share/qt4/mkspecs/common/linux.conf:
/usr/share/qt4/mkspecs/qconfig.pri:
/usr/share/qt4/mkspecs/features/qt_functions.prf:
/usr/share/qt4/mkspecs/features/qt_config.prf:
/usr/share/qt4/mkspecs/features/exclusive_builds.prf:
/usr/share/qt4/mkspecs/features/default_pre.prf:
/usr/share/qt4/mkspecs/features/release.prf:
/usr/share/qt4/mkspecs/features/default_post.prf:
/usr/share/qt4/mkspecs/features/warn_on.prf:
/usr/share/qt4/mkspecs/features/qt.prf:
/usr/share/qt4/mkspecs/features/unix/thread.prf:
/usr/share/qt4/mkspecs/features/moc.prf:
/usr/share/qt4/mkspecs/features/resources.prf:
/usr/share/qt4/mkspecs/features/uic.prf:
/usr/share/qt4/mkspecs/features/yacc.prf:
/usr/share/qt4/mkspecs/features/lex.prf:
/usr/share/qt4/mkspecs/features/include_source_dir.prf:
/usr/lib/qt4/libQtGui.prl:
/usr/lib/qt4/libQtCore.prl:
qmake:  FORCE
	@$(QMAKE) -unix -o Makefile guiEva.pro

dist: 
	@$(CHK_DIR_EXISTS) .tmp/guiEva1.0.0 || $(MKDIR) .tmp/guiEva1.0.0 
	$(COPY_FILE) --parents $(SOURCES) $(DIST) .tmp/guiEva1.0.0/ && $(COPY_FILE) --parents insertpagerange.h MyWidget.h openpdf.h page.h pdfgui.h rotatepagerange.h TabPage.h .tmp/guiEva1.0.0/ && $(COPY_FILE) --parents pdfgui.qrc .tmp/guiEva1.0.0/ && $(COPY_FILE) --parents insertpagerange.cpp main.cpp MyWidget.cpp openpdf.cpp page.cpp pdfgui.cpp rotatepagerange.cpp TabPage.cpp .tmp/guiEva1.0.0/ && $(COPY_FILE) --parents insertpagerange.ui OpenPdf.ui page.ui pdfgui.ui rotatepagerange.ui showPage.ui .tmp/guiEva1.0.0/ && (cd `dirname .tmp/guiEva1.0.0` && $(TAR) guiEva1.0.0.tar guiEva1.0.0 && $(COMPRESS) guiEva1.0.0.tar) && $(MOVE) `dirname .tmp/guiEva1.0.0`/guiEva1.0.0.tar.gz . && $(DEL_FILE) -r .tmp/guiEva1.0.0


clean:compiler_clean 
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) 
	-$(DEL_FILE) Makefile


check: first

mocclean: compiler_moc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_source_make_all

compiler_moc_header_make_all: moc_insertpagerange.cpp moc_MyWidget.cpp moc_openpdf.cpp moc_page.cpp moc_pdfgui.cpp moc_rotatepagerange.cpp moc_TabPage.cpp
compiler_moc_header_clean:
	-$(DEL_FILE) moc_insertpagerange.cpp moc_MyWidget.cpp moc_openpdf.cpp moc_page.cpp moc_pdfgui.cpp moc_rotatepagerange.cpp moc_TabPage.cpp
moc_insertpagerange.cpp: ui_insertpagerange.h \
		insertpagerange.h
	/usr/bin/moc $(DEFINES) $(INCPATH) insertpagerange.h -o moc_insertpagerange.cpp

moc_MyWidget.cpp: MyWidget.h
	/usr/bin/moc $(DEFINES) $(INCPATH) MyWidget.h -o moc_MyWidget.cpp

moc_openpdf.cpp: openpdf.h
	/usr/bin/moc $(DEFINES) $(INCPATH) openpdf.h -o moc_openpdf.cpp

moc_page.cpp: ui_page.h \
		page.h
	/usr/bin/moc $(DEFINES) $(INCPATH) page.h -o moc_page.cpp

moc_pdfgui.cpp: openpdf.h \
		ui_pdfgui.h \
		pdfgui.h
	/usr/bin/moc $(DEFINES) $(INCPATH) pdfgui.h -o moc_pdfgui.cpp

moc_rotatepagerange.cpp: ui_rotatepagerange.h \
		rotatepagerange.h
	/usr/bin/moc $(DEFINES) $(INCPATH) rotatepagerange.h -o moc_rotatepagerange.cpp

moc_TabPage.cpp: ui_showPage.h \
		page.h \
		ui_page.h \
		TabPage.h
	/usr/bin/moc $(DEFINES) $(INCPATH) TabPage.h -o moc_TabPage.cpp

compiler_rcc_make_all: qrc_pdfgui.cpp
compiler_rcc_clean:
	-$(DEL_FILE) qrc_pdfgui.cpp
qrc_pdfgui.cpp: pdfgui.qrc
	/usr/bin/rcc -name pdfgui pdfgui.qrc -o qrc_pdfgui.cpp

compiler_image_collection_make_all: qmake_image_collection.cpp
compiler_image_collection_clean:
	-$(DEL_FILE) qmake_image_collection.cpp
compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_uic_make_all: ui_insertpagerange.h ui_OpenPdf.h ui_page.h ui_pdfgui.h ui_rotatepagerange.h ui_showPage.h
compiler_uic_clean:
	-$(DEL_FILE) ui_insertpagerange.h ui_OpenPdf.h ui_page.h ui_pdfgui.h ui_rotatepagerange.h ui_showPage.h
ui_insertpagerange.h: insertpagerange.ui
	/usr/bin/uic insertpagerange.ui -o ui_insertpagerange.h

ui_OpenPdf.h: OpenPdf.ui
	/usr/bin/uic OpenPdf.ui -o ui_OpenPdf.h

ui_page.h: page.ui
	/usr/bin/uic page.ui -o ui_page.h

ui_pdfgui.h: pdfgui.ui \
		openpdf.h
	/usr/bin/uic pdfgui.ui -o ui_pdfgui.h

ui_rotatepagerange.h: rotatepagerange.ui
	/usr/bin/uic rotatepagerange.ui -o ui_rotatepagerange.h

ui_showPage.h: showPage.ui \
		page.h \
		ui_page.h
	/usr/bin/uic showPage.ui -o ui_showPage.h

compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: compiler_moc_header_clean compiler_rcc_clean compiler_uic_clean 

####### Compile

insertpagerange.o: insertpagerange.cpp insertpagerange.h \
		ui_insertpagerange.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o insertpagerange.o insertpagerange.cpp

main.o: main.cpp pdfgui.h \
		openpdf.h \
		ui_pdfgui.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o main.o main.cpp

MyWidget.o: MyWidget.cpp MyWidget.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o MyWidget.o MyWidget.cpp

openpdf.o: openpdf.cpp openpdf.h \
		TabPage.h \
		ui_showPage.h \
		page.h \
		ui_page.h \
		insertpagerange.h \
		ui_insertpagerange.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o openpdf.o openpdf.cpp

page.o: page.cpp page.h \
		ui_page.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o page.o page.cpp

pdfgui.o: pdfgui.cpp pdfgui.h \
		openpdf.h \
		ui_pdfgui.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o pdfgui.o pdfgui.cpp

rotatepagerange.o: rotatepagerange.cpp rotatepagerange.h \
		ui_rotatepagerange.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o rotatepagerange.o rotatepagerange.cpp

TabPage.o: TabPage.cpp TabPage.h \
		ui_showPage.h \
		page.h \
		ui_page.h \
		insertpagerange.h \
		ui_insertpagerange.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o TabPage.o TabPage.cpp

moc_insertpagerange.o: moc_insertpagerange.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_insertpagerange.o moc_insertpagerange.cpp

moc_MyWidget.o: moc_MyWidget.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_MyWidget.o moc_MyWidget.cpp

moc_openpdf.o: moc_openpdf.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_openpdf.o moc_openpdf.cpp

moc_page.o: moc_page.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_page.o moc_page.cpp

moc_pdfgui.o: moc_pdfgui.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_pdfgui.o moc_pdfgui.cpp

moc_rotatepagerange.o: moc_rotatepagerange.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_rotatepagerange.o moc_rotatepagerange.cpp

moc_TabPage.o: moc_TabPage.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_TabPage.o moc_TabPage.cpp

qrc_pdfgui.o: qrc_pdfgui.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o qrc_pdfgui.o qrc_pdfgui.cpp

####### Install

install:   FORCE

uninstall:   FORCE

FORCE:

-include Makefile.include
