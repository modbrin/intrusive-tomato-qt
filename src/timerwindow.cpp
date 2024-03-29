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
    anim->setEasingCurve(QEasingCurve::InOutQuad);
    anim->start(QPropertyAnimation::DeleteWhenStopped);

}

std::unique_ptr<QPropertyAnimation> TimerWindow::makeFadeOutAnimation()
{
    std::unique_ptr<QPropertyAnimation> anim = std::make_unique<QPropertyAnimation>(this, "windowOpacity");
    anim->setDuration(500);
    anim->setStartValue(1);
    anim->setEndValue(0);
    anim->setEasingCurve(QEasingCurve::InOutQuad);
    return anim;
}

void TimerWindow::fadeOut()
{
    QPropertyAnimation* anim = makeFadeOutAnimation().release();
    connect(anim, SIGNAL(finished()), this, SLOT(countdownFinished()));
    connect(anim, SIGNAL(finished()), this, SLOT(close()));
    anim->start(QPropertyAnimation::DeleteWhenStopped);
}

void TimerWindow::fadeOutNoFinished()
{
    std::unique_ptr<QPropertyAnimation> anim = makeFadeOutAnimation();
    connect(anim.release(), SIGNAL(finished()), this, SLOT(close()));
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
    // empty
}
