#include "page1.h"

Page1::Page1( StyleWidgetAttribute page_swa)
{
    swa=page_swa;
    swa.setW(COMBOBOXW);
    swa.setH(COMBOBOXH);
    swa.itemHeight=ITEMHEIGHT;
    swa.radius=COMBOBOXRADIUS;
    initControlQss();//初始化样式
    dialogInitControlQss(page_swa);
    getStorageInfo();//获取磁盘信息

}

void Page1::initControlQss()
{
    tabIso=new QLabel;
    tabUdisk=new QLabel;
    tabIso->setText(tr("选择光盘镜像文件"));
    tabUdisk->setText(tr("选择U盘"));
    tabIso->setFixedHeight(20);
    tabUdisk->setFixedHeight(20);
    tabIso->setObjectName("tabLable");
    tabUdisk->setObjectName("tabLable");
    comboUdisk=new StyleComboBox(swa);
    warnningIcon=new QLabel;
    warnningIcon->setStyleSheet("border-image:url(:/data/warning.png);border:0px;");
    warnningIcon->setFixedSize(24,24);
    warnningText=new QLabel;
    warnningText->setText(tr("制作启动盘的U盘将被格式化，请先备份好重要文件！"));
    warnningText->setStyleSheet("color:rgba(96, 98, 102, 1);font-size:14px;");

    urlIso=new QLineEdit;
    urlIso->setEnabled(false);
    urlIso->setFixedHeight(30);

    findIso=new QPushButton(this);
    findIso->setText(tr("浏览"));
    findIso->setFixedSize(56,30);
    connect(findIso,&QPushButton::clicked,this,[=]{
        urlIso->setText( QFileDialog::getOpenFileName(0,tr("选择镜像文件"),QDir::homePath(),"ISO(*.iso)"));
        });
    connect(urlIso,&QLineEdit::textChanged,this,&Page1::ifStartBtnChange);
    creatStart=new QPushButton(this);
    creatStart->setFixedSize(200,30);
    creatStart->setText(tr("开始制作"));
    creatStart->setEnabled(false);
    creatStart->setStyleSheet("background-color:rgba(236,236,236,1);border-radius:15px;");
    connect(creatStart,&QPushButton::clicked,this,&Page1::creatStartSlots);
//    connect(createStart,)

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

    this->setStyleSheet(".QPushButton{background-color:rgba(100, 105, 241, 1);color:#fff;border-radius:4px;}"
                        ".QPushButton:hover{background-color:rgba(136,140,255,1);}"
                        ".QPushButton:pressed{background-color:rgba(82,87,217,1);}");
    findIso->setStyleSheet(".QPushButton{background-color:rgba(240, 240, 240, 1);color:#000;border-radius:4px;}"
                           ".QPushButton:hover{background-color:rgba(136,140,255,1);color:#fff;}"
                           ".QPushButton:pressed{background-color:rgba(82,87,217,1);color:#fff;}");
    urlIso->setStyleSheet("background-color:rgba(240, 240, 240, 1);color:rgba(96, 98, 101, 1);font-size:12px;");
    comboUdisk->setStyleSheet("font-size:12px;");

}

void Page1::dialogInitControlQss(StyleWidgetAttribute page_swa)
{
    page_swa.setW(424);
    page_swa.setH(264);
    styleDialog = new StyleWidget(page_swa,tr("授权"),1);
    styleDialog->showOrHide();

    QPushButton *dialogYes=new QPushButton;
    dialogYes->setFixedSize(64,30);
    dialogYes->setText(tr("授权"));
    dialogYes->setObjectName("dialogYes");
    connect(dialogYes,&QPushButton::clicked,this,&Page1::onDialogYesClick);
    connect(dialogYes,&QPushButton::clicked,this,[=]{styleDialog->showOrHide();});
    QPushButton *dialogNo=new QPushButton;
    connect(dialogNo,&QPushButton::clicked,this,&Page1::dealDialogCancel);
    dialogNo->setFixedSize(64,30);
    dialogNo->setText(tr("取消"));
    dialogNo->setObjectName("dialogNo");
    QLabel *dialogWarnningLable=new QLabel;
    dialogWarnningLable->setText(tr("要安装或卸载软件，您需要进行验证："));
    QLabel *dialogWarnningLable2=new QLabel;
    dialogWarnningLable2->setText(tr("一个程序正试图执行一个需要特权的动作。要求授权以执行该动作。"));
    QLabel *dialogKeyLable=new QLabel;
    dialogKeyLable->setText(tr("输入密码:"));
    dialogKey=new QLineEdit;
    dialogKey->setPlaceholderText(tr("请输入密码"));
    dialogKey->setEchoMode(QLineEdit::Password);
    dialogKey->setFixedSize(296,32);

    QHBoxLayout *hlt1=new QHBoxLayout;
    hlt1->setMargin(0);
    hlt1->setSpacing(0);
    hlt1->addSpacing(44);
    hlt1->addWidget(dialogWarnningLable);
    hlt1->addStretch(9);
    QHBoxLayout *hlt2=new QHBoxLayout;
    hlt2->setMargin(0);
    hlt2->setSpacing(0);
    hlt2->addSpacing(44);
    hlt2->addWidget(dialogWarnningLable2);
    hlt2->addSpacing(16+page_swa.shadow);
    QHBoxLayout *hlt3=new QHBoxLayout;
    hlt3->setMargin(0);
    hlt3->setSpacing(0);
    hlt3->addSpacing(44);
    hlt3->addWidget(dialogKeyLable);
    hlt3->addSpacing(6);
    hlt3->addWidget(dialogKey);
    hlt3->addSpacing(16+page_swa.shadow);
    QHBoxLayout *hlt4=new QHBoxLayout;
    hlt4->setMargin(0);
    hlt4->setSpacing(0);
    hlt4->addStretch(2);
    hlt4->addWidget(dialogNo);
    hlt4->addSpacing(16);
    hlt4->addWidget(dialogYes);
    hlt4->addSpacing(16+page_swa.shadow);
    QVBoxLayout *vlt1=new QVBoxLayout;
    vlt1->setMargin(0);
    vlt1->setSpacing(0);
    vlt1->addLayout(hlt1);
    vlt1->addSpacing(16);
    vlt1->addLayout(hlt2);
    vlt1->addSpacing(16);
    vlt1->addLayout(hlt3);
    vlt1->addSpacing(31);
    vlt1->addLayout(hlt4);
    vlt1->addSpacing(24);
    styleDialog->body->setLayout(vlt1);

    dialogKey->setStyleSheet("border:1px solid rgba(221, 223, 231, 1);");
    dialogWarnningLable->setStyleSheet("font-size:14px;");
    dialogWarnningLable2->setStyleSheet("font-size:12px;");
    dialogNo->setStyleSheet("StyleWidget #dialogNo{background-color:rgba(255,255,255,0);border-radius:4px;border:1px solid rgba(221, 223, 231, 1);color:rgba(143, 147, 153, 1);}"
                             "StyleWidget #dialogNo:hover{background-color:rgba(136,140,255,1);border-radius:4px;color:#fff;}"
                             "StyleWidget #dialogNo:pressed{background-color:rgba(82,87,217,1);border-radius:4px;color:#fff;}");
    dialogYes->setStyleSheet("StyleWidget #dialogYes{background-color:rgba(100, 105, 241, 1);border-radius:4px;color:#fff;}"
                               "StyleWidget #dialogYes:hover{background-color:rgba(136,140,255,1);border-radius:4px;color:#fff;}"
                               "StyleWidget #dialogYes:pressed{background-color:rgba(82,87,217,1);border-radius:4px;color:#fff;}");
}

void Page1::getStorageInfo()//获取磁盘信息
{
    QList<QStorageInfo> diskList = QStorageInfo::mountedVolumes();//获取磁盘列表

    for(QStorageInfo& disk : diskList)
    {
        if(""==disk.displayName())//名称为空的磁盘不显示
            continue;
        if("/"==disk.displayName())//系统分区不显示
            continue;
        if(disk.bytesTotal()/1048576<4096 || disk.bytesTotal()/1048576>1024*65)//小于4G或大于65G的磁盘不显示
            continue;
        if("tmpfs"==disk.fileSystemType())//tmpfs类型的磁盘不显示
            continue;
        if("/boot"==disk.displayName())//boot分区不显示
            continue;

        QString displayName=disk.displayName();
        if(displayName.length()>UDISK_NAME_MAX_LENGTH)
            displayName=displayName.mid(0,UDISK_NAME_MAX_LENGTH -1)+"…";

        float diskSize=disk.bytesTotal();
        diskSize=diskSize/1048576/1024;

        QString diskUrl=disk.device();
        diskUrl=diskUrl.mid(0,8);

        QString info=displayName+"  ( "+diskUrl+" ) "+QString::number(diskSize,'f',1)+"GB";
        comboUdisk->addItem(info,diskUrl);
    }
    if(0==comboUdisk->listWidget->count())
    {
        comboUdisk->addItem(tr("无可用U盘"),NOUDISK);
        creatStart->setEnabled(false);
    }
}

void Page1::allClose()
{
    styleDialog->allClose();
}
void Page1::creatStartSlots()
{
    creatStart->setEnabled(false);
    creatStart->setStyleSheet("background-color:rgba(236,236,236,1);border-radius:15px;");
    dialogKey->clear();
    styleDialog->showOrHide();
}
bool Page1::event(QEvent *event)
{
    if(comboUdisk->swshadow == nullptr)return QWidget::event(event);
    if (event->type() == QEvent::Leave)
    {
        if(mouseIsLeave())
        {
            comboUdisk->deleteShadow();
        }
    }
    else if (event->type() == QEvent::MouseButtonPress)
    {
        comboUdisk->deleteShadow();
    }
    return QWidget::event(event);
}
bool Page1::mouseIsLeave()
{
    QPoint mouse=QCursor::pos();
    QPoint thisWidget=this->mapToGlobal(this->pos());
    QSize thisWidgetSize=this->size();
    if(mouse.rx()<=thisWidget.rx() || mouse.rx()>=thisWidget.rx()+thisWidgetSize.width() || mouse.ry()<=thisWidget.ry() || mouse.ry()>=thisWidget.ry()+thisWidgetSize.height())
        return true;
    else
        return false;
}

void Page1::onDialogYesClick()
{
    dialogKey->setPlaceholderText(tr("请输入密码"));
    emit makeStart(dialogKey->text(),urlIso->text(),comboUdisk->getDiskPath());
}

void Page1::ifStartBtnChange()
{
    if(comboUdisk->getDiskPath() != NOUDISK && !urlIso->text().isEmpty())
    {
        creatStart->setEnabled(true);
        creatStart->setStyleSheet("background-color:rgba(100,105,241,1);border-radius:15px;");//开始制作按钮是否可以点击的逻辑判断以及按钮样式修改部分
    }
    else
    {
        creatStart->setEnabled(false);
        creatStart->setStyleSheet("background-color:rgba(236,236,236,1);border-radius:15px;");
    }
}

void Page1::dealWrongPasswd()
{
    creatStartSlots();
    dialogKey->setPlaceholderText(tr("密码错误，请重新输入。"));
}

void Page1::doSomethig()
{
    ifStartBtnChange();
}

void Page1::dealDialogCancel()
{
    styleDialog->showOrHide();
    ifStartBtnChange();
}
