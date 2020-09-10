#pragma once

#include <QThread>
#include <QVector>
#include <QTimer>
#include <QScreen>
#include <QMutex>

class MouseTracker : public QThread {
	Q_OBJECT

	public:
		explicit MouseTracker(QObject *parent = nullptr);
		~MouseTracker() override;
//		bool eventFilter(QObject *watched, QEvent *event) override;

	public slots:
		void watchMouse_slot();
		void handleCoolDown_slot();
		void startWatching_slot();
		void handleCriticalCursorPosition_slot(QPoint cursorPos, int screenID);
		void handleScreenChanges_slot();

	signals:
		void watchMouse_signal();
		void startCoolDown_signal();
		void handleScreenChange_signal();
		void criticalCursorPosition_signal(const QPoint &cursorPos, int screenID);

	private:
		bool isAtScreenEdge(const QPoint &position);
		bool cursorPosToRelative(const QPoint &globalPos, QPoint &resultPos, int *screenID = nullptr);

		QTimer *timer = nullptr;
		QTimer *coolDown = nullptr;
		QVector<QScreen*> screens;
		QMutex screensMutex;
		QMutex mouseDragMutex;
		QBasicAtomicInteger<bool> mouseDrag = false;
};


