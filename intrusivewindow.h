#ifndef INTRUSIVEWINDOW_H
#define INTRUSIVEWINDOW_H

#include <QWidget>

class IntrusiveWindow : public QWidget
{
    Q_OBJECT
public:
    explicit IntrusiveWindow(QWidget *parent = nullptr);

signals:

};

#endif // INTRUSIVEWINDOW_H
