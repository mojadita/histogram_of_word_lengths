# Makefile -- script to build program
# Author: Luis Colorado <luiscoloradourcola@gmail.com>
# Date: Thu Aug 25 22:55:53 EEST 2022
# Copyright: (c) 2022 Luis Colorado.  All rights reserved.
# License: BSD

targets = hwl

RM      ?= rm -f

hwl_deps =
hwl_objs = hwl.o
hwl_libs =
hwl_ldfl =

all: $(targets)
clean:
	$(RM) $(toclean)

.for t in $(targets)
toclean += $t $($t_objs)
$t: $($t_deps) $($t_objs)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $($@_ldfl) $($@_objs) $($@_libs) $(LIBS)
.endfor
