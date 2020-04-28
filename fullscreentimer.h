#ifndef FULLSCREENTIMER_H
#define FULLSCREENTIMER_H

#include <QWidget>

namespace Ui {
class FullscreenTimer;
}

class FullscreenTimer : public QWidget
{
    Q_OBJECT

public:
    explicit FullscreenTimer(QWidget *parent = nullptr);
    ~FullscreenTimer();

    void fadeIn();
    void fadeOut();
private:
    Ui::FullscreenTimer *ui;
};

#endif // FULLSCREENTIMER_H
