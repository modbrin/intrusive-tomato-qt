#ifndef QTOMATO_UTILS_H
#define QTOMATO_UTILS_H

#include <QWidget>
#include <QLineEdit>

void enableWindowBackgroundBlur(QWidget* w);

bool updateAutostartPath();

void setAutostartEnabled(bool state);

bool isAutostartEnabled();

#endif // QTOMATO_UTILS_H
