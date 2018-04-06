// conlecterm.h

#if !defined(_CONLECTERM_H_)
#define _CONLECTERM_H_ 1

#include <QTabWidget>

#include "configuration.h"
#include "session.h"

class Conlecterm : public QWidget {
	Q_OBJECT

public:
	explicit Conlecterm(Configuration *conf, Session *sess, QWidget *parent = 0);
	~Conlecterm();

public slots:
	void showContextMenu(const QPoint &pos);

private:
	QTabWidget *tabs;
	Configuration *configuration;
	Session *session;
};

#endif
