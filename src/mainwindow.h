#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QIcon>

#include "timerwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public TimerWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private: // containers
    Ui::MainWindow *ui;
    int msecWork = 15'000; // 1 minute default work session
    int msecBreak = 15'000; // 1 minutes default work session
    int msecPrepare = 30'000; // 30 seconds default prepare timer

private: // functions
    void showMessage(const QString& msg);
    virtual void updateCountdownDisplay(int msec) override;
    virtual void fadeIn() override;
    virtual void fadeOut() override;
    virtual void fadeOutNoFinished() override;

public slots:
    void startButtonClicked();
    void stopButtonClicked();
    void preparationTimerFinished();
    void breakSessionFinished();
    void workSessionFinished();
    void showHide(QSystemTrayIcon::ActivationReason);

signals:
    void startPreparationTimer();
    void startBreakSession();
    void startWorkSession();
    void stopRequested();

private: // handling dragging
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    int m_nMouseClick_X_Coordinate;
    int m_nMouseClick_Y_Coordinate;

private: // handling tray actions
    QSystemTrayIcon *trayIcon;
    QMenu *trayMenu;
    QAction *trayQuitAction;
    QAction *trayRestoreAction;


};
#endif // MAINWINDOW_H
