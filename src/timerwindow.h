#ifndef TIMERWINDOW_H
#define TIMERWINDOW_H

#include <QWidget>
#include <QTimer>
#include <QTime>
#include <QPropertyAnimation>

class TimerWindow : public QWidget
{
    Q_OBJECT

public:
    TimerWindow(int updateInterval = 200, QWidget *parent = nullptr);
    TimerWindow(int msec, int updateInterval = 200, QWidget *parent = nullptr);
    void setDuration(int msec);
    void setUpdateInterval(int msec);
    ~TimerWindow();

protected: // containers
    QTimer timer;
    QTimer ticker;
    int msecTimer; // actual timer duration
    int msecUpdateInterval; // update timer display every N sec

protected: // functions
    virtual void fadeIn();
    virtual void fadeOut();
    virtual void fadeOutNoFinished();
    virtual void updateCountdownDisplay(int msec) = 0;

private:
    std::unique_ptr<QPropertyAnimation> makeFadeOutAnimation();

public slots:
    void launch(); // fade in and start countdown

protected slots:
    void timerTick(); // update display with current time
    virtual void timerFinished(); // stop timers and fade out
    void startCountdown(); // launch actual timers
    void countdownFinished(); // emit finished() signal
    void stop(); // interrupt execution without finished() signal

signals:
    void finished(); // signal connected slot

};

#endif // TIMERWINDOW_H
