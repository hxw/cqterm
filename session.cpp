// session.cpp

#include <QString>
#include <QStringList>

#include <fstream>

#include <json/json.h>

#include "session.h"


Session::Session(QString filename) : sessionFile(filename) {
}

QStringList Session::load() {
	QStringList l;
	std::ifstream cfg(sessionFile.toStdString());
	if (!cfg) {
		//error = "missing session file";
		return l;
	}

	Json::Value root;
	cfg >> root;
	cfg.close();

	if (root.isNull()) {
		//error = "invalid JSON session file";
		return l;
	}

	auto s = root["session"];
	if (s.isArray()) {
		for (unsigned int index = 0; index < s.size(); ++index) {
			l << QString::fromStdString(s[index].asString());
		}
	} else {
		// error = "not array";
	}

	return l;
}

void Session::save(QStringList items) {
	Json::Value root;

	for (auto &s : items) {
		root["session"].append(s.toStdString());
	}

	std::ofstream cfg(sessionFile.toStdString());
	if (!cfg) {
		//error = "missing session file";
		return;
	}
	cfg << root;
	cfg.close();
}
