// horizontaltabs.cpp
// code extracted from:
//   https://stackoverflow.com/questions/4802079/how-to-change-text-alignment-in-qtabwidget-in-c

#include <QTabBar>
#include <QTabWidget>
#include <QStyle>
#include <QStylePainter>
#include <QStyleOption>

#include "horizontaltabs.h"

static const int tabWidth = 80;
static const int tabHeight = 20;
static const int hMargin = 4;
static const int vMargin = 2;

HorizontalTabBar::HorizontalTabBar(QWidget *parent) : QTabBar(parent) {
	setIconSize(QSize(0, 0));
}

QSize HorizontalTabBar::tabSizeHint(int) const {
	return QSize(tabWidth, tabHeight);
}

void HorizontalTabBar::paintEvent(QPaintEvent *) {
	QStylePainter p(this);
	for (auto index = 0; index < count(); index++) {
		QStyleOptionTab tab;
		initStyleOption(&tab, index);

		QIcon tempIcon = tab.icon;
		QString tempText = tab.text;
		tab.icon = QIcon();
		tab.text = QString();

		p.drawControl(QStyle::CE_TabBarTab, tab);

		QPainter painter;
		painter.begin(this);
		QRect tabrect = tabRect(index);

		tabrect.adjust(hMargin, vMargin, -hMargin, -vMargin);
		painter.drawText(tabrect, Qt::AlignTop | Qt::AlignLeft, tempText);
		tempIcon.paint(&painter, 0, tabrect.top(), tab.iconSize.width(), tab.iconSize.height(), Qt::AlignTop | Qt::AlignHCenter);
		painter.end();
	}
}


HorizontalTabWidget::HorizontalTabWidget(QWidget *parent) : QTabWidget(parent) {
	setTabBar(new HorizontalTabBar());
	setTabPosition(QTabWidget::West);
}
