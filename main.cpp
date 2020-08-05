#include <QCursor>
#include <QtGui>
#include <QtWidgets/QApplication>

#include <QDebug>

int main(int argc, char *argv[]) {
	QApplication application(argc, argv);
	QApplication::setApplicationName("MetaMouse");
	QApplication::setApplicationDisplayName("MetaMouse");

	for (auto screen : QApplication::screens()) {
		qDebug() << "Available size" << screen->availableSize() << "Size" << screen->size() << "Hz:" << screen->refreshRate();
		QPoint pos0 = QCursor::pos(screen);
		qDebug() << "X" << pos0.x() << "\tY" << pos0.y();
	}

	return QApplication::exec();
}
