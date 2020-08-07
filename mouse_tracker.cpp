#include "mouse_tracker.h"

#include <QCursor>
#include <QDebug>

MouseTracker::MouseTracker(QObject *parent) : QThread(parent)
{}

void MouseTracker::watchMouse_slot() {
	qInfo() << QCursor::pos();
}

void MouseTracker::startWatching_slot() {
	timer = new QTimer();
	timer->setTimerType(Qt::TimerType::PreciseTimer);
	timer->setInterval(100);

	connect(timer, &QTimer::timeout, this, &MouseTracker::watchMouse_slot);

	timer->start(10);
}

MouseTracker::~MouseTracker() {
	if (timer->isActive()) {
		timer->stop();
	}

	delete timer;
}