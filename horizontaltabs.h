// horizontaltabs.h

#if !defined(_HORIZONTALTABS_H_)
#define _HORIZONTALTABS_H_ 1

#include <QTabBar>
#include <QTabWidget>

class HorizontalTabBar : public QTabBar {
	Q_OBJECT

public:
	explicit HorizontalTabBar(QWidget *parent = 0);

protected:
	void paintEvent(QPaintEvent *ev);
	QSize tabSizeHint(int n) const;
};


class HorizontalTabWidget : public QTabWidget {
	Q_OBJECT

public:
	explicit HorizontalTabWidget(QWidget *parent = 0);
};

#endif
