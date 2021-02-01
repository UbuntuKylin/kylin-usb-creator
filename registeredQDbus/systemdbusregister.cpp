#include "systemdbusregister.h"
#include <polkitqt1-authority.h>
#include <QDBusConnection>
#include <QDBusError>
#include <QDBusMessage>
#include <QDebug>

using namespace PolkitQt1;

SystemDbusRegister::SystemDbusRegister()
{

}

//excute dd command
void SystemDbusRegister::MakeStart(QString sourcePath,QString targetPath){
    //root authorization
    Authority::Result result;
    SystemBusNameSubject subject(message().service());
    result = Authority::instance()->checkAuthorizationSync("com.demo.systemdbus.exitservice",
             subject , Authority::AllowUserInteraction);
    if (result == Authority::No){
        //TODO: send authorization failed dbus message
        emit authorityFailed();
        return ;
    }

    uDiskPath = targetPath;
    QFileInfo info(sourcePath);
    sourceFileSize = info.size()/1000000;
    command_dd = new QProcess();
    connect(command_dd,&QProcess::readyReadStandardError,this,&SystemDbusRegister::readBashStandardErrorInfo);
    command_dd->start("bash");
    command_dd->waitForStarted();
    QString ddshell = "dd if='"+sourcePath.toLocal8Bit()+"' of="+targetPath.toLocal8Bit()+" status=progress";
    command_dd->write(ddshell.toLocal8Bit() + '\n');
    return ;
}

void SystemDbusRegister::readBashStandardErrorInfo()
{
    QByteArray cmdout = command_dd->readAllStandardError();
    if(!cmdout.isEmpty() && cmdout != "\r" && cmdout != "\n"){
        QString str = cmdout;
        qDebug()<<str;
        str = str.replace(" ","");
        if(str =="" || str.contains("[sudo]")) {return;}
        str = str.replace("\r","");
        QStringList bytes2 =  str.split("bytes");
         QString size_progress = bytes2.first();
         bool ok = false;
         qulonglong progress_num = size_progress.toDouble(&ok)/1048576;
         int mission_percent = progress_num*100/sourceFileSize;
         //send mission percent debus message every output
        emit workingProgress(mission_percent);
         if(bytes2.count() == 1 || !ok){
             finishEvent();
         }
    }
}

void SystemDbusRegister::finishEvent(){
    QTimer *diskRefreshDelay = new QTimer;
    connect(diskRefreshDelay,&QTimer::timeout,[=]{
        if(isMakingSucess()){
            //send production success dbus message
            emit makeFinish("success");
        }else{
            //send production failure dbus message
            emit makeFinish("fail");
        }
    });
    diskRefreshDelay->start(1000);
}

bool SystemDbusRegister::isMakingSucess()
{
    QList<QStorageInfo> diskList = QStorageInfo::mountedVolumes(); //mounted devices
    for(QStorageInfo& disk : diskList)
    {
        QString diskPath = disk.device();
        diskPath = diskPath.mid(0,8);
        if(uDiskPath == diskPath)
        {
            return true;
        }
    }
    return false;
}
