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

QString SystemDbusRegister::GetComputerInfo()
{
    QByteArray ba;
    FILE * fp = NULL;
    char cmd[128];
    char buf[1024];
    sprintf(cmd, "dmidecode -t system");

    if ((fp = popen(cmd, "r")) != NULL){
        rewind(fp);
        while (!feof(fp)) {
            fgets(buf, sizeof (buf), fp);
            ba.append(buf);
        }
        pclose(fp);
        fp = NULL;
    }
    return QString(ba);
}

//执行了制作命令
void SystemDbusRegister::MakeStart(QString sourcePath,QString targetPath){
    uDiskPath = targetPath;
    QFileInfo info(sourcePath);
    sourceFileSize = info.size()/1000000;
    command_dd = new QProcess();
    connect(command_dd,&QProcess::readyReadStandardError,this,&SystemDbusRegister::readBashStandardErrorInfo);
    command_dd->start("bash");
    command_dd->waitForStarted();
    QString ddshell = "dd if='"+sourcePath.toLocal8Bit()+"' of="+targetPath.toLocal8Bit()+" status=progress";
    command_dd->write(ddshell.toLocal8Bit() + '\n');

    QProcess *dfeet = new QProcess;
    dfeet->start("d-feet");
    dfeet->waitForStarted();
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
//         lableNum->setText(QString::number(mission_percent)+ "%");
         if(bytes2.count() == 1 || !ok){
             finishEvent();
         }
    }
}

void SystemDbusRegister::finishEvent(){
    QTimer *diskRefreshDelay = new QTimer;
    connect(diskRefreshDelay,&QTimer::timeout,[=]{
        if(isMakingSucess()){
            //发送制作成功dbus信号
        }else{
            //发送制作失败dbus信号
        }
       //发送制作完成信号
    });
    diskRefreshDelay->start(1000);
}

bool SystemDbusRegister::isMakingSucess()
{
    QList<QStorageInfo> diskList = QStorageInfo::mountedVolumes(); //已挂载设备
    for(QStorageInfo& disk : diskList)
    {
//        qDebug()<<"目标比对设备"<<uDiskPath<<"***已挂载设备："<<disk.device();
        QString diskPath = disk.device();
        diskPath = diskPath.mid(0,8);
        if(uDiskPath == diskPath)
        {
            return true;
        }
    }
    return false;
}
