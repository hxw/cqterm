// conlecterm.cpp

#include <QDebug>
#include <QDir>
#include <QIcon>
#include <QtWidgets>

#include "terminaltab.h"
#include "xinterface.h"

const		   // for C++ to make pixmap include compile
#include "run.xpm" // …
	int dummy; // to keep alignment


class Embed : public QWindow {
public:
	explicit Embed() : QWindow() {
	}

	void resize(const QSize &newSize) {
		QWindow::resize(newSize);
		auto w = newSize.width();
		auto h = newSize.height();
		resizeChild(winId(), w, h);
	}

protected:
	void resizeEvent(QResizeEvent *ev) {
		auto s = ev->size();
		QWindow::resizeEvent(ev);
		resize(s);
	}

private:
};


TerminalTab::TerminalTab(QString program_, QStringList arguments_, QString directory_, QStringList sendLines_,
			 QWidget *parent_) :
		QWidget(parent_),
		program(program_), directory(directory_), sendLines(sendLines_),
		parentTabWidget(static_cast<QTabWidget *>(parent_)) {

	run = false;
	runIcon.addPixmap(QPixmap(run_xpm));

	pane = new QVBoxLayout;

	w = new Embed;
	container = QWidget::createWindowContainer(w);
	container->hide();

	// process argumenst to get window ID
	for (auto i = 0; i < arguments_.size(); ++i) {
		if (arguments_.at(i) == "%ID%") {
			arguments << QString::number(w->winId());
		} else {
			arguments << arguments_.at(i);
		}
	}


	startClose = new QWidget;
	auto *vbox = new QVBoxLayout;

	auto startButton = new QPushButton(tr("&Start"));
	auto *closeButton = new QPushButton(tr("&Close"));

	connect(startButton, SIGNAL(released()), this, SLOT(handleButton()));

	connect(closeButton, &QPushButton::released, [=]() {
		auto index = parentTabWidget->indexOf(this);
		parentTabWidget->removeTab(index);
	});


	startButton->setStyleSheet(
		"QPushButton {background-color: #63C163; color: green; font-size: 60pt; font-weight: bold;}");
	closeButton->setStyleSheet(
		"QPushButton {background-color: #C16464; color: red; font-size: 30pt; font-weight: bold;}");

	startButton->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
	closeButton->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

	vbox->addWidget(startButton);

	vbox->addWidget(closeButton);
	vbox->setAlignment(closeButton, Qt::AlignBottom);
	startClose->setLayout(vbox);
	startClose->show();

	proc = new QProcess(this);

	connect(proc, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
		[=](int exitCode, QProcess::ExitStatus exitStatus) {
			qDebug() << "FINISHED: " << exitCode << exitStatus;
			container->hide();
			startClose->show();
			run = false;
			auto index = parentTabWidget->indexOf(this);
			parentTabWidget->setTabIcon(index, QIcon());

		});

	connect(proc, &QProcess::started, [=]() {
		qDebug() << "Started";
		auto index = parentTabWidget->indexOf(this);
		parentTabWidget->setTabIcon(index, runIcon);

	});
	pane->addWidget(startClose);
	pane->addWidget(container);
	setLayout(pane);
}


void TerminalTab::handleButton() {
	qDebug() << "STARTING";
	start();
	startClose->hide();
	container->show();
}

bool TerminalTab::start() {
	if (!run) {
		if (proc) {
			proc->setProgram(program);
			proc->setArguments(arguments);

			if (QDir(directory).exists()) {
				proc->setWorkingDirectory(directory);
			} else {
				proc->setWorkingDirectory(QDir::homePath());
			}

			proc->start();
			proc->waitForStarted();
			run = true;

			auto lines = sendLines.join("\r") + "\r";
			sendKey(w->winId(), lines.toStdString());

			return true;
		}
	}
	return false;
}

TerminalTab::~TerminalTab() {
	if (proc) {
		proc->terminate();
		proc->waitForFinished();
		delete proc;
		proc = NULL;
	}
}
