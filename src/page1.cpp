#include "page1.h"
#include "include/xatom-helper.h"


Page1::Page1()
{
    initControlQss();//初始化样式
    dialogInitControlQss();
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
                urlIso->setText(isoPath);
            }
        });
    connect(urlIso,&QLineEdit::textChanged,this,&Page1::ifStartBtnChange);
    creatStart=new QPushButton(this);
    creatStart->setFixedSize(200,30);
    creatStart->setText(tr("Start"));
    creatStart->setEnabled(false);
    connect(creatStart,&QPushButton::clicked,this,&Page1::creatStartSlots);
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

void Page1::dialogInitControlQss()
{
    authDialog = new rootAuthDialog(this);
    authDialog->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    authDialog->setAttribute(Qt::WA_TranslucentBackground); //全透明

    authDialogContentWidget = new QWidget(authDialog);
    QHBoxLayout *lytMain =new QHBoxLayout(authDialog);
    lytMain->setMargin(0);
    lytMain->addWidget(authDialogContentWidget);
    authDialog->setFixedSize(430,270);
    authDialogContentWidget->setFixedSize(424,264);
    // 在非顶级窗口设置阴影效果
    shadowEffect = new QGraphicsDropShadowEffect();
    shadowEffect->setOffset(0,0);
    shadowEffect->setBlurRadius(12);

//    授权窗口在屏幕中央显示
    QRect availableGeometry = qApp->primaryScreen()->availableGeometry();
    authDialog->move((availableGeometry.width() - authDialog->width())/2, (availableGeometry.height() - authDialog->height())/2);
    connect(authDialog,&rootAuthDialog::passwdCorrect,this,&Page1::dealRightPasswd);
    connect(authDialog,&rootAuthDialog::cancelCheck,[=]{
        authDialog->dialogKey->clear();
        isAuthDialogShowing = false;
        authDialog->close();
        ifStartBtnChange();
    });
    authDialog->hide();
    authDialog->btnOk->setFixedSize(64,30);
    authDialog->btnOk->setText(tr("OK"));
    authDialog->btnOk->setStyleSheet("font-size:14px;");
    authDialog->btnOk->setObjectName("dialogYes");
    authDialog->btnCancel->setFixedSize(64,30);
    authDialog->btnCancel->setText(tr("Cancel"));
    authDialog->btnCancel->setObjectName("dialogNo");

//    标题栏部分样式设置
    rootWindowTitle = new QWidget;
    rootWindowTitle->setFixedSize(424,38);
    rootWindowTitle->setObjectName("title");
    rootDialogClose = new QPushButton(rootWindowTitle);
    rootDialogClose->setFixedSize(30,30);
//    connect(rootDialogClose,&QPushButton::clicked,authDialog,&rootAuthDialog::close);
    connect(rootDialogClose,&QPushButton::clicked,[=](){
        authDialog->close();
        isAuthDialogShowing = false;
    });
    connect(rootDialogClose,&QPushButton::clicked,[=]{ifStartBtnChange();});
    rootDialogMin = new QPushButton(rootWindowTitle);
    rootDialogMin->setFixedSize(30,30);
    connect(rootDialogMin,&QPushButton::clicked,[=]{
        authDialog->showMinimized();
        ifStartBtnChange();
    });
    rootDialogTitleText = new QLabel(rootWindowTitle);//标题
    rootDialogTitleText->setFixedSize(170,18);

    rootDialogTitleText->setText(tr("Authorization"));
    QHBoxLayout *titlelyt0=new QHBoxLayout();//右上角按钮内部
    titlelyt0->setMargin(0);
    titlelyt0->setSpacing(0);
    titlelyt0->addStretch(99);
    titlelyt0->addWidget(rootDialogMin,1);
    titlelyt0->addSpacing(4);
    titlelyt0->addWidget(rootDialogClose,1);
    titlelyt0->addSpacing(4);
    QVBoxLayout *titlelyt1=new QVBoxLayout();//右上角按钮外部
    titlelyt1->setMargin(0);
    titlelyt1->setSpacing(0);
    titlelyt1->addSpacing(4);
    titlelyt1->addLayout(titlelyt0);
    titlelyt1->addSpacing(4);
    QHBoxLayout *titlelyt2=new QHBoxLayout(); //标题内部
    titlelyt2->setMargin(0);
    titlelyt2->setSpacing(0);
    titlelyt2->addSpacing(16);
    titlelyt2->addWidget(rootDialogTitleText);
    QVBoxLayout *titlelyt3 = new QVBoxLayout(); //标题外部
    titlelyt3->setMargin(0);
    titlelyt3->setSpacing(0);
    titlelyt3->addSpacing(10);
    titlelyt3->addLayout(titlelyt2);
    titlelyt3->addStretch(99);
    QHBoxLayout *titlelyt4=new QHBoxLayout();//标题栏
    titlelyt4->setMargin(0);
    titlelyt4->setSpacing(0);
    QLabel *fill = new QLabel(); //填充label
    fill->setFixedHeight(38);
    titlelyt4->addLayout(titlelyt3);
    titlelyt4->addWidget(fill);
    titlelyt4->addStretch();
    titlelyt4->addLayout(titlelyt1);

    divingLine = new QLabel;
    divingLine->setFixedSize(424,1);
    dialogWarningIcon = new QLabel();
    dialogWarningIcon->setStyleSheet("border-image:url(:data/warning.png);border:0px;");
    dialogWarningIcon->setFixedSize(24,24);
    dialogWarningLable=new QLabel();
    dialogWarningLable->setText(tr("These operations needs to be verified:"));
    dialogWarningLable2=new QLabel();
    dialogWarningLable2->setText(tr("Request authorization:"));
    dialogWarningLable2->setWordWrap(true);
    dialogKeyLable=new QLabel();
    dialogKeyLable->setText(tr("Password："));
    authDialog->dialogKey->setFixedSize(296,32);

    QHBoxLayout *hlt1=new QHBoxLayout();
    hlt1->setMargin(0);
    hlt1->setSpacing(0);
    hlt1->addSpacing(19);
    hlt1->addWidget(dialogWarningIcon);
    hlt1->addSpacing(4);
    hlt1->addWidget(dialogWarningLable);
    hlt1->addStretch(9);
    QHBoxLayout *hlt2=new QHBoxLayout();
    hlt2->setMargin(0);
    hlt2->setSpacing(0);
    hlt2->addSpacing(44);
    hlt2->addWidget(dialogWarningLable2);
    hlt2->addSpacing(16);
    QHBoxLayout *hlt3=new QHBoxLayout();
    hlt3->setMargin(0);
    hlt3->setSpacing(0);
    hlt3->addSpacing(44);
    hlt3->addWidget(dialogKeyLable);
    hlt3->addSpacing(6);
    hlt3->addWidget(authDialog->dialogKey);
    hlt3->addSpacing(16);
    QHBoxLayout *hlt4=new QHBoxLayout();
    hlt4->setMargin(0);
    hlt4->setSpacing(0);
    hlt4->addStretch(2);
    hlt4->addWidget(authDialog->btnCancel);
    hlt4->addSpacing(16);
    hlt4->addWidget(authDialog->btnOk);
    hlt4->addSpacing(22);
    QVBoxLayout *vlt1=new QVBoxLayout();
    vlt1->setMargin(0);
    vlt1->setSpacing(0);
    vlt1->addLayout(titlelyt4);
    vlt1->addWidget(divingLine);
    vlt1->addSpacing(10);
    vlt1->addLayout(hlt1);
    vlt1->addSpacing(16);
    vlt1->addLayout(hlt2);
    vlt1->addSpacing(16);
    vlt1->addLayout(hlt3);
    vlt1->addSpacing(31);
    vlt1->addLayout(hlt4);
    vlt1->addSpacing(24);
    authDialogContentWidget->setLayout(vlt1);
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

//TODO：容量获取方法更新，目前获取的是1024进制单位。计划在将来改为和文件管理器一致的1000进位
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
    authDialog->close();
    comboUdisk->closeListWidget();
}

void Page1::creatStartSlots()
{
    isAuthDialogShowing = true;
    creatStart->setEnabled(false);
    if(DARKTHEME == themeStatus)
    {
        creatStart->setStyleSheet("background-color:rgba(48,49,51,1);color:rgba(249,249,249,1);border-radius:15px;font-size:14px;");

    }else
    {
        creatStart->setStyleSheet("background-color:rgba(242,242,242,1);color:rgba(193,193,193,1);border-radius:15px;font-size:14px;");
    }

    authDialog->dialogKey->setStyleSheet("QLineEdit{border:1px solid rgba(221, 223, 231, 1);font-size:14px;}");
    authDialog->dialogKey->setPlaceholderText("请输入密码");
    authDialog->dialogKey->clear();
    authDialog->show();
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
    if(isAuthDialogShowing)
    {
        return false;
    }
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
    isAuthDialogShowing = false;
    emit makeStart(authDialog->dialogKey->text(),urlIso->text(),comboUdisk->getDiskPath());
    if(themeStatus == LIGHTTHEME)
    {
        authDialog->dialogKey->setStyleSheet("QLineEdit{border:1px solid rgba(221, 223, 231, 1);font-size:14px;}");
    }else{
        authDialog->dialogKey->setStyleSheet("QLineEdit{border:1px solid #606265;font-size:14px;color:rgba(143,147,153,1);}");
    }
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
//    root授权框部分
    rootDialogTitleText->setStyleSheet("font-size:14px;font-weight:600;color:rgba(27,27,27,1);");
    rootDialogClose->setStyleSheet("QPushButton{background-color:rgba(255,255,255,0);border-image:url(:/data/close_d.png);border-radius:4px;}"
                                    "QPushButton:hover{background-color:rgba(253,149,149,1);border-image:url(:/data/close_h.png);border-radius:4px;}"
                                    "QPushButton:pressed{background-color:rgba(237,100,100,1);border-image:url(:/data/close_h.png);border-radius:4px;}");
    rootDialogMin->setStyleSheet("QPushButton{background-color:rgba(255,255,255,0);border-image:url(:/data/min_d.png);border-radius:4px;}"
                                 " QPushButton:hover{background-color:rgba(0,0,0,0.04);border-image:url(:/data/min_d.png);border-radius:4px;}"
                                 "QPushButton:pressed{background-color:rgba(0,0,0,0.08);border-image:url(:/data/min_d.png);border-radius:4px;}");
    authDialog->dialogKey->setStyleSheet("QLineEdit{border:1px solid rgba(221, 223, 231, 1);font-size:14px;}");
//                                         "QLineEdit:hover{border:1px solid rgba(100,105, 241, 1);font-size:14px;}");
    shadowEffect->setColor(Qt::lightGray);
    authDialogContentWidget->setGraphicsEffect(shadowEffect);
    dialogKeyLable->setStyleSheet("color:rgba(48,49,51,1);font-size:14px;");
    divingLine->setStyleSheet("background-color:rgba(242,246,253,1);");
    dialogWarningLable->setStyleSheet("font-size:14px;color:rgba(48,49,51,1);font-weight:600;");
    dialogWarningLable2->setStyleSheet("font-size:14px;color:rgba(48,49,51,1);");
    authDialog->btnCancel->setStyleSheet("QPushButton{background-color:rgba(221,223,231,0);border-radius:4px;border:0.7px solid rgba(221, 223, 231, 1);color:rgba(143, 147, 153, 1);font-size:14px;}"
                             "QPushButton:hover{background-color:rgba(242,246,253,1);border-radius:4px;border:0.7px solid rgba(221, 223, 231, 1);color:rgba(143, 147, 153, 1);font-size:14px;}"
                             "QPushButton:pressed{background-color:rgba(242,246,253,1);border-radius:4px;border:0.7px solid rgba(221, 223, 231, 1);color:rgba(143, 147, 153, 1);font-size:14px;}");
    authDialog->btnOk->setStyleSheet("QPushButton{background-color:rgba(100, 105, 241, 1);border-radius:4px;color:rgba(255,255,255,1);font-size:14px;}"
                                     "QPushButton:hover{background-color:rgba(136,140,255,1);border-radius:4px;color:rgba(255,255,255,1);font-size:14px;}"
                                     "QPushButton:pressed{background-color:rgba(82,87,217,1);border-radius:4px;color:rgba(255,255,255,1);font-size:14px;}");
    authDialogContentWidget->setStyleSheet("background-color:rgba(255,255,255,1);border-radius:6px;");
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
    //    root授权框部分
        authDialogContentWidget->setStyleSheet("background-color:rgba(61,61,65,1);border-radius:6px;");
        rootDialogTitleText->setStyleSheet("font-size:14px;font-weight:600;color:rgba(249,249,249,1);");
        rootDialogClose->setStyleSheet("QPushButton{background-color:rgba(255,255,255,0);border-image:url(:/data/elements_dark/close.png);border-radius:4px;}"
                                        "QPushButton:hover{background-color:rgba(253,149,149,1);border-image:url(:/data/elements_dark/close.png);border-radius:4px;}"
                                        "QPushButton:pressed{background-color:rgba(237,100,100,1);border-image:url(:/data/elements_dark/close.png);border-radius:4px;}");
        rootDialogMin->setStyleSheet("QPushButton{background-color:rgba(255,255,255,0);border-image:url(:/data/elements_dark/minimize.png);border-radius:4px;}"
                                     " QPushButton:hover{background-color:rgba(0,0,0,0.04);border-image:url(:/data/elements_dark/minimize.png);border-radius:4px;}"
                                     "QPushButton:pressed{background-color:rgba(0,0,0,0.08);border-image:url(:/data/elements_dark/minimize.png);border-radius:4px;}");
        authDialog->dialogKey->setStyleSheet("QLineEdit{border:1px solid #606265;font-size:14px;color:rgba(143,147,153,1);}");
//                                             "QLineEdit:hover{border:1px solid rgba(100,105, 241, 1);font-size:14px;}");
        dialogKeyLable->setStyleSheet("color:rgba(192,196,204,1);font-size:14px;");
        divingLine->setStyleSheet("background-color:rgba(72,72,76,1);");
        shadowEffect->setColor(Qt::black);
        authDialogContentWidget->setGraphicsEffect(shadowEffect);
        dialogWarningLable->setStyleSheet("font-size:14px;color:rgba(192,196,204,1);font-weight:600;");
        dialogWarningLable2->setStyleSheet("font-size:14px;color:rgba(192,196,204,1);");
        authDialog->btnCancel->setStyleSheet("QPushButton{background-color:rgba(221,223,231,0);border-radius:4px;border:1px solid rgba(96, 98, 101, 1);color:rgba(192, 196, 204, 1);font-size:14px;}"
                                 "QPushButton:hover{background-color:rgba(242,246,253,1);border-radius:4px;border:0.7px solid rgba(221, 223, 231, 1);color:rgba(143, 147, 153, 1);font-size:14px;}"
                                 "QPushButton:pressed{background-color:rgba(242,246,253,1);border-radius:4px;border:0.7px solid rgba(221, 223, 231, 1);color:rgba(143, 147, 153, 1);font-size:14px;}");
        authDialog->btnOk->setStyleSheet("QPushButton{background-color:rgba(100, 105, 241, 1);border-radius:4px;color:rgba(255,255,255,1);font-size:14px;}"
                                         "QPushButton:hover{background-color:rgba(136,140,255,1);border-radius:4px;color:rgba(255,255,255,1);font-size:14px;}"
                                         "QPushButton:pressed{background-color:rgba(82,87,217,1);border-radius:4px;color:rgba(255,255,255,1);font-size:14px;}");;
    comboUdisk->setThemeDark(); //设置combobox响应深色主题
    this->setStyleSheet("background-color:rgba(31,32,34,1);");
    emit setStyleWidgetStyle(DARKTHEME); //设置stylewidget响应黑色主题
}
