// horizontaltabs.cpp    -*- mode: c++ -*-
// code extracted from:
//   https://stackoverflow.com/questions/4802079/how-to-change-text-alignment-in-qtabwidget-in-c
// but paint corrected to prevent crash when stylesheets was added

#include <QStyle>
#include <QStyleOption>
#include <QStylePainter>
#include <QTabBar>
#include <QTabWidget>

#include "horizontaltabs.h"

static const int defaultTabWidth = 140;
static const int defaultTabHeight = 24;
static const int defaultHMargin = 4;
static const int defaultVMargin = 2;
static const int defaultIconWidth = 10;
static const int defaultIconHeight = 10;
static const int defaultFontPoints = 12;


HorizontalTabBar::HorizontalTabBar(QWidget *parent, int _tabWidth, int _tabHeight, int _hMargin, int _vMargin,
				   int _iconWidth, int _iconHeight) :
		QTabBar(parent),
		tabWidth(_tabWidth), tabHeight(_tabHeight), hMargin(_hMargin), vMargin(_vMargin), iconWidth(_iconWidth),
		iconHeight(_iconHeight) {

        if (0 == tabWidth) {
                tabWidth = defaultTabWidth;
        }
        if (0 == tabHeight) {
                tabHeight = defaultTabHeight;
        }
        if (0 == hMargin) {
                hMargin = defaultHMargin;
        }
        if (0 == vMargin) {
                vMargin = defaultVMargin;
        }
        if (0 == iconWidth) {
                iconWidth = defaultIconWidth;
        }
        if (0 == iconHeight) {
                iconHeight = defaultIconHeight;
        }

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


HorizontalTabWidget::HorizontalTabWidget(QWidget *parent, int tabWidth, int tabHeight, int hMargin, int vMargin,
					 int iconWidth, int iconHeight, int fontPoints) :
		QTabWidget(parent) {

        if (0 == fontPoints) {
                fontPoints = defaultFontPoints;
        }

	auto *bar = new HorizontalTabBar(parent, tabWidth, tabHeight, hMargin, vMargin, iconWidth, iconHeight);
	setTabBar(bar);
	setTabPosition(QTabWidget::West);

        setStyleSheet(
		"QTabBar {"
		"  font-size: " + QString::number(fontPoints) + "pt;"
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
	setFocusPolicy(Qt::NoFocus);
}


void HorizontalTabWidget::tabInserted(int index) {
        (void)index;
        emit tabsChanged();
}


void HorizontalTabWidget::tabRemoved(int index) {
        (void)index;
        emit tabsChanged();
}
