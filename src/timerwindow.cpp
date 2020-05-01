#include "timerwindow.h"
#include "utils.h"
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QScreen>
#include <QDebug>
#include <string>
#include <chrono>

TimerWindow::TimerWindow(int msec, int updateInterval, QWidget *parent) :
    QWidget(parent),
    msecTimer(msec),
    msecUpdateInterval(updateInterval)
{
    connect(&ticker, SIGNAL(timeout()), this, SLOT(timerTick()));
    connect(&timer, SIGNAL(timeout()), this, SLOT(timerFinished()));
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

void TimerWindow::fadeOut()
{
    QPropertyAnimation *anim = new QPropertyAnimation(this, "windowOpacity");
    anim->setDuration(500);
    anim->setStartValue(1);
    anim->setEndValue(0);
    anim->setEasingCurve(QEasingCurve::OutBack);
    anim->start(QPropertyAnimation::DeleteWhenStopped);
    connect(anim, SIGNAL(finished()), this, SLOT(countdownFinished()));
    connect(anim, SIGNAL(finished()), this, SLOT(close()));
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

void TimerWindow::timerFinished()
{
    ticker.stop();
    timer.stop();
    fadeOut();
}

TimerWindow::~TimerWindow()
{
}
