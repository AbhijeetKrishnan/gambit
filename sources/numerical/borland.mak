#
# $Source$
# $Date$
# $Revision$
#
# DESCRIPTION:
# Makefile for Borland C++ 5.5 for numerical module
#

.AUTODEPEND

!include ..\makedef.bcc

EXTRACPPFLAGS = -v -I$(BCCDIR)\include -I.. -D__BCC55__
EXTRALINKFLAGS = -Tpe -aa -v -V4.0 -c

libnumerical_a_SOURCES = \
	basis.cc \
	bfs.cc \
	btableau.cc \
	gfunc.cc \
	gfuncmin.cc \
	lemketab.cc \
	linrcomb.cc \
	lpsolve.cc \
	lptab.cc \
	ludecomp.cc \
	tableau.cc \
	vertenum.cc

OBJECTS = $(libnumerical_a_SOURCES:.cc=.obj)

CFG = ..\gambit32.cfg

OPT = -Od

.cc.obj:
	bcc32 $(CPPFLAGS) -P -c {$< }

LINKFLAGS= /Tpe /L$(WXLIBDIR);$(BCCDIR)\lib $(EXTRALINKFLAGS)
OPT = -Od
DEBUG_FLAGS= -v


CPPFLAGS= $(WXINC) $(EXTRACPPFLAGS) $(OPT) @$(CFG)

numerical: $(OBJECTS)
        copy *.obj ..

clean:
        -erase *.obj
        -erase *.exe
        -erase *.res
        -erase *.map
        -erase *.rws

