#pragma once

#include <QThread>
#include <QVector>
#include <QtCore/QTimer>
#include <QtGui/QScreen>

class MouseTracker : public QThread {
	Q_OBJECT

	public:
		explicit MouseTracker(QObject *parent = nullptr);
		~MouseTracker() override;

	public slots:
		void watchMouse_slot();
		void startWatching_slot();
		void handleCriticalCursorPosition_slot(const QPoint cursorPos, int screenID);
		void handleScreenChanges_slot();

	signals:
		void watchMouse_signal();
		void handleScreenChange_signal();
		void criticalCursorPosition_signal(const QPoint &cursorPos, int screenID);

	private:
		bool isAtScreenEdge(const QPoint &position);
		bool cursorPosToRelative(const QPoint &globalPos, QPoint &resultPos, int *screenID = nullptr);

		QTimer *timer = nullptr;
		QVector<QScreen*> screens;
		std::mutex screensMutex;
};


