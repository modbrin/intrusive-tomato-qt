#include "mainwindow.h"
#include "ui_mainwindow.h"

//#include <QTimer>
#include <QDebug>
#include <QMouseEvent>
#include <QIntValidator>
#include <QStyle>
#include <QMessageBox>
#include <QFontDatabase>
#include <QGraphicsOpacityEffect>
#include <QSound>
#include <QSettings>
#include "notificationwindow.h"
#include "fullscreentimer.h"
#include "utils.h"
#include <sstream>


MainWindow::MainWindow(QWidget*)
    : TimerWindow(2000, nullptr)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // setup fonts
    QFontDatabase::addApplicationFont (":/fonts/misc/OpenSans.ttf");
    setStyleSheet("font-family: Open Sans;"); // stylesheet needs to be applied in each window

    // window configs
    setWindowFlags(Qt::FramelessWindowHint);
    connect(ui->closeButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->trayButton, SIGNAL(clicked()), this, SLOT(hide()));
    ui->workMinutes->setFixedWidth(ui->workMinutes->width());
    ui->breakMinutes->setFixedWidth(ui->breakMinutes->width());

    // style ui controls
    auto buttonStyle = [](auto color, auto icon_url, auto padding) {
        std::stringstream ss;
        ss << "QPushButton{background-color: rgba(" << color << ", 30%);"
               "image: url(" << icon_url << ");"
               "padding: " << padding << ";"
               "border-style: outset;"
               "border-width: 0px;"
               "border-radius: 15px;"
               "border-color: gray;}"
               "QPushButton:hover{background-color: rgba(" << color << ", 40%);}"
               "QPushButton:pressed{background-color: rgba(255, 255, 255, 20%);}";
        return ss.str();
    };

    auto inputStyle = "QLineEdit{background-color: rgba(255, 255, 255, 10%);"
                      "border-style: outset;"
                      "border-width: 0px;"
                      "border-radius: 7px;}";

    auto progressStyle = "QProgressBar {"
                         "border-style: outset;"
                         "border-width: 0px;"
                         "border-radius: 7px;"
                         "background-color: rgba(255,255,255,10%);}"
                         "QProgressBar::chunk {"
                         "border-style: outset;"
                         "border-width: 0px;"
                         "border-radius: 7px;"
                         "background-color: rgba(128, 226, 126, 50%);}";

    auto checkboxStyle = "QCheckBox::indicator {"
                         "width: 16px;"
                         "height: 16px;}"
                         "QCheckBox::indicator {background-color: rgba(255, 255, 255, 10%);"
                         "border-style: outset;"
                         "border-width: 0px;"
                         "border-radius: 2px;}"
                         "QCheckBox::indicator:unchecked {}"
                         "QCheckBox::indicator:checked {"
                         "image: url(:/images/misc/checked.png);}";

    ui->startButton->setStyleSheet(buttonStyle("128, 226, 126", ":/images/misc/play_icon.png", "20px 8px 20px 14px").c_str());
    ui->stopButton->setStyleSheet(buttonStyle("255, 111, 96", ":/images/misc/stop_icon.png", "20px 10px 20px 10px").c_str());
    ui->progressBar->setStyleSheet(progressStyle);
    ui->workMinutes->setStyleSheet(inputStyle);
    ui->breakMinutes->setStyleSheet(inputStyle);
    ui->trayButton->setStyleSheet("QPushButton {background-color: rgb(40,40,40);}");
    ui->closeButton->setStyleSheet("background-color: rgb(112, 21, 21);");
    ui->autostart->setStyleSheet(checkboxStyle);

    // apply shadow effect
    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect;
    shadowEffect->setColor(Qt::black);
    shadowEffect->setXOffset(0);
    shadowEffect->setYOffset(5);
    shadowEffect->setBlurRadius(15);

    ui->startButton->setGraphicsEffect(shadowEffect);
    ui->stopButton->setGraphicsEffect(shadowEffect);
    ui->workMinutes->setGraphicsEffect(shadowEffect);
    ui->breakMinutes->setGraphicsEffect(shadowEffect);
    ui->progressBar->setGraphicsEffect(shadowEffect);

    // resize upper buttons
    ui->closeButton->setFixedWidth(ui->closeButton->width() * 0.4);
    ui->trayButton->setFixedWidth(ui->trayButton->width() * 0.6);

    // style separator lines
    ui->upperLine->setStyleSheet("color: gray;");
    ui->bottomLine->setStyleSheet("color: transparent;");
    ui->bottomLine2->setStyleSheet("color: transparent;");

    // disable start button on launch
    ui->stopButton->setEnabled(false);
    ui->stopButton->setVisible(false);

    // set validators for input fields
    ui->workMinutes->setValidator( new QIntValidator(1, 900, this) );
    ui->breakMinutes->setValidator( new QIntValidator(1, 300, this) );
    ui->workMinutes->setText(QString::number(35));
    ui->breakMinutes->setText(QString::number(5));

    // default progress bar state
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(100);
    ui->progressBar->setValue(0);
    ui->progressBar->setTextVisible(false);

    // system tray icon
    trayIcon = std::unique_ptr<QSystemTrayIcon>(new QSystemTrayIcon(this));
    trayIcon->setIcon(QIcon(":/images/icon.ico"));
    trayIcon->setToolTip("QTomato");

    trayMenu = std::unique_ptr<QMenu>(new QMenu(this));
    trayIcon->setContextMenu(trayMenu.get());
    connect(trayIcon.get(), SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(showHide(QSystemTrayIcon::ActivationReason)));

    trayQuitAction = std::unique_ptr<QAction>(new QAction("Quit",this)); // TODO: is context menu really needed?
    connect(trayQuitAction.get(), SIGNAL(triggered()), this, SLOT(close()));
    trayMenu->addAction(trayQuitAction.get());

    trayRestoreAction = std::unique_ptr<QAction>(new QAction("Restore",this));
    connect(trayRestoreAction.get(), SIGNAL(triggered()), this, SLOT(showNormal()));
    trayMenu->addAction(trayRestoreAction.get());

    trayIcon->show();

    // handle autostart button
    ui->autostart->setChecked(updateAutostartPath());
    connect(ui->autostart, SIGNAL(stateChanged(int)), this, SLOT(checkboxStateChanged()));

    // handle start/stop timer
    connect(ui->startButton, SIGNAL(clicked()), this, SLOT(startButtonClicked()));
    connect(ui->stopButton, SIGNAL(clicked()), this, SLOT(stopButtonClicked()));

    // connect timer signals for this window
    connect(this, SIGNAL(startWorkSession()), this, SLOT(launch()));
    connect(this, SIGNAL(finished()), this, SLOT(workSessionFinished()));
    connect(this, SIGNAL(stopRequested()), this, SLOT(stop()));

    adjustSize();
}

void MainWindow::showHide(QSystemTrayIcon::ActivationReason r)
{
    if (r == QSystemTrayIcon::Trigger)
        {
            if (!this->isVisible()) {
                this->show();
            } else {
                this->hide();
            }
    }
}

void MainWindow::checkboxStateChanged()
{
    auto success = setAutostartEnabled(ui->autostart->isChecked());
    if (!success) { // uncheck if operation was not successful
        showMessage("Autostart is not implemented for this platform.");
        ui->autostart->setChecked(false);
    }
    qDebug() << "Autostart change result: " << isAutostartEnabled();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    m_nMouseClick_X_Coordinate = event->x();
    m_nMouseClick_Y_Coordinate = event->y();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    move(event->globalX() - m_nMouseClick_X_Coordinate, event->globalY() - m_nMouseClick_Y_Coordinate);
}



void MainWindow::updateCountdownDisplay(int msec)
{

    QTime time(0,0,0);
    time = time.addMSecs(msec);
    ui->progressBar->setValue(msec);
    if (!isHidden()){
        ui->progressBar->repaint();
    }
}

void MainWindow::fadeIn()
{
    // override to disable fading effects
    QTimer::singleShot(0, this, SLOT(startCountdown()));
}

void MainWindow::fadeOut()
{
    // override to disable fading effects
    QTimer::singleShot(0, this, SLOT(countdownFinished()));
}

void MainWindow::fadeOutNoFinished()
{
    // override to do nothing
}

void MainWindow::startButtonClicked()
{
    // get and parse user input
    auto workMinInput = ui->workMinutes->text();
    auto breakMinInput = ui->breakMinutes->text();

    bool workParsedOk, breakParsedOk;

    int minWorkParsed = workMinInput.toInt(&workParsedOk);
    if (workParsedOk) {
        workParsedOk = minWorkParsed >= 1 && minWorkParsed <= 900;
    }
    if (!workParsedOk) {
        showMessage("Set work minutes to be between 1 and 900.");
        return;
    }

    int minBreakParsed = breakMinInput.toInt(&breakParsedOk);
    if (breakParsedOk) {
        breakParsedOk = minBreakParsed >= 1 && minBreakParsed <= 300;
    }
    if (!breakParsedOk) {
        showMessage("Set break minutes to be between 1 and 300.");
        return;
    }

    // update timer values
    msecWork = minWorkParsed * 60 * 1'000;
    msecBreak = minBreakParsed * 60 * 1'000;
    setDuration(msecWork);
    ui->progressBar->setMaximum(msecWork);
    ui->progressBar->setMinimum(0);

    // block input
    ui->workMinutes->setEnabled(false);
    ui->breakMinutes->setEnabled(false);

    // block start button
    ui->startButton->setEnabled(false);
    ui->startButton->setVisible(false);

    // unlock stop button
    ui->stopButton->setEnabled(true);
    ui->stopButton->setVisible(true);

    // trigger session start
    breakSessionFinished();
}

void MainWindow::stopButtonClicked()
{
    emit stopRequested();

    // unlock input
    ui->workMinutes->setEnabled(true);
    ui->breakMinutes->setEnabled(true);

    // unlock start button
    ui->startButton->setEnabled(true);
    ui->startButton->setVisible(true);

    // block stop button
    ui->stopButton->setEnabled(false);
    ui->stopButton->setVisible(false);

    // reset progress bar
    ui->progressBar->setMinimum(0); // TODO: make separate function for this?
    ui->progressBar->setMaximum(100);
    ui->progressBar->setValue(0);

}

void MainWindow::workSessionFinished()
{
    qDebug() << __FILE__ << __LINE__ << "Work Session Finished";

    auto* win = new NotificationWindow("until break", msecPrepare); // no manual delete, window performs deletion on close
    connect(this, SIGNAL(startPreparationTimer()), win, SLOT(launch()));
    connect(win, SIGNAL(finished()), this, SLOT(preparationTimerFinished()));
    connect(this, SIGNAL(stopRequested()), win, SLOT(stop()));
    win->show();

    ui->progressBar->setValue(0);

    QSound::play(":/sounds/misc/notification.wav");

    emit startPreparationTimer();
}

void MainWindow::preparationTimerFinished()
{
    qDebug() << __FILE__ << __LINE__ << "Preparation Timer Finished";

    auto* win = new FullscreenTimer(msecBreak); // no manual delete, window performs deletion on close
    connect(this, SIGNAL(startBreakSession()), win, SLOT(launch()));
    connect(win, SIGNAL(finished()), SLOT(breakSessionFinished()));
    connect(this, SIGNAL(stopRequested()), win, SLOT(stop()));
    win->show();

    QSound::play(":/sounds/misc/drip.wav");

    emit startBreakSession();
}

void MainWindow::breakSessionFinished()
{
    qDebug() << __FILE__ << __LINE__ << "Break Session Finished";

    ui->progressBar->setValue(msecWork);

    emit startWorkSession();
}
