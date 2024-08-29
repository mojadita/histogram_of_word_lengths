# Makefile -- script to build program
# Author: Luis Colorado <luiscoloradourcola@gmail.com>
# Date: Thu Aug 25 22:55:53 EEST 2022
# Copyright: (c) 2022 Luis Colorado.  All rights reserved.
# License: BSD

targets = hwl
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

OWNR        ?= root
GROP        ?= bin
XMOD        ?= 0511

INSTALL     ?= install
IFLAGS      ?= -o $(OWNR) -g $(GROP)

install: $(targets)
	$(INSTALL) $(IFLAGS) -d $(bindir)
	$(INSTALL) $(IFLAGS) -m $(XMOD) $(targets) $(bindir)

uninstall:
	$(RM) $(patsubst %, $(bindir)/%, $(targets))

hwl: $(hwl_deps) $(hwl_objs)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $($@_ldfl) $($@_objs) $($@_libs) $(LIBS)
