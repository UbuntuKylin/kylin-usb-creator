#include "page1.h"
#include "include/xatom-helper.h"

Page1::Page1()
{
    initControlQss();//初始化样式
    getStorageInfo();//获取磁盘信息
}

void Page1::initControlQss()
{
    tabIso=new QLabel;
    tabUdisk=new QLabel;
    tabIso->setText(tr("Choose iso file"));
    tabUdisk->setText(tr("Select USB drive"));
    tabIso->setFixedHeight(20);
    tabUdisk->setFixedHeight(20);
    tabIso->setObjectName("tabLable");
    tabUdisk->setObjectName("tabLable");
    comboUdisk = new QComboBox();
    warnningIcon=new QLabel;
    warnningIcon->setStyleSheet("border-image:url(:/data/warning.png);border:0px;");
    warnningIcon->setFixedSize(24,24);
    warnningText=new QLabel;
    urlIso=new QLineEdit;
    urlIso->setEnabled(false);
    urlIso->setFixedHeight(30);
    findIso=new QPushButton();
    findIso->setText(tr("Open"));
    findIso->setFixedSize(56,30);
    connect(findIso,&QPushButton::clicked,this,[=]{
            isoPath = QFileDialog::getOpenFileName(0,tr("choose iso file"),QDir::homePath(),"ISO(*.iso)");
            if(isoPath != "" ){
                if(!checkISO(isoPath)){
                    QMessageBox::StandardButton result =  QMessageBox::warning(this,tr("Warning"),tr("MBR signature not detected,continue anyway?"),
                                         QMessageBox::Yes | QMessageBox::No);
                    switch (result)
                    {
                    case QMessageBox::Yes:
                        break;
                    case QMessageBox::No:
                        isoPath.clear();
                        break;
                    }
                }
                urlIso->setToolTip("");
                if(isoPath.length() > 45){
                    urlIso->setToolTip(isoPath);
                    urlIso->setText(isoPath.mid(0,44) + "...");
                }else{
                    urlIso->setText(isoPath);
                }
            }
        });
    connect(urlIso,&QLineEdit::textChanged,this,&Page1::ifStartBtnChange);
    creatStart=new QPushButton(this);
    creatStart->setFixedSize(200,30);
    creatStart->setText(tr("Start"));
    creatStart->setEnabled(false);
    connect(creatStart,&QPushButton::clicked,[=]{
        QDBusMessage m = QDBusMessage::createMethodCall("com.kylinusbcreator.systemdbus","/",
                                                        "com.kylinusbcreator.interface","MakeStart");
        m<<isoPath;
        m<<diskInfos[comboUdisk->currentIndex()]->devicePath;
        QDBusConnection::systemBus().call(m);
    });

    QHBoxLayout *hl1=new QHBoxLayout;
    hl1->setMargin(0);
    hl1->setSpacing(0);
    hl1->addWidget(urlIso,9);
    hl1->addSpacing(10);
    hl1->addWidget(findIso,1);
    QHBoxLayout *hl2=new QHBoxLayout;
    hl2->setMargin(0);
    hl2->setSpacing(0);
    hl2->addWidget(warnningIcon,1);
    hl2->addSpacing(5);
    hl2->addWidget(warnningText,9);
    QHBoxLayout *hl3=new QHBoxLayout;
    hl3->setMargin(0);
    hl3->setSpacing(0);
    hl3->addStretch(9);
    hl3->addWidget(creatStart);
    hl3->addStretch(9);
    QVBoxLayout *vl0=new QVBoxLayout;
    vl0->setMargin(0);
    vl0->setSpacing(0);
    vl0->addStretch(4);
    vl0->addWidget(tabIso);
    vl0->addSpacing(10);
    vl0->addLayout(hl1);
    vl0->addStretch(3);
    vl0->addWidget(tabUdisk);
    vl0->addSpacing(10);
    vl0->addWidget(comboUdisk);
    vl0->addSpacing(10);
    vl0->addLayout(hl2);
    vl0->addStretch(8);
    vl0->addLayout(hl3);
    vl0->addStretch(2);
    QHBoxLayout *vl00=new QHBoxLayout;
    vl00->addStretch(3);
    vl00->addLayout(vl0,8);
    vl00->addStretch(3);
    this->setLayout(vl00);
    warnningText->setText(tr("USB drive will be formatted,please backup your files!"));
    udiskPlugWatcherInit(); //监控U盘插拔
}

void Page1::udiskPlugWatcherInit()
{
    diskRefreshDelay = new QTimer;
    connect(diskRefreshDelay,&QTimer::timeout,this,&Page1::getStorageInfo);
    udiskplugwatcher = new QFileSystemWatcher(this);
    udiskplugwatcher->addPath("/dev");
    connect(udiskplugwatcher,&QFileSystemWatcher::directoryChanged,this,&Page1::refreshDiskList);
}

void Page1::refreshDiskList()
{
    if(diskRefreshDelay->isActive())
    {
        return;
    }
    comboUdisk->clear();
    diskRefreshDelay->start(1000);
}

bool Page1::isCapicityAvailable(QString str)
{
    if(str[str.length()-1] == 'G')
    {
       str.chop(1);//去掉最后一位
       if(str.toFloat() < 65 && str.toFloat() > 4)
       {
           return true;
       }
    }
    return false;
}

bool
Page1::checkISO(const QString fileName){
    // Check if there's an MBR signature
    // MBR signature will be in last two bytes of the boot record
    QByteArray mbr;
    QFile mbrTest(fileName);
    mbrTest.open(QIODevice::ReadOnly);
    mbrTest.seek(510);
    mbr = mbrTest.read(2);
    mbrTest.close();
    if (mbr.toHex() != "55aa"){ //MBR signature "55aa"
        qDebug()<<"wrong iso,filename = "<<fileName<<"MBR signature = "<<mbr.toHex();
        return false;
    }
    return true;
}

void Page1::getUdiskPathAndCap()
{
    diskInfos.clear();
    QProcess lsblk;
    lsblk.start("lsblk -J");
    lsblk.waitForFinished();

    QProcess lsblk2;
    lsblk2.start("lsblk -JS");
    lsblk2.waitForFinished();
    QJsonArray arr1 = QStringToJsonArray(QString::fromLocal8Bit(lsblk.readAllStandardOutput()));  //获取json类型的shell执行结果
    QJsonArray arr2 = QStringToJsonArray(QString::fromLocal8Bit(lsblk2.readAllStandardOutput()));
    foreach (const QJsonValue& value, arr1) {
        QJsonObject jsonObj1 = value.toObject();
        foreach (const QJsonValue& value, arr2) {
            QJsonObject jsonObj2 = value.toObject();
            if(jsonObj1["name"] == jsonObj2["name"] && jsonObj2["tran"] == "usb" && jsonObj2["type"] == "disk"){
                AvailableDiskInfo *tmp = new AvailableDiskInfo("/dev/" + jsonObj1["name"].toString(),jsonObj2["model"].toString(),jsonObj1["size"].toString());
                diskInfos.append(tmp);
            }
        }
    }
}

QJsonArray  Page1::QStringToJsonArray(const QString jsonString){
    QJsonParseError err;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonString.toLocal8Bit().data(),&err);
    if(jsonDocument.isNull())
    {
        qWarning()<< "JsonDocument is NULL.Origin data is:"<< jsonString.toLocal8Bit().data();
    }
    if(err.error != QJsonParseError::NoError){
        qWarning()<<"Parase json"<<jsonString<<" error:"<<err.error;
    }
    QJsonObject obj = jsonDocument.object();
    return obj["blockdevices"].toArray();
}
void Page1::getUdiskName()
{
    QList<QStorageInfo> storageInfo = QStorageInfo::mountedVolumes();
    foreach(AvailableDiskInfo* tmp,diskInfos)  //lsblk命令拿到的可用U盘信息
    {
        foreach(QStorageInfo disk,storageInfo)
        {
            if((disk.device().length() == 8 && tmp->devicePath == disk.device()))
            {
                tmp->displayName = disk.displayName();
                float cap = disk.bytesTotal();
                tmp->diskCapicity = QString::number(cap/1000000000,'f',1) + 'G';
                continue;
            }
            if(tmp->devicePath + '1' == disk.device())  //使用第一个分区的名字做展示名
            {
                tmp->displayName = disk.displayName();
                tmp->diskCapicity = disk.bytesTotal();
                float cap = disk.bytesTotal();
                tmp->diskCapicity = QString::number(cap / 1000000000,'f',1) + 'G';
                continue;
            }
        }
    }
}

void Page1::getStorageInfo()
{
    diskRefreshDelay->stop();//U盘动态刷新相关
    getUdiskPathAndCap();
    foreach(AvailableDiskInfo *diskInfo,diskInfos)
    {
        //过长的名称做去尾加省略号
        if(diskInfo->displayName.length() > UDISK_NAME_MAX_LENGTH)
        {
            diskInfo->displayName = diskInfo->displayName.mid(0,UDISK_NAME_MAX_LENGTH - 1) + "…";
        }
        QString info = diskInfo->displayName+" ("+diskInfo->devicePath + ") " + diskInfo->diskCapicity;

        comboUdisk->addItem(info,diskInfo->devicePath);
        warnningIcon->show();
        warnningText->show();
    }

    if(0==comboUdisk->count())
    {
        comboUdisk->addItem(tr("No USB drive available"));
        warnningText->hide();
        warnningIcon->hide();
        creatStart->setEnabled(false);
    }
    emit diskLabelRefresh();
    ifStartBtnChange();
}

void Page1::creatStartSlots()
{
    creatStart->setEnabled(false);
}

bool Page1::ifStartBtnChange()
{
    if(comboUdisk->currentText() != tr("No USB drive available") && !urlIso->text().isEmpty())
    {
        creatStart->setEnabled(true);
        return true;
    }else{
        creatStart->setEnabled(false);
        return false;
    }
}

void Page1::dealDialogCancel()
{
    ifStartBtnChange();
}

void Page1::dealComboBoxChangeButton()
{
    ifStartBtnChange();
}

void Page1::dealAuthDialogClose()
{
    creatStart->setEnabled(true);
}

void Page1::setThemeStyleLight()
{
    themeStatus = LIGHTTHEME;
    tabIso->setStyleSheet("font-size:14px;color:rgba(0,0,0,1);");
    tabUdisk->setStyleSheet("font-size:14px;color:rgba(0,0,0,1);");
    warnningText->setStyleSheet("color:rgba(96, 98, 102, 1);font-size:14px;");
    ifStartBtnChange();
}

void Page1::setThemeStyleDark()
{
    themeStatus = DARKTHEME;
    tabUdisk->setStyleSheet("font-size:14px;color:rgba(249,249,249,1);");
    tabIso->setStyleSheet("font-size:14px;color:rgba(249,249,249,1);");
    warnningText->setStyleSheet("color:rgba(249, 249, 249, 1);font-size:14px;");
    ifStartBtnChange();
}

QString Page1::getDevPath(){
    return diskInfos[comboUdisk->currentIndex()]->devicePath;
}
