// terminaltab.h

#if !defined(_TERMINALTAB_H_)
#define _TERMINALTAB_H_ 1

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QProcess>

class Embed;

class TerminalTab : public QWidget {
Q_OBJECT

public:
explicit TerminalTab(QString program, QStringList arguments,
		     QString directory_, QStringList sendLines_,
		     QWidget *parent = 0);
	~TerminalTab();
	bool start();

private slots:
	void handleButton();

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
	bool run;
};

#endif
