// configuration.h       -*- mode: c++ -*-

#if !defined(_CONFIGURATION_H_)
#define _CONFIGURATION_H_ 1

#include <QMap>
#include <QString>
#include <QStringList>

typedef struct {
	QString program;
	QStringList arguments;
} Command;


typedef struct Tab {
	QString name;
	Command *command;
	QString directory;
	QStringList sendLines;
} Tab;


class Configuration {
public:
	explicit Configuration(QString Filename);

	QStringList allTabs() const {
		return tabList;
	}

	const Tab *get(QString name) const {
		return tabs[name];
	}

	bool isValid() const {
		return valid;
	}

	const QString getError() const {
		return error;
	}

private:
	bool valid;
	QMap<QString, Tab *> tabs;
	QStringList tabList;
	QString error;
};

#endif
