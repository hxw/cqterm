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

# utilities
RM = rm -f
SAY = echo
SED = sed
MKDIR = mkdir -p
INSTALL_PROGRAM ?= install -C -s -m 0755
INSTALL_DATA  ?= install -C -m 0644

# installation directory
INSTALL_DIR ?= ${HOME}/bin
APPLICATIONS_DIR ?= ${HOME}/.local/share/applications
PROG_DESKTOP ?= ${PROGRAM}.desktop
CONF_DIR ?= ${XDG_CONFIG_HOME}/${PROGRAM}

# install the application binary
.PHONY: install pre-install do-install post-install
install: pre-install do-install post-install
pre-install:
do-install:
	@if [ ! -d '${INSTALL_DIR}' ]; then ${SAY} 'missing directory: ${INSTALL_DIR}'; false; fi
	@${INSTALL_PROGRAM} -v '${PROGRAM}' '${INSTALL_DIR}/${PROGRAM}'
	@if [ ! -d '${APPLICATIONS_DIR}' ]; then ${SAY} 'missing  directory: ${APPLICATIONS_DIR}'; false; fi
	@${INSTALL_DATA} -v '${PROG_DESKTOP}' '${APPLICATIONS_DIR}/${PROG_DESKTOP}'
	@if [ -d '${XDG_CONFIG_HOME}' ] ; \
	 then \
	   if [ ! -d '${CONF_DIR}' ] ; \
	   then \
	     ${SAY} 'creating  directory: ${CONF_DIR}' ; \
	     ${MKDIR} '${CONF_DIR}' ; \
	     ${INSTALL_DATA} -v '${PROGRAM}.conf' '${CONF_DIR}' ; \
	     ${INSTALL_DATA} -v 'session.json' '${CONF_DIR}' ; \
	   fi ; \
	 fi

post-install:
