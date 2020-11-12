#include "page2.h"

Page2::Page2(QWidget *parent) : QWidget(parent)
{
    movieLoading = new QMovie(":/data/loading2.gif");
    movieFinish = new QMovie(":/data/finish2.gif");
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
    lableNum->setAlignment(Qt::AlignRight);    // 设置对齐方式
    returnPushButton=new QPushButton;
    connect(returnPushButton,&QPushButton::clicked,this,&Page2::returnMain);
    returnPushButton->setFixedSize(200,30);

    QHBoxLayout *hlt1=new QHBoxLayout;
    hlt1->setMargin(0);
    hlt1->setSpacing(0);
    hlt1->addStretch(9);
    hlt1->addWidget(lableMovie);
    hlt1->addStretch(9);
    QHBoxLayout *hlt2=new QHBoxLayout;
    hlt2->setMargin(0);
    hlt2->setSpacing(0);
    hlt2->addStretch(9);;
    hlt2->addWidget(lableText);
    hlt2->addStretch(9);
    QHBoxLayout *hlt3=new QHBoxLayout;
    hlt3->setMargin(0);
    hlt3->setSpacing(0);
    hlt3->addStretch(9);;
    hlt3->addWidget(returnPushButton);
    hlt3->addStretch(9);;
    QVBoxLayout *vlt1=new QVBoxLayout;
    vlt1->setMargin(0);
    vlt1->setSpacing(0);
    vlt1->addSpacing(108);
    vlt1->addLayout(hlt1);
    vlt1->addSpacing(26);
    vlt1->addLayout(hlt2);
    vlt1->addSpacing(124);
    vlt1->addLayout(hlt3);
    vlt1->addSpacing(31);
    this->setLayout(vlt1);
}

void Page2::playLoadingGif()
{
    lableNum->setText("0%");
    lableNum->show();
    returnPushButton->setEnabled(false);
    returnPushButton->setText(tr("正在制作中"));
    if(LIGHTTHEME == themeStatus){
        returnPushButton->setStyleSheet("background-color:rgba(236, 236, 236, 1);color:rgba(193, 193, 193, 1);font-size:14px;border-radius:15px;");
    }else
    {
        returnPushButton->setStyleSheet("background-color:rgba(48,49,51,1);color:rgba(249,249,249,1);font-size:14px;border-radius:15px;");
    }
    lableText->setText(tr("制作时请不要移除磁盘或关机"));

    lableMovie->setMovie(movieLoading); //为label设置movie
    movieLoading->start();         //开始播放动画
}

void Page2::playFinishGif()
{
    lableNum->hide();
    returnPushButton->setEnabled(true);
    returnPushButton->setText(tr("返回"));
    returnPushButton->setStyleSheet(".QPushButton{background-color:rgba(100, 105, 241, 1);color:#fff;border-radius:15px;font-size:14px;}"
                          ".QPushButton:hover{background-color:rgba(136,140,255,1);}"
                          ".QPushButton:pressed{background-color:rgba(82,87,217,1);}");
    lableText->setText(tr("制作完成"));

    lableMovie->clear();
    lableMovie->setMovie(movieFinish);
    movieFinish->start();

}

void Page2::startMaking(QString key,QString sourcePath,QString targetPath)
{
    emit swToPage2();
    playLoadingGif();
    sourceFileSize = getFileSize(sourcePath);
    command_dd = new QProcess();
    connect(command_dd,&QProcess::readyReadStandardError,this,&Page2::readBashStandardErrorInfo);
    command_dd->start("bash");
    command_dd->waitForStarted();
//    QString ddshell = "echo "+key.toLocal8Bit()+"| sudo -S dd if="+sourcePath.toLocal8Bit()+" of="+targetPath.toLocal8Bit()+" status=progress";
//    测试用shell
//    QString ddshell = "dd if=/dev/zero of=/home/kylin/test.iso  bs=1M count=2000  status=progress";
    QString ddshell = "echo "+key.toLocal8Bit()+"| sudo -S dd if="+sourcePath.toLocal8Bit()+" of=/dev/null status=progress";

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
    playFinishGif();
    emit makeFinish();
}

void Page2::setThemeStyleLight()
{
    themeStatus = LIGHTTHEME;
    lableNum->setStyleSheet("background-color:rgba(236, 236, 236,0);color:rgba(100, 105, 241, 1);font-size:16px;");
    lableText->setStyleSheet("font-size:14px;color:");
}

void Page2::setThemeStyleDark()
{
    themeStatus = DARKTHEME;
    lableNum->setStyleSheet("background-color:rgba(236, 236, 236,0);color:rgba(100, 105, 241, 1);font-size:16px;");
    lableText->setStyleSheet("font-size:14px;color:rgba(249,249,249,1);");
}
