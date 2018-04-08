# BSDmakefile

PROGRAM = cqterm

.PHONY: all
all:
	qmake
	make -f Makefile


.PHONY: emacs-1
emacs-1:
	make clean

.PHONY: emacs-2
emacs-2: all
	./${PROGRAM}

.PHONY: emacs-3
emacs-3:
	for f in *.h *.cpp ; \
	do \
	  [ X"$${f#moc_}" = X"$${f}" ] && \
	    clang-format50 -i "$${f}" ; \
	done


.PHONY: clean
clean:
	make -f Makefile clean
