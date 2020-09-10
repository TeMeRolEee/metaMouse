#include "mouse_tracker.h"

#include <QCursor>
#include <QDebug>
#include <QApplication>
#include <QWidget>
#include <QtMath>
#include <QMutexLocker>

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

	coolDown = new QTimer();
	coolDown->setTimerType(Qt::TimerType::PreciseTimer);
	coolDown->setInterval(1000);
	coolDown->setSingleShot(true);

	connect(timer, &QTimer::timeout, this, &MouseTracker::watchMouse_slot);
	connect(this, &MouseTracker::startCoolDown_signal, this, &MouseTracker::handleCoolDown_slot);

	timer->start();
}

void MouseTracker::handleScreenChanges_slot() {
	const QMutexLocker lockGuard(&screensMutex);
	screens.clear();

	for (auto tempScreen : QApplication::screens()) {
		screens.push_back(tempScreen);
	}
}

void MouseTracker::handleCriticalCursorPosition_slot(const QPoint cursorPos, int screenID) {
	qInfo() << coolDown->isActive() << mouseDrag;
	if (coolDown->isActive() || mouseDrag) {
		return;
	}

	const QMutexLocker lockGuard(&screensMutex);

	qreal yPos = ((qreal)cursorPos.y() / (screens.at(screenID)->geometry().height() - 1));

	if (screenID != 0 && screenID < screens.size() - 1) {
		if (cursorPos.x() >= (screens.at(screenID)->geometry().width() / 2)) {
			QCursor::setPos(QCursor::pos().x() + 5, (int) (screens.at(screenID + 1)->geometry().height() * yPos));
		}
	} else if (screenID > 0 && cursorPos.x()) {
		if (screens.at(screenID)->geometry().width() / 2) {
			QCursor::setPos(QCursor::pos().x() - 5, (int) (screens.at(screenID - 1)->geometry().height() * yPos));
		}
	}

	emit startCoolDown_signal();
}

bool MouseTracker::isAtScreenEdge(const QPoint &position) {
	int width = QApplication::screenAt(position)->size().width() - 1;
	QPoint tempPos;

	if (cursorPosToRelative(position, tempPos)) {
		if (width == tempPos.x() || tempPos.x() <= 2) {
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
		const QMutexLocker lockGuard(&screensMutex);
		*screenID = screens.indexOf(QApplication::screenAt(globalPos));
		resultPos = globalPos;

		if (*screenID > screens.size()) {
			return false;
		}

		for (int currentScreenID = 0; currentScreenID < *screenID; ++currentScreenID) {
			resultPos.setX(qFabs(screens.at(currentScreenID)->size().width() - 1 - resultPos.x()));
			resultPos.setY(qFabs(globalPos.y()));
		}
	}

	return true;
}

void MouseTracker::handleCoolDown_slot() {
	qInfo() << "CoolDown started";
	coolDown->start();
}

//bool MouseTracker::eventFilter(QObject *watched, QEvent *event) {
//	qInfo() << "eventFilter";
//
//	switch (event->type()) {
//		case QEvent::MouseButtonPress: {
//			const QMutexLocker lockGuard(&mouseDragMutex);
//			mouseDrag = true;
//			break;
//		}
//		case QEvent::UngrabMouse: {
//			const QMutexLocker lockGuard(&mouseDragMutex);
//			mouseDrag = false;
//			break;
//		}
//	}
//
//	return QObject::eventFilter(watched, event);
//}
