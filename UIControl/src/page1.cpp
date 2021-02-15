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
    comboUdisk=new StyleComboBox();
    connect(this,&Page1::diskLabelRefresh,comboUdisk,&StyleComboBox::dealDiskLabelRefresh);
    connect(comboUdisk,&StyleComboBox::ifStartButtonChange,this,&Page1::dealComboBoxChangeButton);
    warnningIcon=new QLabel;
    warnningIcon->setStyleSheet("border-image:url(:/data/warning.png);border:0px;");
    warnningIcon->setFixedSize(24,24);
    warnningText=new QLabel;
    urlIso=new QLineEdit;
    urlIso->setEnabled(false);
    urlIso->setFixedHeight(30);
    findIso=new QPushButton(this);
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
//    connect(creatStart,&QPushButton::clicked,this,&Page1::creatStartSlots);
    connect(creatStart,&QPushButton::clicked,[=]{/*
        emit makeStart(isoPath,comboUdisk->getDiskPath());*/
        QDBusMessage m = QDBusMessage::createMethodCall("com.kylinusbcreator.systemdbus","/",
                                                        "com.kylinusbcreator.interface","MakeStart");
        m<<isoPath;m<<comboUdisk->getDiskPath();
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
    comboUdisk->clearDiskList();
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

//TODO：设备类型判断，搭配lsblk -S只加入走USB协议的设备
void Page1::getUdiskPathAndCap()
{
    diskInfos.clear();
    QStringList diskInfo;
    QString disk;
    QProcess lsblk;
    lsblk.start("lsblk");
    lsblk.waitForFinished();
    QString info = QString::fromLocal8Bit(lsblk.readAllStandardOutput());
    foreach(disk,info.split("\n"))
    {
        if(disk == NULL)
        {
            continue;
        }
        disk.replace(QRegExp("[\\s]+")," ");
        diskInfo = disk.split(" ");
        if(diskInfo.at(5) == "disk"){
            if(!isCapicityAvailable(diskInfo.at(3)))
            {
                continue;
            }
            AvailableDiskInfo *tmp = new AvailableDiskInfo("/dev/" + diskInfo.at(0),"NONAME",diskInfo.at(3));
            diskInfos.append(tmp);
        }
    }
}

void Page1::getUdiskName()
{
    QList<QStorageInfo> storageInfo = QStorageInfo::mountedVolumes();
    foreach(AvailableDiskInfo* tmp,diskInfos)  //lsblk命令拿到的可用U盘信息
    {
        qDebug()<<tmp->displayName<<"****";
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
//                uchardet_t* type = new uchardet_t();
//                type->uchardet_handle_data()
//                QString target;
//                QByteArray str = disk.displayName().toLatin1();
//                QByteArray tmp = str.toLatin1();
//                QTextCodec::ConverterState state;
//                target = QTextCodec::codecForName("UTF-8")->toUnicode(str.constData(),str.size(),&state);
//                qDebug()<<state.invalidChars;
//                if(state.invalidChars > 0){
//                    qDebug()<<state.invalidChars;
//                    target = QTextCodec::codecForName("GBK")->toUnicode(str.constData(),str.size(),&state);
//                    if(state.invalidChars > 0){
//                        qDebug()<<"不支持的编码集";
//                    }
//                }
//                target.replace("\n","");
//                qDebug()<<target;
//                QTextCodec *utf8 = QTextCodec::codecForName("UTF-8");
//                QTextCodec::setCodecForLocale(utf8);
//                QTextCodec::setCodecForCStrings(utf8);
//                QTextCodec* gbk = QTextCodec::codecForName("gbk");
//                char *p = str.toLocal8Bit().data();

//                QString strUnicode=gbk->toUnicode(p);
//                QByteArray utf8_bytes=utf8->fromUnicode(strUnicode);
//                p = utf8_bytes.data();
//                qDebug()<<QString(QLatin1String(p));
            }
        }
    }
}

void Page1::getStorageInfo()
{
    diskRefreshDelay->stop();//U盘动态刷新相关
    getUdiskPathAndCap();
    getUdiskName();
    foreach(AvailableDiskInfo *diskInfo,diskInfos)
    {
        //过长的名称做去尾加省略号
        if(diskInfo->displayName.length() > UDISK_NAME_MAX_LENGTH)
        {
            diskInfo->displayName = diskInfo->displayName.mid(0,UDISK_NAME_MAX_LENGTH - 1) + "…";
        }
        QString info = diskInfo->displayName+" ("+diskInfo->devicePath + ") " + diskInfo->diskCapicity;

        comboUdisk->addItem(info,diskInfo->devicePath);
//        有分区就向第一分区里写，没分区就直接向块设备写,该方法暂时停止使用——有文件系统但是没有分区的U盘会导致制作失败向不存在的sd*1设备中写入
//        if(diskInfo->displayName == "unknowname")
//        {
//            comboUdisk->addItem(info,diskInfo->devicePath);
//        }else{
//            comboUdisk->addItem(info,diskInfo->devicePath + '1');
//        }

        warnningIcon->show();
        warnningText->show();
    }

    if(0==comboUdisk->listWidget->count())
    {
        comboUdisk->addItem(tr("No USB drive available"),NOUDISK);
        warnningText->hide();
        warnningIcon->hide();
        creatStart->setEnabled(false);
    }
    emit diskLabelRefresh();
    ifStartBtnChange();
}

void Page1::allClose()
{
    comboUdisk->closeListWidget();
}

void Page1::creatStartSlots()
{
    creatStart->setEnabled(false);
    if(DARKTHEME == themeStatus)
    {
        creatStart->setStyleSheet("background-color:rgba(48,49,51,1);color:rgba(249,249,249,1);border-radius:15px;font-size:14px;");

    }else
    {
        creatStart->setStyleSheet("background-color:rgba(242,242,242,1);color:rgba(193,193,193,1);border-radius:15px;font-size:14px;");
    }
}

bool Page1::event(QEvent *event)
{
    if(comboUdisk->listWidget == nullptr)
    {
        return QWidget::event(event);
    }
    if (event->type() == QEvent::Leave)
    {
        if(mouseIsLeaveUdiskWidget())
        {
            comboUdisk->closeListWidget();
        }
    }
    else if (event->type() == QEvent::MouseButtonPress)
    {
        comboUdisk->closeListWidget();
    }
    return QWidget::event(event);
}

bool Page1::mouseIsLeaveUdiskWidget()
{
    QPoint mouse=QCursor::pos();
    QPoint thisWidget=this->mapToGlobal(this->pos());
    QSize thisWidgetSize=this->size();
    if(mouse.rx()<=thisWidget.rx() || mouse.rx()>=thisWidget.rx()+thisWidgetSize.width() || mouse.ry()<=thisWidget.ry() || mouse.ry()>=thisWidget.ry()+thisWidgetSize.height())
        return true;
    else
        return false;
}

bool Page1::ifStartBtnChange()
{
    if(comboUdisk->getDiskPath() != NOUDISK && !urlIso->text().isEmpty())
    {
        creatStart->setEnabled(true);
        creatStart->setStyleSheet("QPushButton{background-color:rgba(100,105,241,1);color:rgba(249,249,249,1);border-radius:15px;font-size:14px;}"
                                  "QPushButton:hover{background-color:rgba(130,140,255,1);color:rgba(249,249,249,1);border-radius:15px;font-size:14px;}"
                                  "QPushButton:pressed{background-color:rgba(82,87,217,1);color:rgba(249,249,249,1);border-radius:15px;font-size:14px;}");
        return true;
    }else{
        creatStart->setEnabled(false);
        if(themeStatus == LIGHTTHEME)
        {
            creatStart->setStyleSheet("background-color:rgba(242,242,242,1);color:rgba(193,193,193,1);border-radius:15px;font-size:14px;");
        }else
        {
            creatStart->setStyleSheet("background-color:rgba(48,49,51,1);color:rgba(249,249,249,1);border-radius:15px;font-size:14px;");
        }
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

void Page1::dealRightPasswd()
{
    emit makeStart(isoPath,comboUdisk->getDiskPath());

}
void Page1::dealAuthDialogClose()
{
    creatStart->setEnabled(true);
    creatStart->setStyleSheet("QPushButton{background-color:rgba(100,105,241,1);color:rgba(249,249,249,1);border-radius:15px;font-size:14px;}"
                              "QPushButton:hover{background-color:rgba(130,140,255,1);color:rgba(249,249,249,1);border-radius:15px;font-size:14px;}"
                              "QPushButton:pressed{background-color:rgba(82,87,217,1);color:rgba(249,249,249,1);border-radius:15px;font-size:14px;}");
}

void Page1::setThemeStyleLight()
{
    themeStatus = LIGHTTHEME;
    tabIso->setStyleSheet("font-size:14px;color:rgba(0,0,0,1);");
    tabUdisk->setStyleSheet("font-size:14px;color:rgba(0,0,0,1);");
    warnningText->setStyleSheet("color:rgba(96, 98, 102, 1);font-size:14px;");
    creatStart->setStyleSheet("background-color:rgba(236,236,236,1);color:rgba(249,249,249,1);border-radius:15px;font-size:14px;");
    ifStartBtnChange();
    this->setStyleSheet(".QPushButton{background-color:rgba(100, 105, 241, 1);color:#fff;border-radius:4px;}"
                        ".QPushButton:hover{background-color:rgba(136,140,255,1);}"
                        ".QPushButton:pressed{background-color:rgba(82,87,217,1);}");
    findIso->setStyleSheet(".QPushButton{background-color:rgba(240, 240, 240, 1);color:rgba(96,98,101,1);border-radius:4px;font-size:14px;}"
                           ".QPushButton:hover{background-color:rgba(136,140,255,1);color:#fff;}"
                           ".QPushButton:pressed{background-color:rgba(82,87,217,1);color:#fff;}");
    urlIso->setStyleSheet("background-color:rgba(240,240,240,1);color:rgba(96,98,101,1);font-size:12px;border:1px  solid rgba(240,240,240,1);border-radius:4px;");


    this->setStyleSheet("background-color:rgba(255,255,255,1);");
    comboUdisk->setThemeLight();    //设置combobox响应浅色主题
    emit setStyleWidgetStyle(LIGHTTHEME);   //设置stylewidget响应浅色主题
}

void Page1::setThemeStyleDark()
{
    themeStatus = DARKTHEME;
    tabUdisk->setStyleSheet("color:rgba(249,249,249,1);");
    tabIso->setStyleSheet("font-size:14px;color:rgba(249,249,249,1);");
    warnningText->setStyleSheet("color:rgba(249, 249, 249, 1);font-size:14px;");
    creatStart->setStyleSheet("background-color:rgba(48,49,51,1);color:rgba(249,249,249,1);border-radius:15px;font-size:14px;");
    ifStartBtnChange();
    this->setStyleSheet("background-color:rgba(31,32,34,1);");

    findIso->setStyleSheet(".QPushButton{background-color:rgba(47, 48, 50, 1);;color:rgba(200,200,200,1);border-radius:4px;font-size:14px;}"
                           ".QPushButton:hover{background-color:rgba(136,140,255,1);color:#fff;}"
                           ".QPushButton:pressed{background-color:rgba(82,87,217,1);color:#fff;}");
    urlIso->setStyleSheet("background-color:rgba(47, 48, 50, 1);color:rgba(200,200,200,1);font-size:12px;");


    comboUdisk->setThemeDark(); //设置combobox响应深色主题
    this->setStyleSheet("background-color:rgba(31,32,34,1);");
    emit setStyleWidgetStyle(DARKTHEME); //设置stylewidget响应黑色主题
}
