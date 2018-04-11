// embed.cpp

#include <QKeyEvent>
#include <QString>
#include <QWindow>

#include "embed.h"
#include "xinterface.h"


Embed::Embed() : QWindow() {
	childId = 0;
}


Embed::~Embed() {
	end();
}


void Embed::begin() {
	childId = getChildIdFrom(winId());
}


void Embed::end() {
	childId = 0;
}


void Embed::resize(const QSize &newSize) {
	QWindow::resize(newSize);
	if (childId > 0) {
		auto w = newSize.width();
		auto h = newSize.height();
		resizeChild(childId, w, h);
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
