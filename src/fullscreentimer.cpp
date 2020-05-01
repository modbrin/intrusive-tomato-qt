#include "fullscreentimer.h"
#include "ui_fullscreentimer.h"
#include "utils.h"

#include <QPropertyAnimation>
#include <QTimer>

FullscreenTimer::FullscreenTimer(int msec, int updateInterval, QWidget *parent) :
    TimerWindow(msec, updateInterval, parent),
    ui(new Ui::FullscreenTimer)
{
    ui->setupUi(this);

    // setup window
    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::SubWindow);
    setStyleSheet("background-color:rgba( 0, 0, 0, 50% );");
    enableWindowBackgroundBlur(this);
    setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    QTimer::singleShot(0, this, SLOT(showFullScreen()));
    setAttribute(Qt::WA_DeleteOnClose, true); // when closed window will deallocate its memory
    setProperty("windowOpacity", 0); // start invisible

    // edittext style setter
    auto setupTextStyle = [](QLineEdit* et) {
        et->setStyleSheet("color: white; background-color:transparent;");
        et->setEnabled(false);
    };

    setupTextStyle(ui->timeField);
    updateCountdownDisplay(msec);
}

void FullscreenTimer::updateCountdownDisplay(int msec)
{
    QTime time(0,0,0);
    time = time.addMSecs(msec);

    ui->timeField->setText(QString::number(time.minute()).rightJustified(2,'0')
                           + ":"
                           + QString::number(time.second()).rightJustified(2, '0'));
}

FullscreenTimer::~FullscreenTimer()
{
    delete ui;
}
