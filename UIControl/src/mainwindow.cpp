#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    init();
    statusbarInit();
    myStyle();
    initGsetting();
    qDebug()<<menu;
    menu->themeUpdate();  //画一次主题
}

MainWindow::~MainWindow()
{
}

void MainWindow::statusbarInit()
{
    title = new QWidget;
    title->setFixedSize(680,40);
    QHBoxLayout *hlyt = new QHBoxLayout();
    hlyt->setMargin(0);
    hlyt->setSpacing(0);
    titleIcon = new QLabel;
    titleIcon->setFixedSize(24,24);
    titleIcon->setPixmap(QPixmap::fromImage(QImage(":/data/usb-boot-maker.svg")));
    titleIcon->setScaledContents(true);
    titleText = new QLabel();
    titleText->setText(tr("USB Boot Maker"));
    titleMin = new QPushButton();
    titleMin->setToolTip(tr("Minimize"));
    titleMin->setProperty("isWindowButton", 0x1);
    titleMin->setProperty("useIconHighlightEffect", 0x2);
    titleMin->setFlat(true);
    titleMin->setFixedSize(30, 30);
    titleMin->setIconSize(QSize(16, 16));
    titleMin->setIcon(QIcon::fromTheme("window-minimize-symbolic"));
    connect(titleMin,&QPushButton::clicked,[=](){
       this->setWindowState(Qt::WindowMinimized); 
    });
    menu  = new menuModule(this);
    connect(menu,&menuModule::menuModuleClose,[=](){this->close();});
    connect(menu,&menuModule::menuModuleSetThemeStyle,this,&MainWindow::setThemeStyle);
    connect(menu,&menuModule::pullupHelp,this,&MainWindow::dealMenuModulePullupHelp);

    titleClose = new QPushButton();
    titleClose->setToolTip(tr("Quit"));
    titleClose->setProperty("isWindowButton", 0x2) ;
    titleClose->setProperty("useIconHighlightEffect", 0x8);;
    titleClose->setFlat(true);
    titleClose->setFixedSize(30, 30);
    titleClose->setIconSize(QSize(16, 16));
    titleClose->setIcon(QIcon::fromTheme("window-close-symbolic"));
    connect(titleClose,&QPushButton::clicked,[=](){
       this->close();
    });
    QHBoxLayout *hlt0 = new QHBoxLayout;
//    hlt0->setMargin(0);
    hlt0->setContentsMargins(0,0,0,0);
    hlt0->setSpacing(0);
    hlt0->addStretch();
    hlt0->addWidget(menu->menuButton,1);
    hlt0->addSpacing(4);
    hlt0->addWidget(titleMin,1);
    hlt0->addSpacing(4);
    hlt0->addWidget(titleClose,1);
    hlt0->addSpacing(4);
    QVBoxLayout *vlt0 = new QVBoxLayout;
    vlt0->setMargin(0);
    vlt0->setSpacing(0);
    vlt0->addSpacing(4);
    vlt0->addLayout(hlt0);
    vlt0->addStretch(); // 顶上充满
    QHBoxLayout *hlt=new QHBoxLayout;//标题栏内部
    hlt->setMargin(0);
    hlt->setSpacing(0);
    hlt->addSpacing(8);
    hlt->addWidget(titleIcon,1);
    hlt->addSpacing(8);
    hlt->addWidget(titleText,1);
    hlt->addStretch(99);
    hlt->addLayout(vlt0);
    hlyt->addLayout(hlt);
    title->setLayout(hlyt);
}

void MainWindow::init(){
    this->setWindowTitle(tr("USB Boot Maker"));
    this->setFixedSize(680,507);
//    在屏幕中央显示
    QRect availableGeometry = qApp->primaryScreen()->availableGeometry();
    this->move((availableGeometry.width()-this->width())/2,(availableGeometry.height()- this->height())/2);
    m_DaemonIpcDbus = new DaemonIpcDbus();

//    连结systembus进程消息
    QDBusConnection::systemBus().connect(QString(),QString("/"),"com.usbbootmaker.interface","authorityStatus",this,SLOT(dealAuthorityStatus(QString)));

}

void MainWindow::aboutClick()
{
    about->setFixedSize(424,298);
    about->setWindowFlag(Qt::FramelessWindowHint);
    about->show();
}

void MainWindow::myStyle()
{
    timer = new QTimer(this);
    page1 = new Page1();
    page2 = new Page2();
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
    bottomWidget = new QWidget();
    bottomWidget->setLayout(hlt);
    QVBoxLayout *vlt =new QVBoxLayout;
    vlt->setMargin(0);
    vlt->setSpacing(0);
    vlt->addWidget(title);
    vlt->addWidget(stackedWidget,99);
//    vlt->addLayout(hlt,1);
    vlt->addWidget(bottomWidget,1);
    this->setLayout(vlt);
    // 状态栏初始化部分，需要时打开注释
//    createTrayActions();
//     关于窗口绘制
    about = new QWidget();
}

void MainWindow::initGsetting()
{
    //应用主窗口状态
    if(QGSettings::isSchemaInstalled(APPDATA))
    {}
    return ;
}

void MainWindow::setThemeStyle(QString str)
{
    if("dark-theme" == str)
    {
        setThemeDark();
    }else{
        setThemeLight();
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
    m_mainTray->setToolTip(tr("USB Boot Maker"));
    m_mainTray->show();
}

void MainWindow::dealMenuModulePullupHelp(){
    if(!m_DaemonIpcDbus->daemonIsNotRunning()){
        m_DaemonIpcDbus->showGuide("tools/usb-boot-maker");
    }
}
void MainWindow::makeStart()
{
    disconnect(titleClose,&QPushButton::clicked,0,0); //开始制作之后取消之前click触发的应用关闭功能
    connect(titleClose,&QPushButton::clicked,this,&MainWindow::doubleCheck);
    page2->startMaking();
    stackedWidget->setCurrentIndex(changePage());
    pointLable1->setStyleSheet("border-radius:4px;background:rgba(151, 151, 151, 1)");
    pointLable2->setStyleSheet("border-radius:4px;background:rgba(100, 105, 241, 1)");
    pointLable3->setStyleSheet("border-radius:4px;background:rgba(151, 151, 151, 1)");
}

void MainWindow::doubleCheck(){
    QMessageBox::StandardButton result =  QMessageBox::warning(this,tr("Warning"),tr("USB driver is in production.Are you sure you want to stop task and exit the program?"),
                         QMessageBox::Yes | QMessageBox::No,QMessageBox::No);
    switch (result){
        case QMessageBox::Yes:{//exit_proc在其他case中也有效，不加花括号exit_proc的生命周期就不会终结，在其他case中就会成为一个没有初始化的变量
            // exit progress and close mainwindow
            QDBusMessage exit_proc = QDBusMessage::createMethodCall("com.usbbootmaker.systemdbus","/","com.usbbootmaker.interface","MakeExit");
            QDBusConnection::systemBus().call(exit_proc);
            this->close();
            break;
        }
        case QMessageBox::No:
            break;
    }
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
    this->setWindowFlag(Qt::WindowStaysOnTopHint,true);
    this->setWindowFlag(Qt::WindowStaysOnTopHint,false);
    showNormal();
    activateWindow();
}

void MainWindow::makeFinish()
{
    disconnect(titleClose,&QPushButton::clicked,0,0); //开始制作之后取消之前click触发的应用关闭功能
    connect(titleClose,&QPushButton::clicked,[=](){
       this->close();
    });
    pointLable3->setStyleSheet("border-radius:4px;background:rgba(100, 105, 241, 1);");
    pointLable2->setStyleSheet("border-radius:4px;background:rgba(151, 151, 151, 1);");
    pointLable1->setStyleSheet("border-radius:4px;background:rgba(151, 151, 151, 1);");
}

void MainWindow::returnMain()
{
    stackedWidget->setCurrentIndex(changePage());
    page1->ifStartBtnChange();
    page1->urlIso->clear();
    pointLable1->setStyleSheet("border-radius:4px;background:rgba(100, 105, 241, 1);");
    pointLable2->setStyleSheet("border-radius:4px;background:rgba(151, 151, 151, 1);");
    pointLable3->setStyleSheet("border-radius:4px;background:rgba(151, 151, 151, 1);");
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_F1)
    {
        if(!m_DaemonIpcDbus->daemonIsNotRunning()){
            m_DaemonIpcDbus->showGuide("tools/usb-boot-maker");
        }
    }
}

void MainWindow::dealAuthorityStatus(QString status){
    if("success" == status){
        makeStart();
    }else{
//        授权失败，回到到page1
    }
}

void MainWindow::setThemeDark()
{
//    titleMin->setStyleSheet("QPushButton{background-color:rgba(255,255,255,0);border-image:url(:/data/min_h.png);border-radius:4px;}"
//                             "QPushButton:hover{background-color:rgba(61,61,61,1);border-image:url(:/data/min_h.png);border-radius:4px;}"
//                             "QPushButton:pressed{background-color:rgba(73,73,73,1);border-image:url(:/data/min_h.png);border-radius:4px;}");
//    titleClose->setStyleSheet("QPushButton{background-color:rgba(255,255,255,0);border-image:url(:/data/close_h.png);border-radius:4px;}"
//                               "QPushButton:hover{background-color:rgba(253,149,149,1);border-image:url(:/data/close_h.png);border-radius:4px;}"
//                               "QPushButton:pressed{background-color:rgba(237,100,100,1);border-image:url(:/data/close_h.png);border-radius:4px;}");
//    titleMenu->setStyleSheet("QPushButton{background-color:rgba(255,255,255,0);border-image:url(:/data/elements_dark/menu.png);border-radius:4px;}"
//                              "QPushButton:hover{background-color:rgba(0,0,0,0.04);border-image:url(:/data/elements_dark/menu.png);border-radius:4px;}"
//                              "QPushButton:pressed{background-color:rgba(0,0,0,0.08);border-image:url(:/data/elements_dark/menu.png);border-radius:4px;}"
//                             "QPushButton::menu-indicator{image:None;}");
    titleText->setStyleSheet("color:rgba(249,249,249,1);font-size:14px;");
    title->setStyleSheet(".QWidget{background-color:rgba(31,32,34,1)}");
    stackedWidget->setStyleSheet("background-color:rgba(31,32,34,1);");
    bottomWidget->setStyleSheet("background-color:rgba(31,32,34,1);");
    page2->setThemeStyleDark();
//    this->setStyleSheet("background-color:rgba(31,32,34,1);");
//    this->setStyleSheet(".QWidget{background-color:rgba(31,32,34,1);}");
    page1->setThemeStyleDark();
}

void MainWindow::setThemeLight()
{
//    titleMin->setStyleSheet("QPushButton{background-color:rgba(255,255,255,0);border-image:url(:/data/min_d.png);border-radius:4px;}"
//                             "QPushButton:hover{background-color:rgba(0,0,0,0.15);border-image:url(:/data/min_d.png);border-radius:4px;}"
//                             "QPushButton:pressed{background-color:rgba(0,0,0,0.3);border-image:url(:/data/min_d.png);border-radius:4px;}");
//    titleClose->setStyleSheet("QPushButton{background-color:rgba(255,255,255,0);border-image:url(:/data/close_d.png);border-radius:4px;}"
//                               "QPushButton:hover{background-color:rgba(253,149,149,1);border-image:url(:/data/close_h.png);border-radius:4px;}"
//                               "QPushButton:pressed{background-color:rgba(237,100,100,1);border-image:url(:/data/close_h.png);border-radius:4px;}");
//    titleMenu->setStyleSheet("QPushButton{background-color:rgba(255,255,255,0);border-image:url(:/data/elements_light/menu.png);border-radius:4px;}"
//                              "QPushButton:hover{background-color:rgba(0,0,0,0.04);border-image:url(:/data/elements_light/menu.png);border-radius:4px;}"
//                              "QPushButton:pressed{background-color:rgba(0,0,0,0.08);border-image:url(:/data/elements_light/menu.png);border-radius:4px;}"
//                             "QPushButton::menu-indicator{image:None;}");
    stackedWidget->setStyleSheet("background-color:rgba(255,255,255,1);");
    title->setStyleSheet(".QWidget{background-color:rgba(255,255,255,1)}");
    bottomWidget->setStyleSheet(".QWidget{background-color:rgba(255,255,255,1)}");
    titleText->setStyleSheet("color:rgba(48,49,51,1);font-size:14px;");
//    this->setStyleSheet("background-color:rgba(255,255,255,1);");
//    this->setStyleSheet(".QWidget{background-color:rgba(255,255,255,1);}");
    page1->setThemeStyleLight();
    page2->setThemeStyleLight();
}
