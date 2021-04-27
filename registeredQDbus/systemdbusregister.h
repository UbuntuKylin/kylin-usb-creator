#ifndef SYSTEMDBUSREGISTER_H
#define SYSTEMDBUSREGISTER_H

#include <QtDBus/QDBusContext>
#include <QObject>
#include <QCoreApplication>
#include <QProcess>
#include <QFile>
#include <QSettings>
#include <QFileInfo>
#include <QProcess>
#include <QTimer>
#include <QStorageInfo>
class SystemDbusRegister : public QObject, protected QDBusContext
{
    Q_OBJECT
//    Q_CLASSINFO("D-Bus Interface", "com.kylinusbcreator.interface")
    Q_CLASSINFO("D-Bus Interface", "com.kylinusbcreator.interface")
public:
    SystemDbusRegister();
//    ~SystemDbusRegister();

Q_SIGNALS:
    void workingProgress(int);
    void makeFinish(QString);
    void authorityStatus(QString);

public slots:
    Q_SCRIPTABLE void MakeStart(QString sourcePath,QString targetPath);
    Q_SCRIPTABLE void MakeExit();
private:
    void readBashStandardErrorInfo();
    void finishEvent();
    bool isMakingSucess();

private:
    QString uDiskPath = "";
    qint64 sourceFileSize = 0;
    QProcess *command_dd = nullptr;

};

#endif // SYSTEMDBUSREGISTER_H
