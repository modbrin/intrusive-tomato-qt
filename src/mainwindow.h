#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QIcon>
#include <QMediaPlayer>
#include <QAudioOutput>
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
    QMediaPlayer* m_player;
    QAudioOutput* m_audioOutput;
    int msecWork = 15'000; // 1 minute default work session
    int msecBreak = 15'000; // 1 minutes default work session
    int msecPrepare = 20'000; // 20 seconds default prepare timer

private: // functions
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
    void checkboxStateChanged();

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
    std::unique_ptr<QSystemTrayIcon> trayIcon;
    std::unique_ptr<QMenu> trayMenu;
    std::unique_ptr<QAction> trayQuitAction;
    std::unique_ptr<QAction> trayRestoreAction;
};
#endif // MAINWINDOW_H
