#ifndef NOTIFICATIONWINDOW_H
#define NOTIFICATIONWINDOW_H

#include <QWidget>
#include <QTimer>
#include <QTime>

namespace Ui {
class NotificationWindow;
}

class NotificationWindow : public QWidget
{
    Q_OBJECT

public:
    explicit NotificationWindow(QWidget *parent = nullptr);
    ~NotificationWindow();

    void fadeIn();
    void fadeOut();
private:
    Ui::NotificationWindow *ui;
    QTimer timer;
    QTimer ticker;
    QTime countdown;
private slots:
    void timerTick();
    void timerFinished();
};

#endif // NOTIFICATIONWINDOW_H
