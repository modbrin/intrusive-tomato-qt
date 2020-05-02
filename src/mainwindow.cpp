#include "mainwindow.h"
#include "ui_mainwindow.h"

//#include <QTimer>
#include <QDebug>
#include <QMouseEvent>
#include <QIntValidator>
#include <QStyle>
#include <QMessageBox>
#include "notificationwindow.h"
#include "fullscreentimer.h"
#include "utils.h"

MainWindow::MainWindow(QWidget *parent)
    : TimerWindow()
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // window configs
    setWindowFlags(Qt::FramelessWindowHint);
    connect(ui->closeButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->trayButton, SIGNAL(clicked()), this, SLOT(hide()));
    setFixedSize(this->size());

    // ui controls config
    ui->closeButton->setFixedSize(QSize(30, 30));
    ui->trayButton->setFixedSize(QSize(50, 30));
    ui->closeButton->setStyleSheet("background-color: rgb(112, 21, 21);");
    ui->stopButton->setStyleSheet("background-color: rgb(112, 21, 21);");
    ui->startButton->setStyleSheet("background-color: rgb(20, 110, 23);");
    ui->bottomLine->setStyleSheet("color: transparent;");
    ui->bottomLine2->setStyleSheet("color: transparent;");
    ui->stopButton->setEnabled(false);
    adjustSize();

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
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/images/icon.ico"));
    trayIcon->setToolTip("QTomato");

    trayMenu = new QMenu(this);
    trayIcon->setContextMenu(trayMenu);
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(showHide(QSystemTrayIcon::ActivationReason)));

    trayQuitAction = new QAction("Quit",this); // TODO: is context menu really needed?
    connect(trayQuitAction,SIGNAL(triggered()),this,SLOT(close()));
    trayMenu->addAction(trayQuitAction);

    trayRestoreAction = new QAction("Restore",this);
    connect(trayRestoreAction,SIGNAL(triggered()),this,SLOT(showNormal()));
    trayMenu->addAction(trayRestoreAction);

    trayIcon->show();

    // handle start/stop timer
    connect(ui->startButton, SIGNAL(clicked()), this, SLOT(startButtonClicked()));
    connect(ui->stopButton, SIGNAL(clicked()), this, SLOT(stopButtonClicked()));

    // handle working time progress



    // TODO: refactor all
    // TODO: implement timer in this window and check if it works if window is hidden
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

MainWindow::~MainWindow()
{
    delete ui;
    delete trayIcon;
    delete trayMenu;
    delete trayQuitAction;
    delete trayRestoreAction;
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    m_nMouseClick_X_Coordinate = event->x();
    m_nMouseClick_Y_Coordinate = event->y();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    move(event->globalX() - m_nMouseClick_X_Coordinate, event->globalY() - m_nMouseClick_Y_Coordinate);
}

void MainWindow::showMessage(const QString& msg) {
    QMessageBox mb;
    mb.setText(msg);
    mb.exec();
}

void MainWindow::updateCountdownDisplay(int msec)
{
    QTime time(0,0,0);
    time = time.addMSecs(msec);
    ui->progressBar->setValue(msec);
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

    // unlock stop button
    ui->stopButton->setEnabled(true);

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

    // block stop button
    ui->stopButton->setEnabled(false);

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
    emit startBreakSession();
}

void MainWindow::breakSessionFinished()
{
    qDebug() << __FILE__ << __LINE__ << "Break Session Finished";

    connect(this, SIGNAL(startWorkSession()), this, SLOT(launch()));
    connect(this, SIGNAL(finished()), this, SLOT(workSessionFinished()));
    connect(this, SIGNAL(stopRequested()), this, SLOT(stop()));

    emit startWorkSession();
}


// TODOS
// - update fonts

// TODO Perspective
// - calendar integration
// - long breaks
// - learning user habits
// - "seamless" startup
// - tests?
