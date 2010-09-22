#include "systemsettings.h"

SystemSettings * SystemSettings::instance = 0;
int SystemSettings::instances = 0;

SystemSettings::SystemSettings()
{
    ++instances;
#ifdef Q_WS_MAEMO_5
    // Connect to DBUS of keyboard slider.
    QDBusConnection::systemBus().connect(QString("org.freedesktop.Hal"),
                                         DBUS_KEYBOARD_SLIDE,
                                         QString("org.freedesktop.Hal.Device"),
                                         QString("PropertyModified"),
                                         this, SLOT(slotKeyboardSlide()));
#endif
}

SystemSettings * SystemSettings::getInstance()
{
    if(instance == 0)
    {
        instance = new SystemSettings();
    }
    return instance;
}

void SystemSettings::slotKeyboardSlide()
{
    emit signalKeyboardClosed(getKeyboardClosed());
}

bool SystemSettings::getKeyboardClosed()
{
    bool closed = false;
#ifdef Q_WS_MAEMO_5
    QDBusInterface propertyInterface("org.freedesktop.Hal",
                    DBUS_KEYBOARD_SLIDE,
                    "org.freedesktop.Hal.Device",
                    QDBusConnection::systemBus());
    closed = propertyInterface.call("GetProperty", "button.state.value").arguments().at(0).toBool();
    qDebug() << "Keyboard is closed:" << closed;
#endif
    return closed;
}
