# Makefile for GNU Ocrad - Optical Character Recognition program
# Copyright (C) 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010, 2011,
# 2012, 2013, 2014 Antonio Diaz Diaz.
# This file was generated automatically by configure. Do not edit.
#
# This Makefile is free software: you have unlimited permission
# to copy, distribute and modify it.

pkgname = ocrad
pkgversion = 0.23
progname = ocrad
libname = ocrad
VPATH = .
prefix = /usr/local
exec_prefix = $(prefix)
bindir = $(exec_prefix)/bin
datarootdir = $(prefix)/share
includedir = ${prefix}/include
infodir = $(datarootdir)/info
libdir = ${exec_prefix}/lib
mandir = $(datarootdir)/man
CXX = g++
CPPFLAGS = 
CXXFLAGS = -Wall -W -O2
LDFLAGS = 

DISTNAME = $(pkgname)-$(pkgversion)
AR = ar
INSTALL = install
INSTALL_PROGRAM = $(INSTALL) -m 755
INSTALL_DATA = $(INSTALL) -m 644
INSTALL_DIR = $(INSTALL) -d -m 755
SHELL = /bin/sh

lib_objs = ocradlib.o
ocr_objs = common.o segment.o mask.o rational.o rectangle.o track.o \
           ucs.o page_image.o page_image_io.o \
           bitmap.o blob.o profile.o feats.o feats_test0.o feats_test1.o \
           character.o character_r11.o character_r12.o character_r13.o \
           textline.o textline_r2.o textblock.o textpage.o
objs     = arg_parser.o main.o


.PHONY : all install install-bin install-info install-man install-strip \
         uninstall uninstall-bin uninstall-info uninstall-man \
         doc info man check dist clean distclean

all : $(progname) lib$(libname).a

lib$(libname).a: $(ocr_objs) $(lib_objs)
	$(AR) -rcs $@ $(ocr_objs) $(lib_objs)

$(progname) : $(ocr_objs) $(objs)
	$(CXX) $(LDFLAGS) -o $@ $(ocr_objs) $(objs)

$(progname)_profiled : $(ocr_objs) $(objs)
	$(CXX) $(LDFLAGS) -pg -o $@ $(ocr_objs) $(objs)

ocrcheck : ocrcheck.o lib$(libname).a
	$(CXX) $(LDFLAGS) -o $@ ocrcheck.o lib$(libname).a

ocrcheck.o : ocrcheck.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -DPROGVERSION=\"$(pkgversion)\" -c -o $@ $<

main.o : main.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -DPROGVERSION=\"$(pkgversion)\" -c -o $@ $<

%.o : %.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

$(lib_objs)     : Makefile ocradlib.h
$(ocr_objs)     : Makefile bitmap.h blob.h common.h rectangle.h ucs.h
$(objs)         : Makefile arg_parser.h
character.o     : character.h
character_r11.o : segment.h character.h profile.h feats.h
character_r12.o : segment.h character.h profile.h feats.h
character_r13.o : segment.h character.h profile.h feats.h
feats.o         : segment.h profile.h feats.h
feats_test0.o   : segment.h profile.h feats.h
feats_test1.o   : segment.h profile.h feats.h
main.o          : common.h rational.h rectangle.h page_image.h textpage.h
mask.o          : segment.h mask.h
ocradlib.o      : common.h rectangle.h ucs.h track.h bitmap.h blob.h character.h page_image.h textline.h textblock.h textpage.h
page_image.o    : ocradlib.h rational.h segment.h mask.h track.h page_image.h
page_image_io.o : rational.h page_image.h
profile.o       : profile.h
rational.o      : rational.h
segment.o       : segment.h
textblock.o     : rational.h track.h character.h page_image.h textline.h textblock.h
textline.o      : rational.h track.h character.h page_image.h textline.h
textline_r2.o   : track.h character.h textline.h
textpage.o      : segment.h mask.h track.h character.h page_image.h textline.h textblock.h textpage.h
track.o         : track.h
ocrcheck.o      : Makefile ocradlib.h


doc : info man

info : $(VPATH)/doc/$(pkgname).info

$(VPATH)/doc/$(pkgname).info : $(VPATH)/doc/$(pkgname).texi
	cd $(VPATH)/doc && makeinfo $(pkgname).texi

man : $(VPATH)/doc/$(progname).1

$(VPATH)/doc/$(progname).1 : $(progname)
	help2man -n 'command line text recognition tool' -o $@ ./$(progname)

Makefile : $(VPATH)/configure $(VPATH)/Makefile.in
	./config.status

check : all ocrcheck
	@$(VPATH)/testsuite/check.sh $(VPATH)/testsuite $(pkgversion)

install : install-bin install-info install-man

install-bin : all
	if [ ! -d "$(DESTDIR)$(bindir)" ] ; then $(INSTALL_DIR) "$(DESTDIR)$(bindir)" ; fi
	if [ ! -d "$(DESTDIR)$(includedir)" ] ; then $(INSTALL_DIR) "$(DESTDIR)$(includedir)" ; fi
	if [ ! -d "$(DESTDIR)$(libdir)" ] ; then $(INSTALL_DIR) "$(DESTDIR)$(libdir)" ; fi
	$(INSTALL_PROGRAM) ./$(progname) "$(DESTDIR)$(bindir)/$(progname)"
	$(INSTALL_DATA) $(VPATH)/$(libname)lib.h "$(DESTDIR)$(includedir)/$(libname)lib.h"
	$(INSTALL_DATA) ./lib$(libname).a "$(DESTDIR)$(libdir)/lib$(libname).a"

install-info :
	if [ ! -d "$(DESTDIR)$(infodir)" ] ; then $(INSTALL_DIR) "$(DESTDIR)$(infodir)" ; fi
	$(INSTALL_DATA) $(VPATH)/doc/$(pkgname).info "$(DESTDIR)$(infodir)/$(pkgname).info"
	-install-info --info-dir="$(DESTDIR)$(infodir)" "$(DESTDIR)$(infodir)/$(pkgname).info"

install-man :
	if [ ! -d "$(DESTDIR)$(mandir)/man1" ] ; then $(INSTALL_DIR) "$(DESTDIR)$(mandir)/man1" ; fi
	$(INSTALL_DATA) $(VPATH)/doc/$(progname).1 "$(DESTDIR)$(mandir)/man1/$(progname).1"

install-strip : all
	$(MAKE) INSTALL_PROGRAM='$(INSTALL_PROGRAM) -s' install

uninstall : uninstall-bin uninstall-info uninstall-man

uninstall-bin :
	-rm -f "$(DESTDIR)$(bindir)/$(progname)"
	-rm -f "$(DESTDIR)$(includedir)/$(libname)lib.h"
	-rm -f "$(DESTDIR)$(libdir)/lib$(libname).a"

uninstall-info :
	-install-info --info-dir="$(DESTDIR)$(infodir)" --remove "$(DESTDIR)$(infodir)/$(pkgname).info"
	-rm -f "$(DESTDIR)$(infodir)/$(pkgname).info"

uninstall-man :
	-rm -f "$(DESTDIR)$(mandir)/man1/$(progname).1"

dist : doc
	ln -sf $(VPATH) $(DISTNAME)
	tar -Hustar --owner=root --group=root -cvf $(DISTNAME).tar \
	  $(DISTNAME)/AUTHORS \
	  $(DISTNAME)/COPYING \
	  $(DISTNAME)/ChangeLog \
	  $(DISTNAME)/INSTALL \
	  $(DISTNAME)/Makefile.in \
	  $(DISTNAME)/NEWS \
	  $(DISTNAME)/README \
	  $(DISTNAME)/configure \
	  $(DISTNAME)/doc/$(progname).1 \
	  $(DISTNAME)/doc/$(pkgname).info \
	  $(DISTNAME)/doc/$(pkgname).texi \
	  $(DISTNAME)/testsuite/check.sh \
	  $(DISTNAME)/testsuite/test.pbm \
	  $(DISTNAME)/testsuite/test.txt \
	  $(DISTNAME)/testsuite/test_utf8.txt \
	  $(DISTNAME)/ocrad.png \
	  $(DISTNAME)/*.h \
	  $(DISTNAME)/*.cc
	rm -f $(DISTNAME)
	lzip -v -9 $(DISTNAME).tar

clean :
	-rm -f $(progname) $(progname)_profiled $(objs)
	-rm -f ocrcheck ocrcheck.o $(ocr_objs) $(lib_objs) *.a

distclean : clean
	-rm -f Makefile config.status *.tar *.tar.lz
