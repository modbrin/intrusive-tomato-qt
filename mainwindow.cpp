#include "mainwindow.h"
#include "ui_mainwindow.h"

//#include <QTimer>
#include <QDebug>
#include "notificationwindow.h"
#include "intrusivewindow.h"
#include "fullscreentimer.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //showMinimized();

    breakSessionFinished();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::workSessionFinished()
{
    qDebug() << __FILE__ << __LINE__ << "Work Session Finished";

    //return;

    auto* win = new NotificationWindow("until break", 5'000); // no manual delete, window performs deletion on close

    connect(this, SIGNAL(startPreparationTimer()), win, SLOT(launch()));
    connect(win, SIGNAL(finished()), this, SLOT(preparationTimerFinished()));

    win->show();
    emit startPreparationTimer();
}

void MainWindow::preparationTimerFinished()
{
    qDebug() << __FILE__ << __LINE__ << "Preparation Timer Finished";

    auto* win = new FullscreenTimer(10'000); // no manual delete, window performs deletion on close
    connect(this, SIGNAL(startBreakSession()), win, SLOT(launch()));
    connect(win, SIGNAL(finished()), SLOT(breakSessionFinished()));

    win->show();
    emit startBreakSession();
}

void MainWindow::breakSessionFinished()
{
    qDebug() << __FILE__ << __LINE__ << "Break Session Finished";

    auto* win = new NotificationWindow("work session", 10'000); // no manual delete, window performs deletion on close

    connect(this, SIGNAL(startWorkSession()), win, SLOT(launch()));
    connect(win, SIGNAL(finished()), this, SLOT(workSessionFinished()));

    win->show();
    emit startWorkSession();
}




// TODOS
// - button in timer
// - work timer
// - whole cycle
// - ui for settings
// - minimize to tray

// TODO Perspective
// - calendar integration
// - long breaks
// - learning user habits
// - "seamless" startup
// - tests?
