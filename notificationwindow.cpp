#include "notificationwindow.h"
#include "ui_notificationwindow.h"
#include "utils.h"
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QScreen>
#include <QDebug>
#include <string>
#include <chrono>

NotificationWindow::NotificationWindow(QString message, int msec, int updateInterval, QWidget *parent) :
    TimerWindow(msec, updateInterval, parent),
    ui(new Ui::NotificationWindow)
{
    ui->setupUi(this);

    // setup window
    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::SubWindow);
    setStyleSheet("background-color:rgba( 255, 81, 0, 30% );");
    enableWindowBackgroundBlur(this);
    //setSizePolicy(QSizePolicy(QSizePolicy::, QSizePolicy::Fixed));
    setAttribute(Qt::WA_DeleteOnClose, true); // when closed window will deallocate its memory
    setProperty("windowOpacity", 0); // start invisible

    // position window
    auto screenGeometry = QGuiApplication::primaryScreen()->geometry();
    move(screenGeometry.left() + 100, screenGeometry.top() + 100); // TODO: match to dpi

    // edittext style setter
    auto setupTextStyle = [](QLineEdit* et) {
        et->setStyleSheet("color: white; background-color:transparent;");
        et->setEnabled(false);
    };

    // set style for timer and label
    setupTextStyle(ui->timeField);
    setupTextStyle(ui->labelField);

    updateCountdownDisplay(msec);
    ui->labelField->setText(message);

    adjustSize();
}

void NotificationWindow::updateCountdownDisplay(int msec)
{
    QTime time(0,0,0);
    time = time.addMSecs(msec);

    ui->timeField->setText(QString::number(time.minute()).rightJustified(2,'0')
                           + ":"
                           + QString::number(time.second()).rightJustified(2, '0'));
}

NotificationWindow::~NotificationWindow()
{
    delete ui;
}
