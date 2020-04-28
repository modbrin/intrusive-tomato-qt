#include "notificationwindow.h"
#include "ui_notificationwindow.h"
#include "utils.h"
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QScreen>
#include <QDebug>
#include <string>
#include <chrono>

NotificationWindow::NotificationWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NotificationWindow)
{
    ui->setupUi(this);
    // setup window style
    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::SubWindow);
    setStyleSheet("background-color:rgba( 255, 81, 0, 30% );");
    enableWindowBackgroundBlur(this);
    setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));

    // position window
    auto screenGeometry = QGuiApplication::primaryScreen()->geometry();
    move(screenGeometry.left() + 100, screenGeometry.top() + 100);

    // edittext style setter
    auto setupTextStyle = [](QTextEdit* et) {
        et->setTextColor(QColor("white"));
        et->setStyleSheet("background-color:transparent;");

    };

    auto& tf = ui->timeField;
    auto& label = ui->labelField;


    // set style for timer and label
    setupTextStyle(tf);
    setupTextStyle(label);

    // set text (DEBUG)
    tf->setText(QString("12:34"));
    label->setText(QString("until break"));



    auto postStyleUpdate = [](QTextEdit* et){
        et->setAlignment(Qt::AlignCenter);
        QFontMetrics m (et->font());
        int RowHeight = m.lineSpacing();
        et->setFixedHeight(RowHeight);
        et->setEnabled(false);
    };

    postStyleUpdate(tf);
    postStyleUpdate(label);


    //tf->setFixedWidth(tf->document()->idealWidth() + tf->contentsMargins().left() + tf->contentsMargins().right());
    //label->setFixedWidth(label->document()->idealWidth() + label->contentsMargins().left() + label->contentsMargins().right());

    tf->adjustSize();
    label->adjustSize();

    adjustSize();

    countdown = QTime(0, 0, 30);

    connect(&ticker, SIGNAL(timeout()), this, SLOT(timerTick()));
    ticker.start(200);

    connect(&timer, SIGNAL(timeout()), this, SLOT(timerFinished()));
    timer.start(10000);

    fadeIn();
}

void NotificationWindow::fadeIn()
{
    QPropertyAnimation *anim = new QPropertyAnimation(this, "windowOpacity");
    anim->setDuration(700);
    anim->setStartValue(0);
    anim->setEndValue(1);
    anim->setEasingCurve(QEasingCurve::InBack);
    anim->start(QPropertyAnimation::DeleteWhenStopped);
}

void NotificationWindow::fadeOut()
{

    QPropertyAnimation *anim = new QPropertyAnimation(this, "windowOpacity");
    anim->setDuration(500);
    anim->setStartValue(1);
    anim->setEndValue(0);
    anim->setEasingCurve(QEasingCurve::OutBack);
    anim->start(QPropertyAnimation::DeleteWhenStopped);
    //connect(anim, SIGNAL(finished()), this, SLOT(signalFinished()));
    connect(anim, SIGNAL(finished()), this, SLOT(close()));
    // custom close action
}

void NotificationWindow::timerTick()
{
    QTime time(0,0,0);
    time = time.addMSecs(timer.remainingTime());
    //timer.remainingTimeAsDuration()
    //qDebug() << "Tick";
    ui->timeField->setText(QString::number(time.minute()).rightJustified(2,'0')
                           + ":"
                           + QString::number(time.second()).rightJustified(2, '0'));

    normalizeSizeAndCenter(ui->timeField);

    //qDebug() << "Tick " << timer.remainingTime();
}

void NotificationWindow::timerFinished()
{
    ticker.stop();
    timer.stop();
    //qDebug() << "Timer finished";
    fadeOut();
}

NotificationWindow::~NotificationWindow()
{
    delete ui;
}
