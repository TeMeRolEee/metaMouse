#include "mouse_tracker.h"

#include <QCursor>
#include <QDebug>

MouseTracker::MouseTracker(QObject *parent) : QThread(parent) {
	timer = new QTimer();
	//timer->setTimerType(Qt::TimerType::PreciseTimer);
	timer->setInterval(10);
	connect(timer, &QTimer::timeout, this, &MouseTracker::watchMouse_slot);
	connect(timer, &QTimer::timeout, timer, &QTimer::deleteLater);
	connect(timer, &QTimer::timeout, this, &MouseTracker::startWatching_slot);
}

void MouseTracker::watchMouse_slot() {
	qDebug() << timer->remainingTime();
	qDebug() << QCursor::pos();
}

void MouseTracker::startWatching_slot() {
	qDebug() << "RUN" << MouseTracker::currentThreadId();
	timer->start(100);
}

void MouseTracker::run() {

	QThread::run();
}
