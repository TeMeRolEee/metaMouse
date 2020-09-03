#pragma once

#include <QSystemTrayIcon>
#include <QDialog>
#include <QMenu>
#include "mouse_tracker.h"

QT_BEGIN_NAMESPACE
class QAction;
class QCheckBox;
class QComboBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QMenu;
class QPushButton;
class QSpinBox;
class QTextEdit;
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
};


