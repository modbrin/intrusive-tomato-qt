#ifndef FULLSCREENTIMER_H
#define FULLSCREENTIMER_H

#include <QWidget>
#include <QTimer>
#include <QTime>
#include "timerwindow.h"

namespace Ui {
class FullscreenTimer;
}

class FullscreenTimer : public TimerWindow
{
    Q_OBJECT

public:
    explicit FullscreenTimer(int msec, int updateInterval = 200, QWidget *parent = nullptr);
    ~FullscreenTimer();

private: // functions
    virtual void updateCountdownDisplay(int msec) override;
    virtual void timerFinished() override;

private: // containers
    Ui::FullscreenTimer *ui;

private slots:
    void breakFinishConfirmed();

};

#endif // FULLSCREENTIMER_H
