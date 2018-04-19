// main.cpp              -*- mode: c++ -*-

#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include <QString>

#include <cstdlib>

#include "configuration.h"
#include "conlecterm.h"
#include "session.h"

static bool defaultConfig(QString filename, QString sessionFilename);
static QString getPathFor(QString filename);


static const QString configurationFile("cqterm.conf");
static const QString sessionFile("session.json");

int main(int argc, char *argv[]) {
	QApplication app(argc, argv);
	// auto arguments = app.arguments();
	// for (auto index = 0; index < arguments.count(); index++) {
	// 	qDebug() << index << ": " << arguments.at(index);
	// }

        auto configurationPath = getPathFor(configurationFile);
        auto sessionPath = getPathFor(sessionFile);

	if (!defaultConfig(configurationPath, sessionPath)) {
                return 1;
        }

	Session session(sessionPath);

	Configuration c(configurationPath);
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


static bool defaultConfig(QString filename, QString sessionFilename) {
        QFileInfo f(filename);
        if (f.exists()) {
                return true;
        }
        auto dir = f.dir();

        if (!dir.exists()) {
		QMessageBox msgBox;
		msgBox.setText("ERROR: missing directory: " + dir.path());
		msgBox.setInformativeText("Create directory and install default configuration");
		msgBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);
		msgBox.setDefaultButton(QMessageBox::Cancel);
		auto rc = msgBox.exec();
		if (QMessageBox::Ok != rc) {
			return false;
                }
		if (!dir.mkpath(".")) {
			QMessageBox msgBox;
			msgBox.setText("ERROR: cannot create directory: " + dir.path());
			msgBox.setInformativeText("Failed to create directory");
			msgBox.setStandardButtons(QMessageBox::Cancel);
			msgBox.setDefaultButton(QMessageBox::Cancel);
			msgBox.exec();
                        return false;
                }
	} else {
		QMessageBox msgBox;
		msgBox.setText("ERROR: missing file: " + filename);
		msgBox.setInformativeText("Install default configuration in this file");
		msgBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);
		msgBox.setDefaultButton(QMessageBox::Cancel);
		auto rc = msgBox.exec();
		if (QMessageBox::Ok != rc) {
			return false;
                }
        }
        if (!QFile::copy(":/default.conf", filename)) {
		QMessageBox msgBox;
		msgBox.setText("ERROR: cannot create file: " + filename);
		msgBox.setInformativeText("Failed to create file");
		msgBox.setStandardButtons(QMessageBox::Cancel);
		msgBox.setDefaultButton(QMessageBox::Cancel);
		msgBox.exec();
		return false;
        }
        auto p = QFileDevice::ReadOwner | QFileDevice::WriteOwner;
        QFile(filename).setPermissions(p);

        QFile::copy(":/default.session", sessionFilename);
        QFile(sessionFilename).setPermissions(p);

        return true;
}


static QString getPathFor(QString filename) {
	QString configHome(std::getenv("XDG_CONFIG_HOME"));
	if (configHome.isEmpty()) {
		configHome = QDir::homePath() + QDir::separator() + ".config";
	}
	return configHome + QDir::separator() + "cqterm" + QDir::separator() + filename;
}
