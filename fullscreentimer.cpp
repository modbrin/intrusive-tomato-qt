#include "fullscreentimer.h"
#include "ui_fullscreentimer.h"
#include "utils.h"

#include <QPropertyAnimation>
#include <QTimer>

FullscreenTimer::FullscreenTimer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FullscreenTimer)
{
    ui->setupUi(this);

    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::SubWindow);
    setStyleSheet("background-color:transparent;");
    enableWindowBackgroundBlur(this);
    QTimer::singleShot(0, this, SLOT(showFullScreen()));

    fadeIn();
}

void FullscreenTimer::fadeIn()
{
    QPropertyAnimation *anim = new QPropertyAnimation(this, "windowOpacity");
    anim->setDuration(700);
    anim->setStartValue(0);
    anim->setEndValue(1);
    anim->setEasingCurve(QEasingCurve::InBack);
    anim->start(QPropertyAnimation::DeleteWhenStopped);
}

void FullscreenTimer::fadeOut()
{

    QPropertyAnimation *anim = new QPropertyAnimation(this, "windowOpacity");
    anim->setDuration(500);
    anim->setStartValue(1);
    anim->setEndValue(0);
    anim->setEasingCurve(QEasingCurve::OutBack);
    anim->start(QPropertyAnimation::DeleteWhenStopped);
    //connect(anim, SIGNAL(finished()), this, SLOT(hideThisWidget()));
    // custom close action
}

FullscreenTimer::~FullscreenTimer()
{
    delete ui;
}
