// embed.cpp             -*- mode: c++ -*-

#include <QKeyEvent>
#include <QString>
#include <QWindow>
#include <unistd.h> // usleep()

#include "embed.h"
#include "xinterface.h"


Embed::Embed() : QWindow() {
	childId = 0;
        wantResize = false;
}


Embed::~Embed() {
	end();
}


void Embed::begin() {
        for (auto i = 0 ; i < 100 ; ++i) {
		childId = getChildIdFrom(winId());
                if (childId > 0) {
                        break;
                }
		usleep(1000);
	}
	if (wantResize && childId > 0) {
		resizeChild(childId, width, height);
		wantResize = false;
        }
}


void Embed::end() {
	childId = 0;
        wantResize = false;
}


void Embed::resize(const QSize &newSize) {
	QWindow::resize(newSize);
	width = newSize.width();
	height = newSize.height();
	if (childId > 0) {
		resizeChild(childId, width, height);
                wantResize = false;
	} else {
                wantResize = true;
        }
}


void Embed::sendKeys(QString s) {
	sendKeysToChild(childId, s.toStdString());
}


void Embed::sendKey(bool press, QKeyEvent *event) {
	sendKeyCode(childId, press, event->nativeModifiers(), event->nativeScanCode());
}


void Embed::resizeEvent(QResizeEvent *ev) {
	auto s = ev->size();
	QWindow::resizeEvent(ev);
	resize(s);
}
