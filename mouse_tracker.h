#pragma once

#include <QThread>
#include <QtCore/QTimer>

class MouseTracker : public QThread {
	Q_OBJECT

	public:
		explicit MouseTracker(QObject *parent = nullptr);

	protected:
	void run() override;

	public slots:
		void watchMouse_slot();
		void startWatching_slot();

	signals:
		void watchMouse_signal();

	private:
		QTimer *timer = nullptr;
};


