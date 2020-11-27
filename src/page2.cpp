#include "page2.h"

Page2::Page2(QWidget *parent) : QWidget(parent)
{
    if(DARKTHEME == themeStatus)
    {
        movieLoading = new QMovie(":/data/elements_dark/loading.gif");
        movieFinish = new QMovie(":/data/elements_dark/finish.gif");
        errLabel = QPixmap::fromImage(QImage("/data/elements_dark/failed.png"));
    }else{
        movieLoading = new QMovie(":/data/elements_light/loading.gif");
        movieFinish = new QMovie(":/data/elements_light/finish.gif");
        errLabel = QPixmap::fromImage(QImage("/data/elements_light/failed.png"));
    }
    frameCount=movieFinish->frameCount();
    connect(movieFinish,&QMovie::frameChanged,this,[=](int num){if(frameCount-1==num)movieFinish->stop();});
    lableMovie=new QLabel;

    QSize movieSize(95,95);
    lableMovie->setFixedSize(movieSize);
    movieLoading->setScaledSize(movieSize);
    movieFinish->setScaledSize(movieSize);

    lableText=new QLabel;
    lableNum=new QLabel(lableMovie);
    lableNum->setFixedSize(36,22);
    lableNum->move((lableMovie->width()-lableNum->width())/2,(lableMovie->height()-lableNum->height())/2);
    lableNum->setAlignment(Qt::AlignCenter);    //设置数字居中对齐
    returnPushButton=new QPushButton;
    connect(returnPushButton,&QPushButton::clicked,this,&Page2::returnMain);
    returnPushButton->setFixedSize(200,30);
    QLabel *filling1 = new QLabel;
    QLabel *filling2 = new QLabel;
    QLabel *filling3 = new QLabel;
    QLabel *filling4 = new QLabel;
    QLabel *filling5 = new QLabel;
    QLabel *filling6 = new QLabel;
    QHBoxLayout *hlt1=new QHBoxLayout;
    hlt1->setMargin(0);
    hlt1->setSpacing(0);
    hlt1->addWidget(filling1,9);
    hlt1->addWidget(lableMovie);
    hlt1->addWidget(filling2,9);
    QHBoxLayout *hlt2=new QHBoxLayout;
    hlt2->setMargin(0);
    hlt2->setSpacing(0);
    hlt2->addWidget(filling3,9);
    hlt2->addWidget(lableText);
    hlt2->addWidget(filling4,9);
    QHBoxLayout *hlt3=new QHBoxLayout;
    hlt3->setMargin(0);
    hlt3->setSpacing(0);
    hlt3->addWidget(filling5,9);
    hlt3->addWidget(returnPushButton);
    hlt3->addWidget(filling6,9);
    QVBoxLayout *vlt1=new QVBoxLayout;
    vlt1->setMargin(0);
    vlt1->setSpacing(0);
    vlt1->addSpacing(115);
    vlt1->addLayout(hlt1);
    vlt1->addSpacing(26);
    vlt1->addLayout(hlt2);
    vlt1->addSpacing(124);
    vlt1->addLayout(hlt3);
    vlt1->addStretch();
    this->setLayout(vlt1);
}
void Page2::playLoadingGif()
{
    lableNum->setText("0%");
    lableNum->show();
    returnPushButton->setEnabled(false);
    returnPushButton->setText(tr("USB starter in production"));
    if(LIGHTTHEME == themeStatus){
        returnPushButton->setStyleSheet("background-color:rgba(236, 236, 236, 1);color:rgba(193, 193, 193, 1);font-size:14px;border-radius:15px;");
    }else
    {
        returnPushButton->setStyleSheet("background-color:rgba(48,49,51,1);color:rgba(249,249,249,1);font-size:14px;border-radius:15px;");
    }
    lableText->setText(tr("Please do not remove the USB driver or power off now"));

    lableMovie->setMovie(movieLoading); //为label设置movie
    movieLoading->start();         //开始播放动画
}

void Page2::playFinishGif()
{
    lableNum->hide();
    returnPushButton->setEnabled(true);
    returnPushButton->setText(tr("return"));
    returnPushButton->setStyleSheet(".QPushButton{background-color:rgba(100, 105, 241, 1);color:#fff;border-radius:15px;font-size:14px;}"
                          ".QPushButton:hover{background-color:rgba(136,140,255,1);}"
                          ".QPushButton:pressed{background-color:rgba(82,87,217,1);}");
    lableText->setText(tr("Finish"));

    lableMovie->clear();
    lableMovie->setMovie(movieFinish);
    movieFinish->start();

}

void Page2::playErrorGif()
{
    lableNum->hide();
    returnPushButton->setEnabled(true);
    returnPushButton->setText(tr("return"));
    returnPushButton->setStyleSheet(".QPushButton{background-color:rgba(100, 105, 241, 1);color:#fff;border-radius:15px;font-size:14px;}"
                          ".QPushButton:hover{background-color:rgba(136,140,255,1);}"
                          ".QPushButton:pressed{background-color:rgba(82,87,217,1);}");
    lableText->setText(tr("Creation Failed"));

    lableMovie->clear();
    lableMovie->setScaledContents(true);
    lableMovie->setPixmap(QPixmap::fromImage(QImage(":/data/elements_dark/failed.png")));
//    lableMovie->setpi

}

void Page2::startMaking(QString key,QString sourcePath,QString targetPath)
{
    uDiskPath = targetPath; //保存U盘路径 用来做错误检查
    emit swToPage2();
    playLoadingGif();
    sourceFileSize = getFileSize(sourcePath);
    command_dd = new QProcess();
    connect(command_dd,&QProcess::readyReadStandardError,this,&Page2::readBashStandardErrorInfo);
    command_dd->start("bash");
    command_dd->waitForStarted();
    QString ddshell = "echo '"+key.toLocal8Bit()+"'| sudo -S dd if='"+sourcePath.toLocal8Bit()+"' of="+targetPath.toLocal8Bit()+" status=progress";
    
//    测试用shell
//    QString ddshell = "dd if=/dev/zero of=/home/kylin/test.iso  bs=1M count=2000  status=progress";
//    QString ddshell = "echo "+key.toLocal8Bit()+"| sudo -S dd if="+sourcePath.toLocal8Bit()+" of=/dev/null status=progress";

    qDebug()<<"ddshell is: "<<ddshell;
    command_dd->write(ddshell.toLocal8Bit() + '\n');
}

qint64 Page2::getFileSize(QString filePath)
{
    QFileInfo info(filePath);
    return info.size()/1048576;
}

void Page2::readBashStandardErrorInfo()
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
         lableNum->setText(QString::number(mission_percent)+ "%");
         if(bytes2.count() == 1 || !ok){
             finishEvent();
         }
    }
}

void Page2::finishEvent()
{
    if(isMakingSuccess())
    {
        playFinishGif();
    }else
    {
        playErrorGif();
    }

    emit makeFinish();
}
bool Page2::isMakingSuccess()
{

    QList<QStorageInfo> diskList = QStorageInfo::mountedVolumes(); //已挂载设备
    for(QStorageInfo& disk : diskList)
    {
        qDebug()<<"目标比对设备"<<uDiskPath<<"***已挂载设备："<<disk.device();
        QString diskPath = disk.device();
        diskPath = diskPath.mid(0,8);
        if(uDiskPath == diskPath)
        {
            return true;
        }
    }
    return false;
}
void Page2::refreshGifStatus()
{
//    qDebug()<<"void Page2::refreshGifStatus()";
//    if(movieLoading != nullptr)
//    {
//        delete movieLoading;
//        delete movieFinish;
//    }
//    if(DARKTHEME == themeStatus)
//    {
//        movieLoading = new QMovie(":/data/elements_dark/loading.gif");
//        movieFinish = new QMovie(":/data/elements_dark/finish.gif");
//    }else{
//        movieLoading = new QMovie(":/data/elements_light/loading.gif");
//        movieFinish = new QMovie(":/data/elements_light/finish.gif");
//    }
//    frameCount=movieFinish->frameCount();
//    connect(movieFinish,&QMovie::frameChanged,this,[=](int num){if(frameCount-1==num)movieFinish->stop();});
//    lableMovie=new QLabel;

//    QSize movieSize(95,95);
//    lableMovie->setFixedSize(movieSize);
//    movieLoading->setScaledSize(movieSize);
//    movieFinish->setScaledSize(movieSize);
}
void Page2::setThemeStyleLight()
{
    themeStatus = LIGHTTHEME;
//    this->setStyleSheet("background-color:rgba(255,5,5,1);");
    lableNum->setStyleSheet("background-color:rgba(236, 236, 236,0);color:rgba(100, 105, 241, 1);font-size:16px;");
    lableText->setStyleSheet("font-size:14px;");

}

void Page2::setThemeStyleDark()
{
    qDebug()<<"void Page2::setThemeStyleDark()";
    themeStatus = DARKTHEME;
    lableNum->setStyleSheet("background-color:rgba(236, 236, 236,0);color:rgba(100, 105, 241, 1);font-size:16px;");
    lableText->setStyleSheet("font-size:14px;color:rgba(249,249,249,1);");
}
