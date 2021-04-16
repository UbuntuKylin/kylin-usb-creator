#include <QCoreApplication>
#include <QDebug>
#include <QDBusConnection>
#include "systemdbusregister.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    a.setOrganizationName("Kylin Team");
    a.setApplicationName("system-bus-usbbootmaker-service");
    //regist service
    QDBusConnection systemBus = QDBusConnection::systemBus();
    if (!systemBus.registerService("com.usbbootmaker.systemdbus")){
        qCritical() << "QDbus register service failed reason:" << systemBus.lastError();
        exit(1);
    }
    //regist object
    if (!systemBus.registerObject("/", new SystemDbusRegister(), QDBusConnection::ExportAllSlots | QDBusConnection::ExportAllSignals)){
        qCritical() << "QDbus register object failed reason:" << systemBus.lastError();
        exit(2);
    }

    //regist signals

    qDebug() << "ok";

    return a.exec();
}
