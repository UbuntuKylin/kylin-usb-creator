#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    init();
    myStyle();
    initGsetting();
}

MainWindow::~MainWindow()
{

}
void MainWindow::init(){
    this->setWindowTitle(tr("kylin usb creator"));
    this->setFixedSize(680,467);
//    在屏幕中央显示
    QRect availableGeometry = qApp->primaryScreen()->availableGeometry();
//    QRect availableGeometry = qApp->
    this->move((availableGeometry.width()-this->width())/2,(availableGeometry.height()- this->height())/2);

}
void MainWindow::myStyle()
{
    timer = new QTimer(this);
    page1 = new Page1();
    page2 = new Page2();
    connect(page1,&Page1::makeStart,page2,&Page2::startMaking);
    connect(page2,&Page2::swToPage2,this,&MainWindow::makeStart);
    connect(page2,&Page2::makeFinish,this,&MainWindow::makeFinish);
    connect(page2,&Page2::returnMain,this,&MainWindow::returnMain);



    //内部样式
    QSize pointSize(8,8);
    pointLable1=new QLabel;
    pointLable2=new QLabel;
    pointLable3=new QLabel;
    pointLable1->setFixedSize(pointSize);
    pointLable2->setFixedSize(pointSize);
    pointLable3->setFixedSize(pointSize);
    pointLable1->setStyleSheet("border-radius:4px;background:rgba(100, 105, 241, 1)");
    pointLable2->setStyleSheet("border-radius:4px;background:rgba(151, 151, 151, 1)");
    pointLable3->setStyleSheet("border-radius:4px;background:rgba(151, 151, 151, 1)");

    stackedWidget =new QStackedWidget(this);
    stackedWidget->addWidget(page1);
    stackedWidget->addWidget(page2);

    //布局
    QHBoxLayout *hlt =new QHBoxLayout;
    hlt->addStretch(15);
    hlt->addWidget(pointLable1);
    hlt->addStretch(1);
    hlt->addWidget(pointLable2);
    hlt->addStretch(1);
    hlt->addWidget(pointLable3);
    hlt->addStretch(15);
    QVBoxLayout *vlt =new QVBoxLayout;
    vlt->addWidget(stackedWidget,99);
    vlt->addLayout(hlt,1);
    vlt->addSpacing(7);
    this->setLayout(vlt);

    // 状态栏初始化部分，需要时打开注释
//    createTrayActions();
}

void MainWindow::initGsetting()
{
    if(QGSettings::isSchemaInstalled(FITTHEMEWINDOW))
    {
//        m_pGsettingThemeData = new QGSettings(KYLINUSBCREATORDATA);
//        if(1)
//        {
//            qDebug()<<"Local gsettings init success.";
//            page1->setThemeStyleLight();
//            page2->setThemeStyleLight();
//        }
        // 主题适配
        if(QGSettings::isSchemaInstalled(FITTHEMEWINDOW))
        {
            m_pGsettingThemeData = new QGSettings(FITTHEMEWINDOW);

            connect(m_pGsettingThemeData,&QGSettings::changed,this, [=] (const QString &key)
            {
                if(key == "styleName")
                {

                        setThemeStyle();
                }
            });
            setThemeStyle(); //主题安装成功之后默认做一次主题状态的判断
        }
    }
    return ;
}


void MainWindow::setThemeStyle()
{
    QString nowThemeStyle = m_pGsettingThemeData->get("styleName").toString();
    if("ukui-dark" == nowThemeStyle || "ukui-black" == nowThemeStyle)
    {
//        子类在这里调用对应方法做深色渲染
        qDebug()<<"深色渲染start";
        page2->setThemeStyleDark();
        this->setStyleSheet("background-color:rgba(31,32,34,1);");
        page1->setThemeStyleDark();
    }else{
//        子类在这里调用方法做对应浅色渲染
        qDebug()<<"浅色渲染start";
        page1->setThemeStyleLight();
        this->setStyleSheet("background-color:rgba(255,255,255,1);");
        page2->setThemeStyleLight();
    }

}
void MainWindow::createTrayActions()
{
    if(!QSystemTrayIcon::isSystemTrayAvailable())
    {
        return;
    }
    m_mainTray = new QSystemTrayIcon(this);
    m_mainTray->setIcon(QIcon(":/data/logo/48.png"));
    m_mainTray->setToolTip(tr("kylin usb creator"));
    m_mainTray->show();
}

void MainWindow::makeStart()
{
//    isInPage2 = true;
    stackedWidget->setCurrentIndex(changePage());
    pointLable1->setStyleSheet("border-radius:4px;background:rgba(151, 151, 151, 1)");
    pointLable2->setStyleSheet("border-radius:4px;background:rgba(100, 105, 241, 1)");
    pointLable3->setStyleSheet("border-radius:4px;background:rgba(151, 151, 151, 1)");
}

int MainWindow::changePage()
{
    int count = stackedWidget->count();
    int index = stackedWidget->currentIndex();
    ++index;
    if (index >= count)index = 0;
    return index;
}
void MainWindow::handleIconClickedSub()
{
    qDebug()<<"MainWindow::handleIconClickedSub  iswindowminized:"<<this->isMinimized();

//    QRect availableGeometry = qApp->primaryScreen()->availableGeometry();
//    this->move((availableGeometry.width() - this->width())/2, (availableGeometry.height() - this->height())/2);
    this->showNormal();
    this->raise();
    this->activateWindow();
    qDebug()<<this->parent();

//    this->show();

//    stackedWidget->showNormal();
//    stackedWidget->raise();
//    stackedWidget->activateWindow();
//    stackedWidget->show();


//    page1->showNormal();
//    page1->raise();
//    page1->activateWindow();
//    page1->show();
}
void MainWindow::makeFinish()
{
    if(!page1->ifStartBtnChange())
    {
    }
    pointLable3->setStyleSheet("border-radius:4px;background:rgba(100, 105, 241, 1);");
    pointLable2->setStyleSheet("border-radius:4px;background:rgba(151, 151, 151, 1);");
    pointLable1->setStyleSheet("border-radius:4px;background:rgba(151, 151, 151, 1);");
}

void MainWindow::returnMain()
{
    stackedWidget->setCurrentIndex(changePage());
    page1->ifStartBtnChange();
    pointLable1->setStyleSheet("border-radius:4px;background:rgba(100, 105, 241, 1);");
    pointLable2->setStyleSheet("border-radius:4px;background:rgba(151, 151, 151, 1);");
    pointLable3->setStyleSheet("border-radius:4px;background:rgba(151, 151, 151, 1);");
}
