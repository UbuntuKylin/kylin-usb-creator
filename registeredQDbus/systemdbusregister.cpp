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
    result = Authority::instance()->checkAuthorizationSync("com.kylinusbcreator.systemdbus.authoritycheck",
             subject , Authority::AllowUserInteraction);
    if (result == Authority::Yes){
        //TODO: send authorization failed dbus message
        emit authorityStatus("success");
    }else{
        //there's two cases NO and Challenge
        emit authorityStatus("failed");
        return ;
    }
    if(!unmountDevice(targetPath)){
        emit makeFinish("unmount_error");
//        TODO:Deal sbus message unmount_error
//        return;
    }
    uDiskPath = targetPath;
    QFileInfo info(sourcePath);
    sourceFileSize = info.size()/1000000;
    command_dd = new QProcess();
    //指定输出语言
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("LANGUAGE","en_US:en");
    command_dd->setProcessEnvironment(env);
    connect(command_dd,&QProcess::readyReadStandardError,this,&SystemDbusRegister::readBashStandardErrorInfo);
    command_dd->start("bash");
    command_dd->waitForStarted();
    QString ddshell = "dd if='"+sourcePath.toLocal8Bit()+"' of="+targetPath.toLocal8Bit()+" status=progress";
    command_dd->write(ddshell.toLocal8Bit() + '\n');
    return ;
}

bool SystemDbusRegister::unmountDevice(QString target)
{
    QProcess unmount;
    unmount.start("udisksctl",QStringList{"unmount","-b",target+"1"});
    qDebug()<<"Start unmount disk:"<<target+"1";
    if(!unmount.waitForStarted()){
        qWarning()<<"unmount process start failed.device path:"<<target;
        return false;
    }
    if(!unmount.waitForFinished()){
        qWarning()<<"unmount process finish failed,device path:"<<target;
        return false;
    }
    if(QProcess::NormalExit == unmount.exitCode()){
        qDebug()<<"unmount device success.device path:"<<target;
        return true;
    }
    qWarning()<<"An unknow error occurred! unmount process exit code:"<<unmount.exitCode();
    return false;
}

//kill task process
void SystemDbusRegister::MakeExit(){
    qDebug()<<"exit dd process";
    if(!command_dd) return ;
    if(command_dd->Running){
        command_dd->kill();
    }
    return ;
}
void SystemDbusRegister::readBashStandardErrorInfo()
{
    QByteArray cmdout = command_dd->readAllStandardError();
    if(!cmdout.isEmpty() && cmdout != "\r" && cmdout != "\n"){
        QString str = cmdout;
        qDebug()<<str;
        str = str.replace(" ","");
//        if(str =="" || str.contains("[sudo]")) {return;}
        str = str.replace("\r","");
        QStringList bytes2 =  str.split("bytes");
         QString size_progress = bytes2.first();
         bool ok = false;
         qulonglong progress_num = size_progress.toDouble(&ok)/1048576;
         int mission_percent = progress_num*100/sourceFileSize;
         if(bytes2.count() == 1 || !ok){
              finishEvent();
         }
        if(!mission_percent){
            return ;
        }
         //send mission percent debus message every output
        emit workingProgress(mission_percent);

    }
}

void SystemDbusRegister::finishEvent(){
    QTimer *diskRefreshDelay = new QTimer;
    connect(diskRefreshDelay,&QTimer::timeout,[=]{
        diskRefreshDelay->stop();
        if(isMakingSucess()){
            //send production success dbus message
            qDebug()<<"make success";
            emit makeFinish("success");
        }else{
            //send production failure dbus message
            qDebug()<<"make failed";
            emit makeFinish("fail");
        }
    });
    diskRefreshDelay->start(2000);
}

bool SystemDbusRegister::isMakingSucess()
{
    QProcess lsblk;
    lsblk.start("lsblk -JS");
//    lsblk.waitForStarted();
    lsblk.waitForFinished();
//    qDebug()<<"lsblk output"<<lsblk.readAllStandardError();
    QJsonArray output = QStringToJsonArray(QString::fromLocal8Bit(lsblk.readAllStandardOutput()));
    foreach (const QJsonValue& value, output) {
        QString diskpath = value.toObject()["name"].toString();
        if("/dev/"+diskpath == uDiskPath) return true;
    }
//    for(QStorageInfo& disk : diskList)
//    {
//        QString diskPath = disk.device();
//        diskPath = diskPath.mid(0,8);
//        if(uDiskPath == diskPath)
//        {
//            return true;
//        }
//    }
    return false;
}
QJsonArray SystemDbusRegister::QStringToJsonArray(const QString jsonString){
    QJsonParseError err;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonString.toLocal8Bit().data(),&err);
    if(jsonDocument.isNull())
    {
        qDebug()<< "JsonDocument is NULL.Origin data is:"<< jsonString.toLocal8Bit().data();
    }
    if(err.error != QJsonParseError::NoError){
        qDebug()<<"Parase json"<<jsonString<<" error:"<<err.error;
        //TODO：这里的错误处理后期还可以优化,目前处理错误了就会调用exit()退出程序
//        exit(-1);
    }
    QJsonObject obj = jsonDocument.object();
    return obj["blockdevices"].toArray();
}
