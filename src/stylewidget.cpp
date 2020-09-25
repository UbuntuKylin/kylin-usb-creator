#include "stylewidget.h"

StyleWidget::StyleWidget(StyleWidgetAttribute swa, QString dialogTitleText, bool isDialog )
{
    m_isDialog=isDialog;
    swshadow = new StyleWidgetShadow(swa);
    myStyle(swa);
    text->setText(dialogTitleText);
}

StyleWidget::~StyleWidget()
{
    swshadow->deleteLater();
}

void StyleWidget::WidgetStyleClose()
{
    //emit doSomethig();
    emit allClose();
    swshadow->close();
}

void StyleWidget::myStyle(StyleWidgetAttribute swa)
{
    //窗口设置
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

    body = new QWidget;//窗体
    body->setObjectName("body");

    icon = new QLabel;//图标
    icon->setPixmap(QPixmap::fromImage(QImage(":/data/icon.png")));

    text = new QLabel;//标题

    QSize smallWidgetSize(30,30);//小按钮大小
    widgetMin =new QPushButton;//最小化按钮
    widgetMin->setObjectName("widgetMin");
    widgetMin->setIconSize(smallWidgetSize);
    widgetMin->setFixedSize(smallWidgetSize);
    connect(widgetMin,&QPushButton::clicked,this,[=]{this->setWindowState(Qt::WindowMinimized);});

    widgetClose =new QPushButton;//关闭按钮
    widgetClose->setObjectName("widgetClose");
    widgetClose->setIconSize(smallWidgetSize);
    widgetClose->setFixedSize(smallWidgetSize);
    connect(widgetClose,&QPushButton::clicked,this,&StyleWidget::WidgetStyleClose);

    //布局
    QHBoxLayout *hlt0=new QHBoxLayout;//右上角按钮内部
    hlt0->setMargin(0);
    hlt0->setSpacing(0);
    hlt0->addWidget(widgetMin,1);
    hlt0->addSpacing(4);
    hlt0->addWidget(widgetClose,1);
    QVBoxLayout *vlt0=new QVBoxLayout;//右上角按钮外部
    vlt0->setMargin(0);
    vlt0->setSpacing(0);
    vlt0->addLayout(hlt0);
    vlt0->addSpacing(10);
    QHBoxLayout *hlt=new QHBoxLayout;//标题栏内部
    hlt->setMargin(0);
    hlt->setSpacing(0);
    hlt->addSpacing(swa.radius);
    hlt->addWidget(icon,1);
    hlt->addSpacing(5);
    hlt->addWidget(text,1);
    hlt->addStretch(99);
    hlt->addLayout(vlt0);
    hlt->addSpacing(2);
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
    QString bodyStyleSheet="StyleWidget #body{background-color:rgba(255,255,255,1);border-bottom-left-radius:"+QString::number(swa.radius)+
                            "px;border-bottom-right-radius:"+QString::number(swa.radius)+"px;}";
    if(swa.allRadius)
        body->setStyleSheet(bodyStyleSheet);
    else
        body->setStyleSheet("background-color:rgba(255,255,255,1);");
    text->setStyleSheet("color:#303133");
    widgetMin->setStyleSheet("StyleWidget #widgetMin{background-color:rgba(255,255,255,0);border-image:url(:/data/min_d.png);border-radius:4px;}"
                             "StyleWidget #widgetMin:hover{background-color:rgba(100,105,241,1);border-image:url(:/data/min_h.png);border-radius:4px;}"
                             "StyleWidget #widgetMin:pressed{background-color:rgba(82,87,217,1);border-image:url(:/data/min_h.png);border-radius:4px;}");
    widgetClose->setStyleSheet("StyleWidget #widgetClose{background-color:rgba(255,255,255,0);border-image:url(:/data/close_d.png);border-radius:4px;}"
                               "StyleWidget #widgetClose:hover{background-color:rgba(240,64,52,1);border-image:url(:/data/close_h.png);border-radius:4px;}"
                               "StyleWidget #widgetClose:pressed{background-color:rgba(215,51,53,1);border-image:url(:/data/close_h.png);border-radius:4px;}");

    if(m_isDialog)
    {
        icon->hide();
        title->setStyleSheet("StyleWidget #title{background-color:rgba(255,255,255,1);border-top-left-radius:"
                             +QString::number(swa.radius)+"px;border-top-right-radius:"+QString::number(swa.radius)+"px;}");
        widgetClose->hide();//初版在dialog中先暂时隐藏最小化和最大化按钮
        widgetMin->hide();


    }
    else
    {
        title->setStyleSheet("StyleWidget #title{background-color:rgba(255,255,255,0.7);border-top-left-radius:"
                             +QString::number(swa.radius)+"px;border-top-right-radius:"+QString::number(swa.radius)+"px;}");
    }
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
