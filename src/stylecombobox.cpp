#include "stylecombobox.h"

StyleComboBox::StyleComboBox(StyleWidgetAttribute scb_swa )
{
    swa=scb_swa;
    //窗口设置
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint);//无边框
    this->setAttribute(Qt::WA_TranslucentBackground, true);//窗体透明
    this->setFixedSize(swa.w-swa.shadow,swa.h-swa.shadow);

    pushButton = new QPushButton(this);
    pushButton->setFixedSize(swa.w-swa.shadow,swa.h-swa.shadow);
    connect(pushButton,&QPushButton::clicked,this,&StyleComboBox::on_diskButton_click);

    text=new QLabel;
    text->setFixedHeight(16);
    icon=new QLabel;
    icon->setFixedSize(16,16);
    QHBoxLayout *hlt=new QHBoxLayout(pushButton);
    hlt->addWidget(text,9);
    hlt->addWidget(icon);

    listWidget=new QListWidget;
    listWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); //关闭水平滚动条
    connect(listWidget,&QListWidget::itemClicked,this,&StyleComboBox::setPushKey);

    QFont ft;
    ft.setPixelSize(14);
    listWidget->setFont(ft);
    ft.setPixelSize(12);
    text->setFont(ft);
}

void StyleComboBox::clearDiskList()
{
    listWidget->clear();
    if(isDiskListOpen)
    {
        on_diskButton_click();
    }else{
        return;
    }
}

void StyleComboBox::on_diskButton_click()
{
    if(!isDiskListOpen)
    {
        deleteShadow();
        int listWidgetHeight=listWidget->count() * swa.itemHeight+swa.shadow;
        listWidget->setFixedHeight(listWidgetHeight);
        listWidget->setFixedWidth(UDISKLISTWIDGETWIDTH);
        swa.setH(listWidgetHeight);
        swa.w = UDISKLISTWIDGETWIDTH + 6;
        //设置阴影
        swshadow =new StyleWidgetShadow(swa);
        QVBoxLayout *vblayout=new QVBoxLayout(swshadow);
        QPoint point= pushButton->mapToGlobal(QPoint(0,0));
        vblayout->setMargin(0);//控件间距
        vblayout->setSpacing(0);//控件间距
        vblayout->addWidget(listWidget);
        icon->setStyleSheet("border-image:url(:/data/comboboxIcon_c.png);border:0px;");
        if(LIGHTTHEME == themeStatus){
            pushButton->setStyleSheet(".QPushButton{background-color:#fff;border:1px solid rgba(100, 105, 241, 1);}");
        }else if(DARKTHEME == themeStatus){
            pushButton->setStyleSheet(".QPushButton{background-color:rgba(31,32,34,1);border:1px solid rgba(100,105,241,1);border-radius:4px;}");
        }
        swshadow->move(point.rx()-swa.shadow/2,point.ry()+pushButton->height()+swa.shadow);
        swshadow->show();
        listWidget->move(swa.shadow/2,swa.shadow);
        listWidget->show();
        isDiskListOpen = true;
    }
    else
    {
        deleteShadow();
        isDiskListOpen = false;
    }
}

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
        if(LIGHTTHEME == themeStatus){
            pushButton->setStyleSheet(".QPushButton{background-color:#fff;border:1px solid rgba(192, 196,204,1);border-radius:4px;}"
                                  ".QPushButton:hover{background-color:#fff;border:1px solid rgba(192,196,204,1);border-radius:4px;}");
        }else if(DARKTHEME == themeStatus){
            pushButton->setStyleSheet(".QPushButton{background-color:rgba(31,32,34,1);border:1px solid rgba(61,61,65,1);border-radius:4px;}"
                                  ".QPushButton:hover{background-color:rgba(31,32,34,1);border:1px solid rgba(61,61,65,1);border-radius:4px;}");
        }
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

void StyleComboBox::dealDiskLabelRefresh()
{
    if(1 == listWidget->count())
    {
        text->setText(listWidget->item(0)->text());
        text->setStatusTip(listWidget->item(0)->statusTip());
        emit ifStartButtonChange();
    }
}

void StyleComboBox::setThemeDark()
{
    themeStatus = DARKTHEME;
    text->setStyleSheet("background-color:rgba(31,32,34,1);color:rgba(143,147,153,1);border:0px;");
    listWidget->setStyleSheet("QListWIdget{color:rgba(61,61,65,1);}"
                "QListWidget::Item{background-color:rgba(61,61,65,1);color:rgba(249,249,249,1);padding-left:10px;}"
                              "QListWidget::Item:hover{background-color:(26,246,246,1);border:1px soild red};");
    icon->setStyleSheet("border-image:url(:/data/comboboxIcon_d.png);border:0px;");
    pushButton->setStyleSheet(".QPushButton{background-color:rgba(31,32,34,1);border:1px solid rgba(61,61,65,1);}");
}

void StyleComboBox::setThemeLight()
{
    themeStatus = LIGHTTHEME;
    text->setStyleSheet("color:rgba(96,98,101,1);border:0px;");
    listWidget->setStyleSheet("QListWidget::Item{background-color:rgba(255,255,255,1);color:rgba(96,98,102,1);padding-left:10px;}"
                              "QListWidget::Item:hover{background-color:(246,246,246,1);border:1px soild red};");
    icon->setStyleSheet("border-image:url(:/data/comboboxIcon_d.png);border:0px;");
    pushButton->setStyleSheet(".QPushButton{background-color:#fff;border:1px solid rgba(192, 196,204,1);border-radius:4px;}"
                              ".QPushButton:hover{background-color:#fff;border:1px solid rgba(192,196,204,1);border-radius:4px;}");
}
