#include "mouse_tracker.h"

#include <QCursor>
#include <QDebug>
#include <QApplication>
#include <QWidget>
#include <QtMath>

MouseTracker::MouseTracker(QObject *parent) : QThread(parent)
{
	connect(this, &MouseTracker::criticalCursorPosition_signal, this, &MouseTracker::handleCriticalCursorPosition_slot, Qt::BlockingQueuedConnection);
	connect(this, &MouseTracker::handleScreenChange_signal, this, &MouseTracker::handleScreenChanges_slot);
	emit handleScreenChange_signal();
}

MouseTracker::~MouseTracker() {
	if (timer->isActive()) {
		timer->stop();
	}

	delete timer;
}

void MouseTracker::watchMouse_slot() {
	if (isAtScreenEdge(QCursor::pos())) {
		QPoint relativePos;
		int screenID = 0;
		if (cursorPosToRelative(QCursor::pos(), relativePos, &screenID)) {
//			qInfo() << "At edge";
			emit handleCriticalCursorPosition_slot(relativePos, screenID);
		}
	} else {
//		qInfo() << "Not at edge";
	}
}

void MouseTracker::startWatching_slot() {
	timer = new QTimer();
	timer->setTimerType(Qt::TimerType::PreciseTimer);
	timer->setInterval(5);

	connect(timer, &QTimer::timeout, this, &MouseTracker::watchMouse_slot);

	timer->start();
}

void MouseTracker::handleScreenChanges_slot() {
	const std::lock_guard<std::mutex> lockGuard(screensMutex);

	screens.clear();
	for (auto tempScreen : QApplication::screens()) {
		screens.push_back(tempScreen);
	}
}

void MouseTracker::handleCriticalCursorPosition_slot(const QPoint cursorPos, int screenID) {
	const std::lock_guard<std::mutex> lockGuard(screensMutex);

	qreal yPos = ((qreal)cursorPos.y() / screens.at(screenID)->geometry().height());
	qInfo() << cursorPos << screenID << screens.at(screenID)->geometry().height() << cursorPos.y() << yPos;

	if (screenID != 0 && screenID < screens.size() && cursorPos.x() >= (screens.at(screenID)->geometry().width() / 2)) {
		QCursor::setPos(screens.at(screenID + 1), 0, (int) (screens.at(screenID + 1)->geometry().height() * yPos));
	} else if (screenID > 0 && cursorPos.x() <= (screens.at(screenID)->geometry().width() / 2)) {
		QCursor::setPos(screens.at(screenID - 1), screens.at(screenID - 1)->geometry().x() - 1, (int) (screens.at(screenID - 1)->geometry().height() * yPos));
	}
}

bool MouseTracker::isAtScreenEdge(const QPoint &position) {
	int width = QApplication::screenAt(position)->size().width() - 1;
	QPoint tempPos;

	if (cursorPosToRelative(position, tempPos)) {
		//qInfo() << width << tempPos.x();
		if (width == tempPos.x() || tempPos.x() <= 3) {
			return true;
		}
	}

	return false;
}

bool MouseTracker::cursorPosToRelative(const QPoint &globalPos, QPoint &resultPos, int *screenID) {
	if (!screenID) {
		screenID = new int;
	}

	{
		const std::lock_guard<std::mutex> lockGuard(screensMutex);
		*screenID = screens.indexOf(QApplication::screenAt(globalPos));
		resultPos = globalPos;

		if (*screenID > screens.size()) {
			return false;
		}

		for (int currentScreenID = 0; currentScreenID < *screenID; ++currentScreenID) {
			resultPos.setX(qFabs(screens.at(currentScreenID)->size().width() - 1 - resultPos.x()));
			resultPos.setY(qFabs(globalPos.y()));
		}

//		qInfo() << resultPos.x() << screens.at(*screenID)->size().width();
	}

	return true;
}
