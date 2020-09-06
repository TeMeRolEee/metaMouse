#include "sys_tray.h"

#include <QCoreApplication>
#include <QCloseEvent>
#include <QAction>

SysTray::SysTray() {
	mouseTracker = new MouseTracker();
	quitAction = new QAction(tr("&Quit"), this);
	createTrayIcon();

	connect(mouseTracker, &MouseTracker::started, mouseTracker, &MouseTracker::watchMouse_signal);
	connect(mouseTracker, &MouseTracker::watchMouse_signal, mouseTracker, &MouseTracker::startWatching_slot);
	connect(mouseTracker, &MouseTracker::finished, mouseTracker, &MouseTracker::deleteLater);
	connect(this, &SysTray::handleScreenChange_signal, mouseTracker, &MouseTracker::handleScreenChange_signal);
	connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);

	mouseTracker->start(QThread::HighPriority);

	setMouseTracking(true);
	installEventFilter(this);
	setWindowTitle(tr("MetaMouse"));

	trayIcon->show();
	resize(400, 300);
}

void SysTray::createTrayIcon() {
	trayIcon = new QSystemTrayIcon(this);
	trayIconMenu = new QMenu(this);
	trayIcon->setIcon(QIcon(":/images/bad.png"));
	setWindowIcon(trayIcon->icon());
	trayIcon->setVisible(true);
	trayIconMenu->addAction(quitAction);
	trayIcon->setContextMenu(trayIconMenu);
}

