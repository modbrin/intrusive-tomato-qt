#include "fullscreentimer.h"
#include "ui_fullscreentimer.h"
#include "utils.h"

#include <QPropertyAnimation>
#include <QTimer>
#include <QMediaPlayer>
#include <QAudioOutput>

FullscreenTimer::FullscreenTimer(int msec, int updateInterval, QWidget *parent) :
    TimerWindow(msec, updateInterval, parent),
    ui(new Ui::FullscreenTimer)
{
    ui->setupUi(this);

    // setup window
    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::SubWindow);
    setStyleSheet("font-family: Open Sans; background-color:rgba( 0, 0, 0, 50% );");
    enableWindowBackgroundBlur(this);
    QTimer::singleShot(0, this, SLOT(showFullScreen())); // set to fullscreen
    setAttribute(Qt::WA_DeleteOnClose); // when closed window will deallocate its memory
    setProperty("windowOpacity", 0); // start invisible

    // setup confirmation button
    ui->confirmButton->setStyleSheet("QPushButton{background-color: rgba(255,255,255, 0%);"
                                     "padding: 8px 16px 8px 16px;"
                                     "border-style: outset;"
                                     "border-width: 2px;"
                                     "border-radius: 10px;"
                                     "border-color: white;}"
                                     "QPushButton:hover{background-color: rgba(255,255,255, 10%);}"
                                     "QPushButton:pressed{background-color: rgba(255,255,255, 20%);}");
    ui->confirmButton->setEnabled(false);
    ui->confirmButton->setVisible(false);
    connect(ui->confirmButton, SIGNAL(clicked()), this, SLOT(breakFinishConfirmed()));

    // edittext style setter
    auto setupTextStyle = [](QLineEdit* et) {
        et->setStyleSheet("color: white; background-color:transparent;");
        et->setEnabled(false);
    };

    // init media player
    m_player = new QMediaPlayer(this);
    m_audioOutput = new QAudioOutput(this);
    m_player->setAudioOutput(m_audioOutput);

    setupTextStyle(ui->timeField);
    updateCountdownDisplay(msec);
}

void FullscreenTimer::updateCountdownDisplay(int msec)
{
    QTime time(0,0,0);
    time = time.addMSecs(msec);

    ui->timeField->setText(QString::number(time.minute()).rightJustified(2, '0')
                           + ":"
                           + QString::number(time.second()).rightJustified(2, '0'));
}

void FullscreenTimer::timerFinished()
{
    // stop the timer
    ticker.stop();
    timer.stop();

    // hide message
    ui->message->setVisible(false);

    // hide timer
    //ui->timeField->setVisible(false); // without it looks kind of strange

    // show confirmation button
    ui->confirmButton->setEnabled(true); // TODO: add animation
    ui->confirmButton->setVisible(true);

    // sound effect
    m_player->setSource(QUrl("qrc:/sounds/misc/notification.wav"));
    m_player->play();
}

void FullscreenTimer::breakFinishConfirmed()
{
    ui->confirmButton->setEnabled(false);
    fadeOut();
}

FullscreenTimer::~FullscreenTimer()
{
    delete ui;
}
