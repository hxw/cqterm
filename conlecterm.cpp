// conlecterm.cpp        -*- mode: c++ -*-

#include <QCoreApplication>
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
		QMainWindow(parent), configuration(conf), session(sess) {

        saveChanges = false;

        this->cq=new QWidget();

	setWindowTitle(tr("CQterm"));
	//resize(1200, 800);
	showMaximized();
	QIcon ico(":/application.icon");
	setWindowIcon(ico);
	setFocusPolicy(Qt::NoFocus);
	this->cq->setFocusPolicy(Qt::StrongFocus);

	auto *vbox = new QVBoxLayout;
	vbox->setContentsMargins(0, 0, 0, 0);

	auto layout = conf->getLayout();
	tabs = new HorizontalTabWidget(0, layout.tabWidth, layout.tabHeight, layout.hMargin, layout.vMargin,
				       layout.iconWidth, layout.iconHeight, layout.fontPoints);

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
	this->cq->setLayout(vbox);
        setCentralWidget(this->cq);

	setContextMenuPolicy(Qt::CustomContextMenu);
	connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(showContextMenu(const QPoint &)));

	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(save()));
	timer->start(10000);

	connect(tabs, &HorizontalTabWidget::tabsChanged,
		[=]() {
                        saveChanges = true;
                });
}


Conlecterm::~Conlecterm() {
	save();
	// delete tabs; not necessary?
}


// redirect key presses to the tab widget's current widget
bool Conlecterm::event(QEvent *event) {
	if (QEvent::KeyPress == event->type() || QEvent::KeyRelease == event->type()) {
		if (tabs) {
			auto *current = tabs->currentWidget();
			if (current) {
				QCoreApplication::sendEvent(tabs->currentWidget(), event);
				return true;
			}
		}
	}
	return QWidget::event(event);
}


// save current session
void Conlecterm::save() {
	if (saveChanges) {
		QStringList l;
		for (auto i = 0; i < tabs->count(); ++i) {
			l << tabs->tabText(i);
		}
		session->save(l);
                saveChanges = false;
        }
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
		// qDebug() << "nothing was chosen";
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
