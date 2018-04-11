// conlecterm.h

#if !defined(_CONLECTERM_H_)
#define _CONLECTERM_H_ 1

#include <QCloseEvent>
#include <QEvent>
#include <QPoint>
#include <QTabWidget>
#include <QTimer>

#include "configuration.h"
#include "session.h"
#include "horizontaltabs.h"

class Conlecterm : public QWidget {
	Q_OBJECT

public:
	explicit Conlecterm(Configuration *conf, Session *sess, QWidget *parent = 0);
	~Conlecterm();

public slots:
	void showContextMenu(const QPoint &pos);

private slots:
	void save();

protected:
	void closeEvent(QCloseEvent *event);
	bool event(QEvent *event);

private:
	HorizontalTabWidget *tabs;
	Configuration *configuration;
	Session *session;
        bool saveChanges;
        QTimer *timer;
};

#endif
