#include "notificationwindow.h"
#include "ui_notificationwindow.h"
#include "utils.h"
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QScreen>
#include <QDebug>
#include <string>
#include <chrono>
#include <QFontDatabase>

NotificationWindow::NotificationWindow(QString message, int msec, int updateInterval, QWidget *parent) :
    TimerWindow(msec, updateInterval, parent),
    ui(new Ui::NotificationWindow)
{
    ui->setupUi(this);

    QFontDatabase::addApplicationFont (":/fonts/misc/OpenSans.ttf");

    // setup window
    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::SubWindow);
    setStyleSheet("font-family: Open Sans; background-color:rgba( 255, 81, 0, 30% );");
    enableWindowBackgroundBlur(this);
    setAttribute(Qt::WA_DeleteOnClose); // when closed window will deallocate its memory
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
    QTime time(0, 0, 0);
    time = time.addMSecs(msec);

    ui->timeField->setText(QString::number(time.minute()).rightJustified(2, '0')
                           + ":"
                           + QString::number(time.second()).rightJustified(2, '0'));
}

void NotificationWindow::mousePressEvent(QMouseEvent *event) {
    m_nMouseClick_X_Coordinate = event->x();
    m_nMouseClick_Y_Coordinate = event->y();
}

void NotificationWindow::mouseMoveEvent(QMouseEvent *event) {
    move(event->globalX() - m_nMouseClick_X_Coordinate, event->globalY() - m_nMouseClick_Y_Coordinate);
}

NotificationWindow::~NotificationWindow()
{
    delete ui;
}
