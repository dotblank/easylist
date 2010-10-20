#ifndef SYSTEMSETTINGS_H
#define SYSTEMSETTINGS_H

#include <QObject>
#include <QDebug>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusInterface>
#include "globals.h"

class SystemSettings : public QObject
{
    Q_OBJECT
public:
    static SystemSettings * getInstance();

    bool getKeyboardClosed();
private:
    bool landscape;
    static SystemSettings * instance;
    static int instances;
    SystemSettings();

public slots:
    void slotKeyboardSlide();
signals:
    void signalKeyboardClosed(bool open);
};

#endif // SYSTEMSETTINGS_H
