#include <QCoreApplication>
#include <QDebug>
#include <ukui-log4qt.h>
#include <QDBusConnection>
#include "systemdbusregister.h"

int main(int argc, char *argv[])
{
    //init log module
    initUkuiLog4qt("usb-boot-maker-service");
    QCoreApplication a(argc, argv);
    a.setOrganizationName("Kylin Team");
    a.setApplicationName("system-bus-kylinusbcreator-service");
    //regist service
    QDBusConnection systemBus = QDBusConnection::systemBus();
    if (!systemBus.registerService("com.kylinusbcreator.systemdbus")){
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
