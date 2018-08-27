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

typedef struct Layout {
        int tabWidth;
	int tabHeight;
	int hMargin;
	int vMargin;
	int iconWidth;
	int iconHeight;
	int fontPoints;
} Layout;

class Configuration {
public:
	explicit Configuration(QString Filename);

	QStringList allTabs() const {
		return tabList;
	}

	const Tab *get(QString name) const {
		return tabs[name];
	}

	const Layout &getLayout() const {
		return layout;
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
        Layout layout;
};

#endif
