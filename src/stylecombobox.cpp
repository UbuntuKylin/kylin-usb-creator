#include "stylecombobox.h"

StyleComboBox::StyleComboBox(StyleWidgetAttribute scb_swa )
{
    swa=scb_swa;
    //窗口设置
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint);//无边框
    this->setAttribute(Qt::WA_TranslucentBackground, true);//窗体透明
    this->setFixedSize(swa.w-swa.shadow,swa.h-swa.shadow);

    pushButton = new QPushButton(this);
    //pushButton->setLayoutDirection(Qt::LayoutDirection::RightToLeft);
    pushButton->setFixedSize(swa.w-swa.shadow,swa.h-swa.shadow);
    //pushButton->setIcon(QIcon(":/data/comboboxIcon_d.png"));
    //pushButton->setIconSize(QSize(16,16));
    connect(pushButton,&QPushButton::clicked,this,&StyleComboBox::on_pushButton_click);

    text=new QLabel;
    icon=new QLabel;
    icon->setFixedSize(16,16);
    QHBoxLayout *hlt=new QHBoxLayout(pushButton);
    hlt->addWidget(text,9);
    hlt->addWidget(icon);

    listWidget=new QListWidget;
    listWidget->setFixedWidth(swa.w-swa.shadow);
    connect(listWidget,&QListWidget::itemClicked,this,&StyleComboBox::setPushKey);

    QFont ft;
    ft.setPixelSize(14);
    listWidget->setFont(ft);
    ft.setPixelSize(12);
    text->setFont(ft);

    pushButton->setStyleSheet(".QPushButton{background-color:#fff;border:1px solid rgba(213, 216, 222, 1);}"
                              ".QPushButton:hover{background-color:#fff;border:1px solid rgba(100, 105, 241, 1);}");
    icon->setStyleSheet("border-image:url(:/data/comboboxIcon_d.png);border:0px;");
    text->setStyleSheet("color:rgba(96, 98, 101, 1);border:0px;");
    listWidget->setStyleSheet("QListWidget::Item{background-color:#fff;color:rgba(96, 98, 102, 1);padding-left:10px;}"
                              "QListWidget::Item:hover,QListWidget::Item:selected {background-color:rgba(246, 246, 246, 1);color:rgba(96, 98, 102, 1);}");
}

void StyleComboBox::on_pushButton_click()
{
    deleteShadow();
    int listWidgetHeight=listWidget->count() * swa.itemHeight+swa.shadow;
    listWidget->setFixedHeight(listWidgetHeight);
    swa.setH(listWidgetHeight);
    //设置阴影
    swshadow =new StyleWidgetShadow(swa);
    QVBoxLayout *vblayout=new QVBoxLayout(swshadow);
    QPoint point= pushButton->mapToGlobal(QPoint(0,0));
    vblayout->setMargin(0);//控件间距
    vblayout->setSpacing(0);//控件间距
    vblayout->addWidget(listWidget);
    icon->setStyleSheet("border-image:url(:/data/comboboxIcon_c.png);border:0px;");
    pushButton->setStyleSheet(".QPushButton{background-color:#fff;border:1px solid rgba(100, 105, 241, 1);}");
    swshadow->move(point.rx()-swa.shadow/2,point.ry()+pushButton->height()+swa.shadow);
    swshadow->show();
    listWidget->move(swa.shadow/2,swa.shadow);
    listWidget->show();
}

//void StyleComboBox::addItem(QString lable, QByteArray data)
void StyleComboBox::addItem(QString lable, QString data)
{
    QListWidgetItem *item=new QListWidgetItem;
    item->setText(lable);
    item->setStatusTip(data);
    item->setSizeHint(QSize(pushButton->width()-swa.shadow*2,swa.itemHeight));
    if(text->text().isEmpty())
        setPushKey(item);
    listWidget->addItem(item);
}

void StyleComboBox::deleteShadow()
{
    if(swshadow!=nullptr)
    {
        swshadow->close();
        icon->setStyleSheet("border-image:url(:/data/comboboxIcon_d.png);border:0px;");
        pushButton->setStyleSheet(".QPushButton{background-color:#fff;border:1px solid rgba(213, 216, 222, 1);}"
                                  ".QPushButton:hover{background-color:#fff;border:1px solid rgba(213, 216, 222, 1);border:1px solid rgba(100, 105, 241, 1);}");
    }
}

void StyleComboBox::setPushKey(QListWidgetItem *item)
{
    text->setText(item->text());
    text->setStatusTip(item->statusTip());
    deleteShadow();
}

QString StyleComboBox::getDiskPath()
{
    return text->statusTip();
}
