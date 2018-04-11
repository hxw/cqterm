// terminaltab.h

#if !defined(_TERMINALTAB_H_)
#define _TERMINALTAB_H_ 1

#include <QCoreApplication>
#include <QIcon>
#include <QKeyEvent>
#include <QProcess>
#include <QPushButton>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QWidget>

#include "embed.h"


class TerminalTab : public QWidget {
	Q_OBJECT

public:
	explicit TerminalTab(QString program, QStringList arguments, QString directory_, QStringList sendLines_,
			     QWidget *parent = 0);
	~TerminalTab();
	bool start();

private slots:
	void handleButton();

protected:
	void keyPressEvent(QKeyEvent *event);
	void keyReleaseEvent(QKeyEvent *event);

private:
	QVBoxLayout *pane;

	QWidget *startClose;

	Embed *w;
	QWidget *container;
	QProcess *proc;
	QString program;
	QStringList arguments;
	QString directory;
	QStringList sendLines;
	QTabWidget *parentTabWidget;
	QIcon runIcon;

	bool run;
};

#endif
