QT += widgets

INCPATH      += /usr/local/include
INCPATH      += /usr/local/include/jsoncpp
LIBS         += -lX11 -lucl -ljsoncpp
HEADERS       = configuration.h \
                session.h \
                conlecterm.h \
                terminaltab.h \
                embed.h \
                horizontaltabs.h \
                run.xpm \
                xinterface.h
SOURCES       = configuration.cpp \
                session.cpp \
                conlecterm.cpp \
                terminaltab.cpp \
                embed.cpp \
                horizontaltabs.cpp \
                xinterface.cpp \
                main.cpp

RESOURCES    += cqterm.qrc

# install
target.path = dist
INSTALLS += target
