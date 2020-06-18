#ifndef QTOMATO_UTILS_H
#define QTOMATO_UTILS_H

#include <QWidget>
#include <QLineEdit>

void enableWindowBackgroundBlur(QWidget* w);

bool updateAutostartPath();

bool setAutostartEnabled(bool state);

bool isAutostartEnabled();

void showMessage(const QString& msg);

#endif // QTOMATO_UTILS_H
