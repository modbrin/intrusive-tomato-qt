#ifndef NOTIFICATIONWINDOW_H
#define NOTIFICATIONWINDOW_H

#include <QWidget>
#include <QTimer>
#include <QTime>
#include <QMouseEvent>
#include "timerwindow.h"

namespace Ui {
class NotificationWindow;
}

class NotificationWindow : public TimerWindow
{
    Q_OBJECT

public:
    NotificationWindow(QString message, int msec, int updateInterval = 200, QWidget *parent = nullptr);
    ~NotificationWindow();


private: // functions
    virtual void updateCountdownDisplay(int msec) override;

private: // containers
    Ui::NotificationWindow *ui;

private: // handling dragging
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    int m_nMouseClick_X_Coordinate;
    int m_nMouseClick_Y_Coordinate;
};

#endif // NOTIFICATIONWINDOW_H
