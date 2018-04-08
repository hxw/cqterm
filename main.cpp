// main.cpp

#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QMessageBox>
#include <QString>

#include <cstdlib>

#include "configuration.h"
#include "conlecterm.h"
#include "session.h"

static QString getPathFor(QString filename);


static const QString configurationFile("cqterm.conf");
static const QString sessionFile("session.json");

int main(int argc, char *argv[]) {
	QApplication app(argc, argv);
	// auto arguments = app.arguments();
	// for (auto index = 0; index < arguments.count(); index++) {
	// 	qDebug() << index << ": " << arguments.at(index);
	// }

	Session session(getPathFor(sessionFile));

	Configuration c(getPathFor(configurationFile));
	if (!c.isValid()) {
		qDebug() << "error: " << c.getError();
		QMessageBox msgBox;
		msgBox.setText("ERROR: reading configuration: " + getPathFor(configurationFile));
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

static QString getPathFor(QString filename) {
	QString configHome(std::getenv("XDG_CONFIG_HOME"));
	if (configHome.isEmpty()) {
		configHome = QDir::homePath() + QDir::separator() + ".config";
	}
	return configHome + QDir::separator() + "cqterm" + QDir::separator() + filename;
}
