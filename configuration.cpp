// configuration.cpp     -*- mode: c++ -*-

#include <QDir>
#include <QMap>
#include <QString>
#include <QStringList>

#include <fstream>

#include <ucl++.h>

#include "configuration.h"


Configuration::Configuration(QString filename) {
	valid = false;
	std::ifstream cfg(filename.toStdString());
	if (!cfg) {
		error = "missing configuration file";
		return;
	}

	std::string uclError;
	auto conf = ucl::Ucl::parse(cfg, uclError); // parse_from_file("configuration", &uclError); <- in next version
	cfg.close();

	if (!cfg) {
		error = "configuration parse error: " + QString::fromStdString(uclError);
		return;
	}


	// for (const auto &o : conf) {
	// 	auto k = QString::fromStdString(o.key());
	// 	std::cout << o.key() << std::endl;
	// 	for (const auto &o2 : o) {
	// 		std::cout << o2.key() << std::endl;
	// 		std::cout << o2.dump(UCL_EMIT_CONFIG) << std::endl;
	// 	}
	// }

	QMap<QString, Command *> cmdMap;
	static std::string cmd("commands");
	static std::string program("program");
	static std::string option("option");
	auto commands = conf[cmd];
	if (commands) {
		for (const auto &c : commands) {
			auto key = QString::fromStdString(c.key());
			if (!c[program]) {
				error = "command: " + key + " missing program";
				return;
			}
			auto cm = new Command;
			cm->program = QString::fromStdString(c[program].string_value());
			if (c[option]) {
				for (const auto &o : c[option]) {
					cm->arguments << QString::fromStdString(o.string_value());
				}
			}
			cmdMap[key] = cm;
		}
	} else {
		error = "configuration file has no commands";
		return;
	}

	auto homeDirectory = QDir::homePath();

	static std::string tab("tab");
	static std::string name("name");
	static std::string run("run");
	static std::string directory("directory");
	static std::string sendLine("send-line");

	auto tabItems = conf[tab];
	if (tabItems) {
		// no size in current ucl++.h, just count untli null
		for (auto i = 0; tabItems[i]; ++i) {
			auto t = tabItems[i];

			if (!t[name]) {
				error = "tab without a name";
				return;
			}
			auto tabName = QString::fromStdString(t[name].string_value());
			if (!t[run]) {
				error = "tab: " + tabName + " without a run";
				return;
			}
			auto cmdKey = QString::fromStdString(t[run].string_value());
			auto *cmd = cmdMap[cmdKey];
			if (!cmd) {
				error = "tab: " + tabName + " has invalid run: " + cmdKey;
				return;
			}
			auto theTab = new Tab;
			theTab->name = tabName;
			theTab->command = cmd;
			if (t[directory]) {
				theTab->directory = QString::fromStdString(t[directory].string_value());
			}

			if ("" == theTab->directory) {
				theTab->directory = homeDirectory;
			} else if (theTab->directory.startsWith("~/")) {
				theTab->directory = homeDirectory + theTab->directory.mid(1);
			} else if (!theTab->directory.startsWith(QDir::separator())) {
				theTab->directory = homeDirectory + QDir::separator() + theTab->directory;
			}

			if (t[sendLine]) {
				for (const auto &s : t[sendLine]) {
					theTab->sendLines << QString::fromStdString(s.string_value());
				}
			}
			tabList << tabName;
			tabs[tabName] = theTab;
		}
	} else {
		error = "configuration file has no tabs";
		return;
	}

	valid = true;
}
