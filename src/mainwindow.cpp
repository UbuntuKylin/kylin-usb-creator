#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    statusbarInit();
    init();
    myStyle();
    initGsetting();
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
    titleIcon->setPixmap(QPixmap::fromImage(QImage(":/data/kylin-usb-creator.svg")));
    titleIcon->setScaledContents(true);
    titleText = new QLabel();
    titleText->setText(tr("kylin usb creator"));
    titleMin = new QPushButton();
    titleMin->setIconSize(QSize(30,30));
    titleMin->setFixedSize(30,30);
    connect(titleMin,&QPushButton::clicked,[=](){
       this->setWindowState(Qt::WindowMinimized); 
    });
    titleMenu = new QPushButton();
    titleMenu->setIconSize(QSize(30,30));
    titleMenu->setFixedSize(30,30);
//    使用主题效果
//    titleMenu->setProperty("isWindowButton",0x1);
//    titleMenu->setProperty("useIconHighlightEffect",0x2);
//    titleMenu->setFlat(true);
//    titleMenu->setIcon(QIcon::fromTheme("application-menu"));
    titleClose = new QPushButton();
    titleClose->setIconSize(QSize(30,30));
    titleClose->setFixedSize(30,30);
    connect(titleClose,&QPushButton::clicked,[=](){
       this->close();
    });
    QHBoxLayout *hlt0 = new QHBoxLayout;
    hlt0->setMargin(0);
    hlt0->setSpacing(0);
    hlt0->addStretch();
//    hlt0->addWidget(titleMenu,1);
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

    Menu = new QMenu();
    QList<QAction *> actions ;
    QList<QAction *> themeaction;
    //一级菜单绘制
    actionTheme = new QAction(Menu);
    actionTheme->setText(tr("theme"));
    actionHelp = new QAction(Menu);
    actionHelp->setText(tr("help"));
    actionAbout = new QAction(Menu);
    actionAbout->setText(tr("about"));
    actionQuit = new QAction(Menu);
    actionQuit->setText(tr("quit"));
    actions<<actionTheme<<actionHelp<<actionAbout<<actionQuit;
    Menu->addActions(actions);
    titleMenu->setMenu(Menu);
//    主题菜单绘制
//    TODO：参照帮助菜单部分精简代码
    QMenu *themeMenu = new QMenu();
    QAction *actionThemeAuto = new QAction(actionTheme);
    actionThemeAuto->setText(tr("auto"));
    QAction *actionThemeDark = new QAction(actionTheme);
    actionThemeDark->setText("dark");
    QAction *actionThemeLight = new QAction(actionTheme);
    actionThemeLight->setText("light");
    themeaction<<actionThemeAuto<<actionThemeDark<<actionThemeLight;
    actionTheme->setMenu(themeMenu);
    themeMenu->addActions(themeaction);
//    帮助菜单绘制
    QMenu *helpMenu = new QMenu();
    helpMenu->addAction(tr("check for updates"));
    helpMenu->addAction(tr("feedback"));
    helpMenu->addAction(tr("offical website"));
    actionHelp->setMenu(helpMenu);

    connect(Menu,&QMenu::triggered,this,&MainWindow::trigerMenu);
    title->show();
}

void MainWindow::trigerMenu(QAction *act)
{
    if(act->text() == "quit")
    {
        this->close();
    }else if(act->text() == "about")
    {
        aboutClick();
    }
}

void MainWindow::init(){
    this->setWindowTitle(tr("kylin usb creator"));
    this->setFixedSize(680,467);
//    在屏幕中央显示
    QRect availableGeometry = qApp->primaryScreen()->availableGeometry();
    this->move((availableGeometry.width()-this->width())/2,(availableGeometry.height()- this->height())/2);
    m_DaemonIpcDbus = new DaemonIpcDbus();
    //    QGraphicsDropShadowEffect* shadowEffect = new QGraphicsDropShadowEffect(this);
    //    shadowEffect->setOffset(0,0);
    //    shadowEffect->setColor(QColor(0,0,0));
    //    shadowEffect->setBlurRadius(30);
    //    title->setGraphicsEffect(shadowEffect);
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
    vlt->setMargin(0);
    vlt->setSpacing(0);
    vlt->addWidget(title);
    vlt->addWidget(stackedWidget,99);
    vlt->addLayout(hlt,1);
    vlt->addSpacing(7);
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
    {
        m_pGsettingAppData = new QGSettings(APPDATA);
        connect(m_pGsettingAppData,&QGSettings::changed,[=](){
            this->showNormal();
            this->raise();
            this->activateWindow();
        });
    }
    // 主题适配
    if(QGSettings::isSchemaInstalled(FITTHEMEWINDOW))
    {
        m_pGsettingThemeData = new QGSettings(FITTHEMEWINDOW);

        connect(m_pGsettingThemeData,&QGSettings::changed,this, [=] (const QString &key)
        {
            if(key == "styleName")
            {
                setThemeStyle();
                this->showNormal();
                this->raise();
                this->activateWindow();
            }
        });
        setThemeStyle(); //主题安装成功之后默认做一次主题状态的判断
    }
    return ;
}

void MainWindow::setThemeStyle()
{
    QString nowThemeStyle = m_pGsettingThemeData->get("styleName").toString();
    if("ukui-dark" == nowThemeStyle || "ukui-black" == nowThemeStyle)
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
    this->setWindowFlag(Qt::WindowStaysOnTopHint,true);
    showNormal();
    activateWindow();
}

void MainWindow::makeFinish()
{
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

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_F1)
    {
        if(!m_DaemonIpcDbus->daemonIsNotRunning()){
            m_DaemonIpcDbus->showGuide("tools/kylin-usb-creator");
        }
    }
}
void MainWindow::setThemeDark()
{
    titleMin->setStyleSheet("QPushButton{background-color:rgba(255,255,255,0);border-image:url(:/data/min_h.png);border-radius:4px;}"
                             "QPushButton:hover{background-color:rgba(61,61,61,1);border-image:url(:/data/min_h.png);border-radius:4px;}"
                             "QPushButton:pressed{background-color:rgba(73,73,73,1);border-image:url(:/data/min_h.png);border-radius:4px;}");
    titleClose->setStyleSheet("QPushButton{background-color:rgba(255,255,255,0);border-image:url(:/data/close_h.png);border-radius:4px;}"
                               "QPushButton:hover{background-color:rgba(253,149,149,1);border-image:url(:/data/close_h.png);border-radius:4px;}"
                               "QPushButton:pressed{background-color:rgba(237,100,100,1);border-image:url(:/data/close_h.png);border-radius:4px;}");
    titleMenu->setStyleSheet("QPushButton{background-color:rgba(255,255,255,0);border-image:url(:/data/elements_dark/menu.png);border-radius:4px;}"
                              "QPushButton:hover{background-color:rgba(0,0,0,0.04);border-image:url(:/data/elements_dark/menu.png);border-radius:4px;}"
                              "QPushButton:pressed{background-color:rgba(0,0,0,0.08);border-image:url(:/data/elements_dark/menu.png);border-radius:4px;}"
                             "QPushButton::menu-indicator{image:None;}");
    titleText->setStyleSheet("color:rgba(249,249,249,1);font-size:14px;");
    page2->setThemeStyleDark();
    this->setStyleSheet("background-color:rgba(31,32,34,1);");
    page1->setThemeStyleDark();
}

void MainWindow::setThemeLight()
{
    titleMin->setStyleSheet("QPushButton{background-color:rgba(255,255,255,0);border-image:url(:/data/min_d.png);border-radius:4px;}"
                             "QPushButton:hover{background-color:rgba(0,0,0,0.15);border-image:url(:/data/min_d.png);border-radius:4px;}"
                             "QPushButton:pressed{background-color:rgba(0,0,0,0.3);border-image:url(:/data/min_d.png);border-radius:4px;}");
    titleClose->setStyleSheet("QPushButton{background-color:rgba(255,255,255,0);border-image:url(:/data/close_d.png);border-radius:4px;}"
                               "QPushButton:hover{background-color:rgba(253,149,149,1);border-image:url(:/data/close_h.png);border-radius:4px;}"
                               "QPushButton:pressed{background-color:rgba(237,100,100,1);border-image:url(:/data/close_h.png);border-radius:4px;}");
    titleMenu->setStyleSheet("QPushButton{background-color:rgba(255,255,255,0);border-image:url(:/data/elements_light/menu.png);border-radius:4px;}"
                              "QPushButton:hover{background-color:rgba(0,0,0,0.04);border-image:url(:/data/elements_light/menu.png);border-radius:4px;}"
                              "QPushButton:pressed{background-color:rgba(0,0,0,0.08);border-image:url(:/data/elements_light/menu.png);border-radius:4px;}"
                             "QPushButton::menu-indicator{image:None;}");
    titleText->setStyleSheet("color:rgba(48,49,51,1);font-size:14px;");
    this->setStyleSheet("background-color:rgba(255,255,255,1);");
    page1->setThemeStyleLight();
    page2->setThemeStyleLight();
}
