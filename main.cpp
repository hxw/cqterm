// main.cpp

#include <QApplication>
#include <QMessageBox>
#include <QString>
#include <QDebug>

#include "conlecterm.h"
#include "configuration.h"
#include "session.h"

static const QString configurationFile("cqterm.conf");
static const QString sessionFile("session.json");

int main(int argc, char *argv[]) {
	QApplication app(argc, argv);
	// auto arguments = app.arguments();
	// for (auto index = 0; index < arguments.count(); index++) {
	// 	qDebug() << index << ": " << arguments.at(index);
	// }

	Session session(sessionFile);

	Configuration c(configurationFile);
	if (!c.isValid()) {
		qDebug() << "error: " << c.getError();
		QMessageBox msgBox;
		msgBox.setText("ERROR: reading configuration: " + configurationFile);
		msgBox.setInformativeText(c.getError());
		msgBox.setStandardButtons(QMessageBox::Cancel);
		msgBox.setDefaultButton(QMessageBox::Cancel);
		msgBox.exec();
		return 1;
	}

	Conlecterm cq(&c, &session);
	cq.show();
	return app.exec();
}
