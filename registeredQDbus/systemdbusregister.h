#ifndef SYSTEMDBUSREGISTER_H
#define SYSTEMDBUSREGISTER_H

#include <QtDBus/QDBusContext>
#include <QObject>
#include <QCoreApplication>
#include <QProcess>
#include <QFile>
#include <QSettings>

class SystemDbusRegister : public QObject, protected QDBusContext
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.kylinusbcreator.interface")
public:
    SystemDbusRegister();
//    ~SystemDbusRegister();

signals:
public slots:
    Q_SCRIPTABLE QString GetComputerInfo();
};

#endif // SYSTEMDBUSREGISTER_H
