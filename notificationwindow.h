#ifndef NOTIFICATIONWINDOW_H
#define NOTIFICATIONWINDOW_H

#include <QWidget>
#include <QTimer>
#include <QTime>
#include "timerwindow.h"

namespace Ui {
class NotificationWindow;
}

class NotificationWindow : public TimerWindow
{
    Q_OBJECT

public:
    explicit NotificationWindow(QString message, int msec, int updateInterval = 200, QWidget *parent = nullptr);
    ~NotificationWindow();


private: // functions
    virtual void updateCountdownDisplay(int msec) override;

private: // containers
    Ui::NotificationWindow *ui;
};

#endif // NOTIFICATIONWINDOW_H
