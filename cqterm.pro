# cqterm.pro

QT += widgets

QMAKE_CXXFLAGS += -std=c++2a
QMAKE_CXXFLAGS += -fsanitize=address
QMAKE_CXXFLAGS += -fno-omit-frame-pointer
QMAKE_CXXFLAGS += -Wshadow-uncaptured-local
#QMAKE_CXXFLAGS += -Wshadow-compatible-local ## ¿clang 8+?

QMAKE_LFLAGS += -fsanitize=address

INCPATH      += /usr/local/include
INCPATH      += /usr/local/include/jsoncpp
LIBS         += -lX11 -lucl -ljsoncpp
HEADERS       = configuration.h \
                session.h \
                conlecterm.h \
                terminaltab.h \
                embed.h \
                horizontaltabs.h \
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
isEmpty(PREFIX) {
    PREFIX    = /usr/local
}
isEmpty(BIN_DIR) {
    BIN_DIR   = $${PREFIX}/bin
}
isEmpty(SHARE_DIR) {
    SHARE_DIR = $${PREFIX}/share
}
isEmpty(EXAMPLES_DIR) {
    EXAMPLES_DIR = $${SHARE_DIR}/examples/cqterm
}
isEmpty(DESKTOP_DIR) {
    DESKTOP_DIR = $${SHARE_DIR}/applications
}
isEmpty(ICONS_DIR) {
    ICONS_DIR = $${SHARE_DIR}/icons/hicolor/scalable/apps
}

target.path   = $${BIN_DIR}
target.files  = cqterm

icons.path    = $${ICONS_DIR}
icons.files   = cqterm.svg run.svg

conf.path     = $${EXAMPLES_DIR}
conf.files    = cqterm.conf session.json

desktop.path  = $${DESKTOP_DIR}
desktop.files = cqterm.desktop

INSTALLS     += target icons conf desktop
