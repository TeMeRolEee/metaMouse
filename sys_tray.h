#pragma once

#include <QSystemTrayIcon>
#include <QDialog>
#include <QMenu>
#include "mouse_tracker.h"

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
QT_END_NAMESPACE

class SysTray : public QDialog {
	Q_OBJECT

	public:
		SysTray();

	private:
		void createTrayIcon();

		QSystemTrayIcon *trayIcon = nullptr;
		QMenu *trayIconMenu = nullptr;
		QAction *quitAction = nullptr;
		MouseTracker *mouseTracker = nullptr;

	signals:
		void handleScreenChange_signal(QScreen *screen);
};


