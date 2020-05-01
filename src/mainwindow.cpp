#include "mainwindow.h"
#include "ui_mainwindow.h"

//#include <QTimer>
#include <QDebug>
#include <QMouseEvent>
#include <QIntValidator>
#include <QStyle>
#include "notificationwindow.h"
#include "fullscreentimer.h"
#include "utils.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
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
    adjustSize();

    ui->workMinutes->setValidator( new QIntValidator(1, 900, this) );
    ui->breakMinutes->setValidator( new QIntValidator(1, 300, this) );
    ui->workMinutes->setText(QString::number(35));
    ui->breakMinutes->setText(QString::number(5));

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
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    m_nMouseClick_X_Coordinate = event->x();
    m_nMouseClick_Y_Coordinate = event->y();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    move(event->globalX() - m_nMouseClick_X_Coordinate, event->globalY() - m_nMouseClick_Y_Coordinate);
}

void MainWindow::startButtonClicked()
{
    // check input validity
    // update timer values
    // block input
    // block start button
    // unlock stop button
    breakSessionFinished();
}

void MainWindow::stopButtonClicked()
{
    // TODO: throw stop signal to timers
}

void MainWindow::workSessionFinished()
{
    qDebug() << __FILE__ << __LINE__ << "Work Session Finished";

    auto* win = new NotificationWindow("until break", msecPrepare); // no manual delete, window performs deletion on close
    connect(this, SIGNAL(startPreparationTimer()), win, SLOT(launch()));
    connect(win, SIGNAL(finished()), this, SLOT(preparationTimerFinished()));

    win->show();
    emit startPreparationTimer();
}

void MainWindow::preparationTimerFinished()
{
    qDebug() << __FILE__ << __LINE__ << "Preparation Timer Finished";

    auto* win = new FullscreenTimer(msecBreak); // no manual delete, window performs deletion on close
    connect(this, SIGNAL(startBreakSession()), win, SLOT(launch()));
    connect(win, SIGNAL(finished()), SLOT(breakSessionFinished()));

    win->show();
    emit startBreakSession();
}

void MainWindow::breakSessionFinished()
{
    qDebug() << __FILE__ << __LINE__ << "Break Session Finished";

    auto* win = new NotificationWindow("work session", msecWork); // no manual delete, window performs deletion on close

    connect(this, SIGNAL(startWorkSession()), win, SLOT(launch()));
    connect(win, SIGNAL(finished()), this, SLOT(workSessionFinished()));

    win->show();
    emit startWorkSession();
}


// TODOS
// - start work timer
// - work timer
// - minimize to tray

// TODO Perspective
// - calendar integration
// - long breaks
// - learning user habits
// - "seamless" startup
// - tests?
