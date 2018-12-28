// conlecterm.cpp        -*- mode: c++ -*-

#include <QDebug>
#include <QDir>
#include <QIcon>
#include <QtWidgets>
#include <unistd.h> // usleep()

#include "embed.h"
#include "terminaltab.h"


TerminalTab::TerminalTab(QString program_, QStringList arguments_, QString directory_, QStringList sendLines_,
			 QWidget *parent_) :
		QWidget(parent_),
		program(program_), directory(directory_), sendLines(sendLines_),
		parentTabWidget(static_cast<QTabWidget *>(parent_)) {

	run = false;
	runIcon = QIcon(":/run.icon");

	pane = new QVBoxLayout;
	pane->setContentsMargins(0, 0, 0, 0);

	w = new Embed;
	container = QWidget::createWindowContainer(w);

	container->hide();
	container->setFocusPolicy(Qt::StrongFocus);
	setFocusProxy(container);
	setFocusPolicy(Qt::NoFocus);

	// process argumenst to get window ID
	for (auto i = 0; i < arguments_.size(); ++i) {
		if (arguments_.at(i) == "%ID%") {
			arguments << QString::number(w->winId());
		} else {
			arguments << arguments_.at(i);
		}
	}


	startClose = new QWidget;
	startClose->setFocusPolicy(Qt::NoFocus);
	auto *vbox = new QVBoxLayout;

	auto startButton = new QPushButton(tr("&Start"));
	auto *closeButton = new QPushButton(tr("&Close"));

	startButton->setFocusPolicy(Qt::NoFocus);
	closeButton->setFocusPolicy(Qt::NoFocus);

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
			w->end();
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
			qDebug() << "begin";
			w->begin();
			run = true;

			if (!sendLines.isEmpty()) {
				usleep(100000);
				auto lines = sendLines.join("\r") + "\r";
				w->sendKeys(lines);
			}
			return true;
		}
	}
	return false;
}


void TerminalTab::keyPressEvent(QKeyEvent *event) {
	w->sendKey(true, event);
}


void TerminalTab::keyReleaseEvent(QKeyEvent *event) {
	w->sendKey(false, event);
}


TerminalTab::~TerminalTab() {
	if (proc) {
		proc->terminate();
		proc->waitForFinished();
		delete proc;
		proc = NULL;
	}
}
