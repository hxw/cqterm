// conlecterm.cpp

#include <QWidget>
#include <QLayout>
#include <QDebug>
#include <QMenu>
#include <QIcon>
#include <QPixmap>

#include "conlecterm.h"
#include "horizontaltabs.h"
#include "terminaltab.h"
#include "configuration.h"
#include "session.h"


Conlecterm::Conlecterm(Configuration *conf, Session *sess, QWidget *parent) :
	QWidget(parent), configuration(conf), session(sess) {

	auto *vbox = new QVBoxLayout;
	tabs = new HorizontalTabWidget;

	tabs->setMovable(true);
	tabs->setUsesScrollButtons(true);

	for (auto &s : session->load()) {
		auto tab = conf->get(s);
		auto cmd = tab->command;
		auto *t = new TerminalTab(cmd->program, cmd->arguments, tab->directory, tab->sendLines, tabs);
		tabs->addTab(t, tab->name);
	}

	vbox->addWidget(tabs);

	setLayout(vbox);
	setWindowTitle(tr("My Test"));
	resize(1200, 800);


	setContextMenuPolicy(Qt::CustomContextMenu);
	connect(this, SIGNAL(customContextMenuRequested(const QPoint&)),
		this, SLOT(showContextMenu(const QPoint&)));
}

Conlecterm::~Conlecterm() {

	// save current session
	QStringList l;
	for (auto i = 0; i < tabs->count(); ++i) {
		l << tabs->tabText(i);
	}

	session->save(l);

	//delete tabs; not necessary?
}


void Conlecterm::showContextMenu(const QPoint &pos) {

	// for most widgets
	QPoint globalPos = mapToGlobal(pos);
	// for QAbstractScrollArea and derived classes you would use:
	// QPoint globalPos = myWidget->viewport()->mapToGlobal(pos);

	QMenu sessionMenu;
	sessionMenu.setTitle("Choose a session…");
	for (auto &s : configuration->allTabs()) {
		sessionMenu.addAction(configuration->get(s)->name);
	}

	QAction* selectedItem = sessionMenu.exec(globalPos);
	if (selectedItem) {
		auto tab = configuration->get(selectedItem->text());
		if (tab) {
			auto cmd = tab->command;
			auto *t = new TerminalTab(cmd->program, cmd->arguments, tab->directory, tab->sendLines, tabs);
			tabs->addTab(t, tab->name);
		}
	} else {
		qDebug() << "nothing was chosen";
	}
}
