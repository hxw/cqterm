// horizontaltabs.cpp
// code extracted from:
//   https://stackoverflow.com/questions/4802079/how-to-change-text-alignment-in-qtabwidget-in-c
// but paint corrected to prevent crash when stylesheets was added

#include <QStyle>
#include <QStyleOption>
#include <QStylePainter>
#include <QTabBar>
#include <QTabWidget>

#include "horizontaltabs.h"

static const int tabWidth = 140;
static const int tabHeight = 24;
static const int hMargin = 4;
static const int vMargin = 2;
static const int iconWidth = 10;
static const int iconHeight = 10;

HorizontalTabBar::HorizontalTabBar(QWidget *parent) : QTabBar(parent) {
	setIconSize(QSize(iconWidth, iconHeight));
	setFocusPolicy(Qt::NoFocus);
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

		QPainter painter;
		painter.begin(this);

		p.drawControl(QStyle::CE_TabBarTab, tab);

		QRect tabrect = tabRect(index);

		tabrect.adjust(hMargin + iconWidth, vMargin, -hMargin, -vMargin);
		painter.drawText(tabrect, Qt::AlignTop | Qt::AlignLeft, tempText);

		tempIcon.paint(&painter, hMargin, tabrect.top() + vMargin, tab.iconSize.width(), tab.iconSize.height(),
			       Qt::AlignTop | Qt::AlignHCenter);

		painter.end();
	}
}


HorizontalTabWidget::HorizontalTabWidget(QWidget *parent) : QTabWidget(parent) {
	auto *bar = new HorizontalTabBar();
	setTabBar(bar);
	setTabPosition(QTabWidget::West);
	setStyleSheet(
		"QTabBar {"
		"  font-size: 12pt;"
		"  font-weight: bold;"
		"}"
		"QTabBar::tab {"
		"  background-color: #a6daff;"
		"  color: #000000;"
		"  font-weight: slant;"
		"}"
		"QTabBar::tab:selected {"
		"  background-color: #ffdaff;"
		"}"
		"QTabBar::tab:selected:hover {"
		"  background-color: #af7aaf;"
		"}"
		"QTabBar::tab:hover {"
		"  background-color: #00daff;"
		"}");

	connect(bar, QOverload<int, int>::of(&QTabBar::tabMoved),
		[=](int from, int to) {
                        (void)from;
                        (void)to;
			emit tabsChanged();
                });
}


void HorizontalTabWidget::tabInserted(int index) {
        (void)index;
        emit tabsChanged();
}


void HorizontalTabWidget::tabRemoved(int index) {
        (void)index;
        emit tabsChanged();
}
