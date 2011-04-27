#ifndef SYSTEMSETTINGS_H
#define SYSTEMSETTINGS_H

#include <QObject>
#include <QDebug>
#include <QSettings>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusInterface>
#include "globals.h"

class SystemSettings : public QObject
{
    Q_OBJECT
public:
    static SystemSettings * getInstance();

    bool getKeyboardClosed();
    void saveCurrentList();
private:
    bool landscape;
    static SystemSettings * instance;
    static int instances;
    SystemSettings();

    QSettings * settings;

public slots:
    void slotKeyboardSlide();
signals:
    void signalKeyboardClosed(bool open);
};

#endif // SYSTEMSETTINGS_H
