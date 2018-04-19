// session.h             -*- mode: c++ -*-

#if !defined(_SESSION_H_)
#define _SESSION_H_ 1

#include <QString>
#include <QStringList>


class Session {
public:
	explicit Session(QString filename);

	QStringList load();
	void save(QStringList items);

private:
	QString sessionFile;
};

#endif
