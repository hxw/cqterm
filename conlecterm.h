// conlecterm.h          -*- mode: c++ -*-

#if !defined(_CONLECTERM_H_)
#define _CONLECTERM_H_ 1

#include <QCloseEvent>
#include <QEvent>
#include <QMainWindow>
#include <QPoint>
#include <QTabWidget>
#include <QTimer>

#include "configuration.h"
#include "session.h"
#include "horizontaltabs.h"

class Conlecterm : public QMainWindow {
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

	void keyPressEvent(QKeyEvent *event) {
                (void)(event);
        }
	void keyReleaseEvent(QKeyEvent *event) {
                (void)(event);
        }

	// make Tab a normal key
	bool focusNextPrevChild(bool next) {
                (void)(next);
                return false;
        }

private:
	QWidget *cq;
	HorizontalTabWidget *tabs;
	Configuration *configuration;
	Session *session;
        bool saveChanges;
        QTimer *timer;
};

#endif
