#include "mainwindow.h"
#include "ui_mainwindow.h"

//#include <QTimer>
#include "notificationwindow.h"
#include "intrusivewindow.h"
#include "fullscreentimer.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    auto* win = new NotificationWindow();
    win->show();

//    auto* full = new FullscreenTimer();
//    full->show();

    //showMinimized();
}

MainWindow::~MainWindow()
{
    delete ui;
}


// TODOS
// notification window size
// notification positioning
// timer slots
// button in timer
// counting time
