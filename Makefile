# Makefile -- script to build program
# Author: Luis Colorado <luiscoloradourcola@gmail.com>
# Date: Thu Aug 25 22:55:53 EEST 2022
# Copyright: (c) 2022 Luis Colorado.  All rights reserved.
# License: BSD

targets = hwl
binmans = hwl.1
toclean += $(targets)

RM      ?= rm -f

hwl_deps =
hwl_objs = hwl.o
hwl_libs =
hwl_ldfl =
toclean += $(hwl_objs)

all: $(targets)
clean:
	$(RM) $(toclean)

prefix      ?= /usr/local
exec_prefix ?= $(prefix)
bindir      ?= $(exec_prefix)/bin
datarootdir ?= $(prefix)/share
mandir      ?= $(datarootdir)/man
man1dir     ?= $(mandir)/man1


OWNR        ?= root
GROP        ?= bin
XMOD        ?= 0511
FMOD        ?= 0544

INSTALL     ?= install
IFLAGS      ?= -o $(OWNR) -g $(GROP)

install: $(targets)
	$(INSTALL) $(IFLAGS) -d $(bindir)
	$(INSTALL) $(IFLAGS) -m $(XMOD) $(targets) $(bindir)
	$(INSTALL) $(IFLAGS) -m $(FMOD) $(binmans) $(man1dir)

uninstall:
	$(RM) $(patsubst %, $(bindir)/%, $(targets))

hwl: $(hwl_deps) $(hwl_objs)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $($@_ldfl) $($@_objs) $($@_libs) $(LIBS)

hwl.1.pdf: hwl.1
	groff -Tpdf -mandoc $< > $@
