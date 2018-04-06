# BSDmakefile

PROGRAM = cqterm

.PHONY: all
all:
	qmake
	make -f Makefile || exit 1


.PHONY: emacs-1
emacs-1:
	make clean

.PHONY: emacs-2
emacs-2: all
	./${PROGRAM} -hello -conf foobar --config=asdfg


.PHONY: clean
clean:
	make -f Makefile clean
