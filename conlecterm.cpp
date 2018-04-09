// conlecterm.cpp

#include <QCloseEvent>
#include <QDebug>
#include <QIcon>
#include <QLayout>
#include <QMenu>
#include <QMessageBox>
#include <QWidget>

#include "configuration.h"
#include "conlecterm.h"
#include "horizontaltabs.h"
#include "session.h"
#include "terminaltab.h"


Conlecterm::Conlecterm(Configuration *conf, Session *sess, QWidget *parent) :
		QWidget(parent), configuration(conf), session(sess) {

	auto *vbox = new QVBoxLayout;
	vbox->setContentsMargins(0, 0, 0, 0);

	tabs = new HorizontalTabWidget;

	tabs->setMovable(true);
	tabs->setUsesScrollButtons(true);
	tabs->setFocusPolicy(Qt::NoFocus);

	for (auto &s : session->load()) {
		auto *tab = conf->get(s);
		if (tab) {
			auto cmd = tab->command;
			auto *t = new TerminalTab(cmd->program, cmd->arguments, tab->directory, tab->sendLines, tabs);
			tabs->addTab(t, tab->name);
		}
	}

	vbox->addWidget(tabs);

	setLayout(vbox);
	setFocusPolicy(Qt::StrongFocus);

	setWindowTitle(tr("CQterm"));
	resize(1200, 800);

	setContextMenuPolicy(Qt::CustomContextMenu);
	connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(showContextMenu(const QPoint &)));
}

Conlecterm::~Conlecterm() {

	// save current session
	QStringList l;
	for (auto i = 0; i < tabs->count(); ++i) {
		l << tabs->tabText(i);
	}

	session->save(l);

	// delete tabs; not necessary?
}


void Conlecterm::showContextMenu(const QPoint &pos) {

	// for most widgets
	QPoint globalPos = mapToGlobal(pos);
	// for QAbstractScrollArea and derived classes you would use:
	// QPoint globalPos = myWidget->viewport()->mapToGlobal(pos);

	QMenu sessionMenu;
	sessionMenu.setTitle("Choose a session…");
	sessionMenu.setSeparatorsCollapsible(false);
	sessionMenu.addSeparator()->setText("choose session…");
	sessionMenu.addSeparator();
	sessionMenu.setStyleSheet(
		"QMenu {"
		//"  background-color: #a6daff;"  // hides separator text
		"  font-size: 10pt;"   // separator
		"  font-weight: bold;" // separator
		"  border: 2px solid black;"
		"}"
		"QMenu::item {"
		"  background-color: #a6daff;"
		"  color: #000000;"
		"  font-size: 12pt;"
		"  font-weight: bold;"
		"}"
		"QMenu::separator {"
		"  color: #0000ff;"
		"}"
		"QMenu::item::selected {"
		"  background-color: #00daff;"
		"}");

	for (auto &s : configuration->allTabs()) {
		sessionMenu.addAction(configuration->get(s)->name);
	}

	QAction *selectedItem = sessionMenu.exec(globalPos);
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


void Conlecterm::closeEvent(QCloseEvent *event) {
	auto active = 0;
	for (auto i = 0; i < tabs->count(); ++i) {
		if (!tabs->tabIcon(i).isNull()) {
			++active;
		}
	}

	if (active > 0) {
		QMessageBox msgBox;
		msgBox.setText("Active Tabs!");
		msgBox.setInformativeText("Choose Ok to force exit");
		msgBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);
		msgBox.setDefaultButton(QMessageBox::Cancel);
		auto r = msgBox.exec();

		if (QMessageBox::Ok == r) {
			event->accept();
		} else {
			event->ignore();
		}
		return;
	}

	event->accept();
}
