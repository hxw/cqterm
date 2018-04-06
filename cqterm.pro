QT += widgets

INCPATH      += /usr/local/include
INCPATH      += /usr/local/include/jsoncpp
LIBS         += -lX11 -lucl -ljsoncpp
HEADERS       = configuration.h \
                session.h \
                conlecterm.h \
                terminaltab.h \
                horizontaltabs.h \
                xinterface.h
SOURCES       = configuration.cpp \
                session.cpp \
                conlecterm.cpp \
                terminaltab.cpp \
                horizontaltabs.cpp \
                xinterface.cpp \
                main.cpp

# install
target.path = dist
INSTALLS += target
