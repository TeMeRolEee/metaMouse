#include <QCursor>
#include <QtGui>
#include <QApplication>
#include <QCoreApplication>
#include <QMessageBox>

#include <QDebug>
#include <QtWidgets/QSystemTrayIcon>

#include "mouse_tracker.h"
#include "sys_tray.h"

int main(int argc, char *argv[]) {
	Q_INIT_RESOURCE(systray);

	QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

	QApplication application(argc, argv);
	QApplication::setApplicationName("MetaMouse");
	QApplication::setApplicationDisplayName("MetaMouse");

	if (!QSystemTrayIcon::isSystemTrayAvailable()) {
		QMessageBox::critical(nullptr, QObject::tr("Systray"),
							  QObject::tr("I couldn't detect any system tray "
										  "on this system."));
		return 1;
	}

	QApplication::setQuitOnLastWindowClosed(false);

//	for (auto screen : QApplication::screens()) {
//		qDebug() << "Available size" << screen->availableSize() << "Size" << screen->size() << "Hz:" << screen->refreshRate();
//		QPoint pos0 = QCursor::pos(screen);
//		qDebug() << "X" << pos0.x() << "\tY" << pos0.y();
//	}

	SysTray mouseTracker;
	mouseTracker.show();

	return QApplication::exec();
}
