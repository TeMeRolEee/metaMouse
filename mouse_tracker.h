#pragma once

#include <QThread>
#include <QtCore/QTimer>

class MouseTracker : public QThread {
	Q_OBJECT

	public:
		explicit MouseTracker(QObject *parent = nullptr);
		~MouseTracker() override;

	public slots:
		void watchMouse_slot();

	public slots:
		void startWatching_slot();

	signals:
		void watchMouse_signal();

	private:
		QTimer *timer = nullptr;
};


