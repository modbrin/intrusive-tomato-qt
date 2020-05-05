#include "timerwindow.h"
#include "utils.h"
#include <QGraphicsOpacityEffect>
#include <QScreen>
#include <QDebug>
#include <string>
#include <chrono>

TimerWindow::TimerWindow(int updateInterval, QWidget *parent) :
    QWidget(parent),
    msecUpdateInterval(updateInterval)
{
    connect(&ticker, SIGNAL(timeout()), this, SLOT(timerTick()));
    connect(&timer, SIGNAL(timeout()), this, SLOT(timerFinished()));
}

TimerWindow::TimerWindow(int msec, int updateInterval, QWidget *parent) :
    TimerWindow(updateInterval, parent)
{
    setDuration(msec);
}

void TimerWindow::setDuration(int msec)
{
    msecTimer = msec;
}

void TimerWindow::setUpdateInterval(int msec)
{
    msecUpdateInterval = msec;
}

void TimerWindow::launch()
{
    if (msecTimer > 1000) {
        fadeIn();
    } else {
        qDebug() << __FILE__ << __LINE__ << "attempt to set preparation timer for less than 1 second";
    }
}

void TimerWindow::fadeIn()
{
    //updateCountdownDisplay(msecTimer);
    QPropertyAnimation *anim = new QPropertyAnimation(this, "windowOpacity");
    connect(anim, SIGNAL(finished()), this, SLOT(startCountdown()));

    anim->setDuration(700);
    anim->setStartValue(0);
    anim->setEndValue(1);
    anim->setEasingCurve(QEasingCurve::InBack);
    anim->start(QPropertyAnimation::DeleteWhenStopped);

}

QPropertyAnimation* TimerWindow::makeFadeOutAnimation()
{
    QPropertyAnimation *anim = new QPropertyAnimation(this, "windowOpacity");
    anim->setDuration(500);
    anim->setStartValue(1);
    anim->setEndValue(0);
    anim->setEasingCurve(QEasingCurve::OutBack);
    return anim;
}

void TimerWindow::fadeOut()
{
    auto anim = makeFadeOutAnimation();
    connect(anim, SIGNAL(finished()), this, SLOT(countdownFinished()));
    connect(anim, SIGNAL(finished()), this, SLOT(close()));
    anim->start(QPropertyAnimation::DeleteWhenStopped);
}

void TimerWindow::fadeOutNoFinished()
{
    auto anim = makeFadeOutAnimation();
    connect(anim, SIGNAL(finished()), this, SLOT(close()));
    anim->start(QPropertyAnimation::DeleteWhenStopped);
}

void TimerWindow::startCountdown()
{
    ticker.start(msecUpdateInterval);
    timer.start(msecTimer);
}

void TimerWindow::countdownFinished()
{
    emit finished();
}

void TimerWindow::timerTick()
{
    updateCountdownDisplay(timer.remainingTime());
}

void TimerWindow::stop()
{
    ticker.stop();
    timer.stop();
    fadeOutNoFinished();
}

void TimerWindow::timerFinished()
{
    ticker.stop();
    timer.stop();
    fadeOut();
}

TimerWindow::~TimerWindow()
{
}
