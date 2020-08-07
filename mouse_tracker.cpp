#include "mouse_tracker.h"

#include <QCursor>
#include <QDebug>

MouseTracker::MouseTracker(QObject *parent) : QThread(parent)
{}

void MouseTracker::watchMouse_slot() {
//	qInfo() << QPoint(200,200).manhattanLength() << "\t" << QCursor::pos().manhattanLength();
//	if (QCursor::pos().manhattanLength() < QPoint(200,200).manhattanLength()) {
//		QCursor::setPos(QCursor::pos() + QPoint(50,50));
//	}
	qInfo() << QCursor::pos();
}

void MouseTracker::startWatching_slot() {
	timer = new QTimer();
	timer->setTimerType(Qt::TimerType::PreciseTimer);
	timer->setInterval(10);

	connect(timer, &QTimer::timeout, this, &MouseTracker::watchMouse_slot);

	timer->start();
}

MouseTracker::~MouseTracker() {
	if (timer->isActive()) {
		timer->stop();
	}

	delete timer;
}
