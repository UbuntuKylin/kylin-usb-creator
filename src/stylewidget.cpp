#include "stylewidget.h"

StyleWidget::StyleWidget(StyleWidgetAttribute swa, QString dialogTitleText, bool isDialog )
{
    m_isDialog=isDialog;
    swshadow = new StyleWidgetShadow(swa);
    swshadow->setWindowTitle(tr("麒麟U盘启动器"));
    myStyle(swa);
    text->setText(dialogTitleText);
}

StyleWidget::~StyleWidget()
{
    swshadow->deleteLater();
}

void StyleWidget::WidgetStyleClose()
{
    emit allClose();
    swshadow->close();
    menuListWidget->close();
}

void StyleWidget::myStyle(StyleWidgetAttribute swa)
{
    //窗口设置
    m_swa = swa;
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);//无边框
    this->setAttribute(Qt::WA_TranslucentBackground, true);//窗体透明
    this->setFixedSize(swa.w,swa.h);

    //设置阴影
    QHBoxLayout *hblayout=new QHBoxLayout(swshadow);
    hblayout->setMargin(0);//控件间距
    hblayout->setSpacing(0);//控件间距
    hblayout->addWidget(this);
    swshadow->show();

    //控件
    title = new QWidget;//标题栏
    title->setMaximumHeight(swa.titleHeight);
    title->setMinimumHeight(swa.titleHeight);
    title->setObjectName("title");

//    titleBlur = new QGraphicsBlurEffect(title);
//    titleBlur->setBlurRadius(5);
//    title->setGraphicsEffect(titleBlur);

    body = new QWidget;//窗体
    body->setObjectName("body");

    icon = new QLabel;//图标
    icon->setFixedSize(24,24);
    icon->setPixmap(QPixmap::fromImage(QImage(":/data/logo/24.png")));

    text = new QLabel;//标题

    QSize smallWidgetSize(30,30);//小按钮大小
    widgetMin =new QPushButton;//最小化按钮
    widgetMin->setObjectName("widgetMin");
    widgetMin->setIconSize(smallWidgetSize);
    widgetMin->setFixedSize(smallWidgetSize);
    connect(widgetMin,&QPushButton::clicked,this,[=]{
        swshadow->setWindowState(Qt::WindowMinimized);
    });
    widgetClose =new QPushButton;//关闭按钮
    widgetClose->setObjectName("widgetClose");
    widgetClose->setIconSize(smallWidgetSize);
    widgetClose->setFixedSize(smallWidgetSize);
    connect(widgetClose,&QPushButton::clicked,this,&StyleWidget::WidgetStyleClose);

    widgetMenu = new QPushButton;//菜单按钮
    widgetMenu->setObjectName("widgetMenu");
    widgetMenu->setIconSize(smallWidgetSize);
    widgetMenu->setFixedSize(smallWidgetSize);
    connect(widgetMenu,&QPushButton::clicked,this,&StyleWidget::on_menuButton_click);

    //布局
    QHBoxLayout *hlt0=new QHBoxLayout;//右上角按钮内部
    hlt0->setMargin(0);
    hlt0->setSpacing(0);
    hlt0->addStretch();
//    hlt0->addWidget(widgetMenu,1);
//    hlt0->setSpacing(4);
    hlt0->addWidget(widgetMin,1);
    hlt0->addSpacing(4);
    hlt0->addWidget(widgetClose,1);
    hlt0->addSpacing(4);
    QVBoxLayout *vlt0=new QVBoxLayout;//右上角按钮外部
    vlt0->setMargin(0);
    vlt0->setSpacing(0);
    vlt0->addSpacing(4);
    vlt0->addLayout(hlt0);
    vlt0->addStretch(); // 顶上充满
    QHBoxLayout *hlt=new QHBoxLayout;//标题栏内部
    hlt->setMargin(0);
    hlt->setSpacing(0);
    hlt->addSpacing(8);
    hlt->addWidget(icon,1);
    hlt->addSpacing(8);
    hlt->addWidget(text,1);
    hlt->addStretch(99);
    hlt->addLayout(vlt0);
//    hlt->addSpacing(2);
    title->setLayout(hlt);
    QHBoxLayout *hlt2=new QHBoxLayout;//标题栏外部
    hlt2->setMargin(0);
    hlt2->setSpacing(0);
    hlt2->addSpacing(swa.shadow+1);
    hlt2->addWidget(title);
    hlt2->addSpacing(swa.shadow+1);
    QHBoxLayout *hlt3=new QHBoxLayout;//窗体
    hlt3->setMargin(0);
    hlt3->setSpacing(0);
    hlt3->addSpacing(swa.shadow+1);
    hlt3->addWidget(body);
    hlt3->addSpacing(swa.shadow+1);
    QVBoxLayout *vl=new QVBoxLayout;//总体
    vl->setMargin(0);
    vl->setSpacing(0);
    vl->addSpacing(swa.shadow);
    vl->addLayout(hlt2,1);
    vl->addLayout(hlt3,99);
    vl->addSpacing(swa.shadow+3);
    this->setLayout(vl);

    //样式表stylesheet//测试用border:1px solid red;
//    QString bodyStyleSheet="StyleWidget #body{background-color:rgba(255,255,255,1);border-bottom-left-radius:"+QString::number(swa.radius)+
//                            "px;border-bottom-right-radius:"+QString::number(swa.radius)+"px;}";
//    if(swa.allRadius)
//        body->setStyleSheet(bodyStyleSheet);
//    else
//        body->setStyleSheet("background-color:rgba(255,255,255,1);");
//    text->setStyleSheet("color:#303133");

//    // 设置page1中菜单、最小化、关闭三个按钮样式
//    widgetMin->setStyleSheet("StyleWidget #widgetMin{background-color:rgba(255,255,255,0);border-image:url(:/data/min_d.png);border-radius:4px;}"
//                             "StyleWidget #widgetMin:hover{background-color:rgba(0,0,0,0.04);border-image:url(:/data/min_d.png);border-radius:4px;}"
//                             "StyleWidget #widgetMin:pressed{background-color:rgba(0,0,0,0.08);border-image:url(:/data/min_d.png);border-radius:4px;}");

//    widgetClose->setStyleSheet("StyleWidget #widgetClose{background-color:rgba(255,255,255,0);border-image:url(:/data/close_d.png);border-radius:4px;}"
//                               "StyleWidget #widgetClose:hover{background-color:rgba(253,149,149,1);border-image:url(:/data/close_h.png);border-radius:4px;}"
//                               "StyleWidget #widgetClose:pressed{background-color:rgba(237,100,100,1);border-image:url(:/data/close_h.png);border-radius:4px;}");

//    widgetMenu->setStyleSheet("StyleWidget #widgetMenu{background-color:rgba(255,255,255,0);border-image:url(:/data/menu.png);border-radius:4px;}"
//                              "StyleWidget #widgetMenu:hover{background-color:rgba(0,0,0,0.04);border-image:url(:/data/menu.png);border-radius:4px;}"
//                              "StyleWidget #widgetMenu:pressed{background-color:rgba(0,0,0,0.08);border-image:url(:/data/menu.png);border-radius:4px;}");

    if(m_isDialog)
    {
        icon->hide();
//        title->setStyleSheet("StyleWidget #title{background-color:rgba(255,255,255,1);border-top-left-radius:"
//                             +QString::number(swa.radius)+"px;border-top-right-radius:"+QString::number(swa.radius)+"px;}");
        widgetClose->hide();//dialog中隐藏min close和menu按钮
        widgetMin->hide();
        widgetMenu->hide();
    }
    else
    {
//        title->setStyleSheet("StyleWidget #title{background-color:rgba(255,255,255,0.7);border-top-left-radius:"
//                             +QString::number(swa.radius)+"px;border-top-right-radius:"+QString::number(swa.radius)+"px;}");
        initMenuListWidget(swa);
        // 在资源初始化完毕只有在这里重新调用资源，绘制前端页面

    }
    setThemeLight();
//    setThemeDark();
}

void StyleWidget::initMenuListWidget(StyleWidgetAttribute swa)
{
//        qDebug()<<"initMenuListWidget";
        menuShadow = new StyleWidgetShadow(swa); //带阴影的菜单项
        menuListWidget = new QListWidget;
        QVBoxLayout *vblayout = new QVBoxLayout(menuShadow);
        vblayout->setMargin(0);//控件间距
        vblayout->setSpacing(0); //控件间距
        menuListWidget->addItem(tr("Theme"));
        vblayout->addSpacing(2);
        menuListWidget->addItem(tr("Help"));
        vblayout->addSpacing(2);
        menuListWidget->addItem(tr("About"));
        vblayout->addSpacing(2);
        menuListWidget->addItem(tr("Quit"));
        menuListWidget->setStyleSheet("QListWidget::Item{background-color:#F7F7F7;color:rgba(96, 98, 102, 1);padding-left:10px;border-radius:2px;height:30px;border-radius:2px;}"
                                      "QListWidget::Item:hover,QListWidget::Item:selected {background-color:rgba(247, 247, 247, 1);color:rgba(96, 98, 102, 1);}"
                                      "QListWidget{font-size:14px;}");
        menuShadow->setFixedSize(97,138);
//        windowPos = this->mapToGlobal(QPoint(0,0));
//        qDebug()<<"stylewidget"<<windowPos;
        menuShadow->move(windowPos.rx()+589,windowPos.ry()+46);
//        qDebug()<<windowPos;
        menuShadow->setStyleSheet("QWidget{height:138px;width:97px;}");
        vblayout->addWidget(menuListWidget);
        menuShadow->hide();
        menuListWidget->hide();
}

void StyleWidget::on_menuButton_click()
{
        menuShadow->show();
        menuListWidget->show();
}

void StyleWidget::paintEvent(QPaintEvent *event)//重绘窗口
{
    if(paintOnce)return;
    //设置字体字号
    QFont ft;
    ft.setPixelSize(14);
    text->setFont(ft);
    paintOnce=true;
}

void StyleWidget::showOrHide()
{
    if(this->isHidden())
    {
        swshadow->show();
        this->show();
    }
    else
    {
        swshadow->hide();
        this->hide();
    }
}

bool StyleWidget::event(QEvent *event)
{
    if(nullptr == menuShadow)
    {
        return QWidget::event(event);
    }
    if(event->type() == QEvent::Leave)
    {
        if(isMouseLeavedMenuWidget())
        {
            menuShadow->close();
        }
    }else if(event->type() == QEvent::MouseButtonPress)
    {
        menuShadow->close();
    }
    return QWidget::event(event);
}

bool StyleWidget::isMouseLeavedMenuWidget()
{
    QPoint mouse = QCursor::pos();
    QPoint thisWidget = this->mapToGlobal(this->pos());
    QSize thisWidgetSize = this->size();
    if(mouse.rx()<=thisWidget.rx() || mouse.rx()>=thisWidget.rx()+thisWidgetSize.width() || mouse.ry()<=thisWidget.ry() || mouse.ry()>=thisWidget.ry()+thisWidgetSize.height())
        return true;
    else
        return false;
}

void StyleWidget::setThemeLight()
{
//    qDebug()<<"StyleWidget::setThemeLight被调用";
    //样式表stylesheet//测试用border:1px solid red;
    QString bodyStyleSheet="StyleWidget #body{background-color:rgba(255,255,255,1);border-bottom-left-radius:"+QString::number(m_swa.radius)+
                            "px;border-bottom-right-radius:"+QString::number(m_swa.radius)+"px;}";
    if(m_swa.allRadius)
    {
        body->setStyleSheet(bodyStyleSheet);
    }else{
        body->setStyleSheet("background-color:rgba(255,255,255,1);");
    }
    text->setStyleSheet("color:#303133");

    // 设置page1中菜单、最小化、关闭三个按钮样式
    widgetMin->setStyleSheet("StyleWidget #widgetMin{background-color:rgba(255,255,255,0);border-image:url(:/data/min_d.png);border-radius:4px;}"
                             "StyleWidget #widgetMin:hover{background-color:rgba(0,0,0,0.04);border-image:url(:/data/min_d.png);border-radius:4px;}"
                             "StyleWidget #widgetMin:pressed{background-color:rgba(0,0,0,0.08);border-image:url(:/data/min_d.png);border-radius:4px;}");

    widgetClose->setStyleSheet("StyleWidget #widgetClose{background-color:rgba(255,255,255,0);border-image:url(:/data/close_d.png);border-radius:4px;}"
                               "StyleWidget #widgetClose:hover{background-color:rgba(253,149,149,1);border-image:url(:/data/close_h.png);border-radius:4px;}"
                               "StyleWidget #widgetClose:pressed{background-color:rgba(237,100,100,1);border-image:url(:/data/close_h.png);border-radius:4px;}");

    widgetMenu->setStyleSheet("StyleWidget #widgetMenu{background-color:rgba(255,255,255,0);border-image:url(:/data/menu.png);border-radius:4px;}"
                              "StyleWidget #widgetMenu:hover{background-color:rgba(0,0,0,0.04);border-image:url(:/data/menu.png);border-radius:4px;}"
                              "StyleWidget #widgetMenu:pressed{background-color:rgba(0,0,0,0.08);border-image:url(:/data/menu.png);border-radius:4px;}");
    if(m_isDialog)
    {
        title->setStyleSheet("StyleWidget #title{background-color:rgba(255,255,255,1);border-top-left-radius:"
                             +QString::number(m_swa.radius)+"px;border-top-right-radius:"+QString::number(m_swa.radius)+"px;}");
    }else
    {
        title->setStyleSheet("StyleWidget #title{background-color:rgba(255,255,255,0.7);border-top-left-radius:"
                             +QString::number(m_swa.radius)+"px;border-top-right-radius:"+QString::number(m_swa.radius)+"px;}");
    }

}

void StyleWidget::setThemeDark()
{
    qDebug()<<"StyleWidget::setThemeDark被调用";
    QString bodyStyleSheet="StyleWidget #body{background-color:rgba(255,255,255,1);border-bottom-left-radius:"+QString::number(m_swa.radius)+
                            "px;border-bottom-right-radius:"+QString::number(m_swa.radius)+"px;}";
    if(m_swa.allRadius)
    {
        body->setStyleSheet(bodyStyleSheet);
    }else{
        body->setStyleSheet("background-color:rgba(31,32,34,1);");
    }

    if(m_isDialog)
    {
        title->setStyleSheet("StyleWidget #title{background-color:rgba(255,255,255,1);border-top-left-radius:"
                             +QString::number(m_swa.radius)+"px;border-top-right-radius:"+QString::number(m_swa.radius)+"px;}");
    }else
    {
        title->setStyleSheet("StyleWidget #title{background-color:rgba(255,255,255,0.7);border-top-left-radius:"
                             +QString::number(m_swa.radius)+"px;border-top-right-radius:"+QString::number(m_swa.radius)+"px;}");
    }


}
