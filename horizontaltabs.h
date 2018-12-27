// horizontaltabs.h      -*- mode: c++ -*-

#if !defined(_HORIZONTALTABS_H_)
#define _HORIZONTALTABS_H_ 1

#include <QTabBar>
#include <QTabWidget>


class HorizontalTabBar : public QTabBar {
	Q_OBJECT

public:
	explicit HorizontalTabBar(QWidget *parent = 0, int _tabWidth = 0,
				  int _tabHeight = 0, int _hMargin = 0,
				  int _vMargin = 0, int _iconWidth = 0,
				  int _iconHeight = 0);

protected:
	void paintEvent(QPaintEvent *ev);
	QSize tabSizeHint(int n) const;

	// make Tab a normal key
	bool focusNextPrevChild(bool next) {
                (void)(next);
                return false;
        }

private:
	int tabWidth;
	int tabHeight;
	int hMargin;
	int vMargin;
	int iconWidth;
	int iconHeight;
};


class HorizontalTabWidget : public QTabWidget {
	Q_OBJECT

public:
	explicit HorizontalTabWidget(QWidget *parent = 0, int tabWidth = 0,
				     int tabHeight = 0, int hMargin = 0,
				     int vMargin = 0, int iconWidth = 0,
				     int iconHeight = 0, int fontPoints = 0);

protected:
	// make Tab a normal key
	bool focusNextPrevChild(bool next) {
                (void)(next);
                return false;
        }


signals:
        void tabsChanged();

protected:
	void tabInserted(int index);
	void tabRemoved(int index);
};

#endif
