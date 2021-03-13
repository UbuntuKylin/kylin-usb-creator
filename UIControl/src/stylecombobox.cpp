#include "stylecombobox.h"

StyleComboBox::StyleComboBox()
{
    //窗口设置
    this->setAttribute(Qt::WA_TranslucentBackground, true);//窗体透明
    this->setFixedSize(370,30);

    pushButton = new QPushButton(this);
    pushButton->setFixedSize(370,30);
    connect(pushButton,&QPushButton::clicked,this,&StyleComboBox::on_diskButton_click);

    text=new QLabel;
    text->setFixedHeight(16);
    icon=new QLabel;
    icon->setFixedSize(16,16);
    QHBoxLayout *hlt=new QHBoxLayout(pushButton);
    hlt->addWidget(text,9);
    hlt->addWidget(icon);

    //下拉弹窗以及阴影绘制
    listWidget=new QListWidget;
    listWidget->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
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
        closeListWidget();
        int listWidgetHeight=listWidget->count() * ITEMHIGHT + SHADOW;
        listWidget->setFixedHeight(listWidgetHeight);
        listWidget->setFixedWidth(UDISKLISTWIDGETWIDTH);

        QPoint point= pushButton->mapToGlobal(QPoint(0,0));
        icon->setStyleSheet("border-image:url(:/data/comboboxIcon_c.png);border:0px;");
        if(LIGHTTHEME == themeStatus){
            pushButton->setStyleSheet(".QPushButton{background-color:#fff;border:1px solid rgba(100, 105, 241, 1);border-radius:4px;}");
            listWidget->setStyleSheet("QListWIdget{color:rgba(161,61,65,1);border:4px soild blue;}"
                                      "QListWidget::Item{background-color:rgba(255,255,255,1);border-radius:2px;padding-left:20px;color:rgba(96,98,102,1);}"
                                      "QListWidget::Item:hover{background-color:rgba(246,246,246,1);}");
        }else if(DARKTHEME == themeStatus){
            pushButton->setStyleSheet(".QPushButton{background-color:rgba(31,32,34,1);border:1px solid rgba(100,105,241,1);border-radius:4px;color:rgba(143,417,153,1);}");
            listWidget->setStyleSheet("QListWidget::Item{background-color:rgba(61,61,65,1);border-radius:2px;color:rgba(249,249,249,1);padding-left:20px;}"
                                      "QListWidget::Item:hover{background-color:rgba(72,72,76,1);color:rgba(249,249,249,1);}");
        }
        listWidget->move(point.rx()-SHADOW/3 + 2,point.ry()+pushButton->height()+ SHADOW);

        listWidget->show();
        isDiskListOpen = true;
    }
    else
    {
        isDiskListOpen = false;
        listWidget->close();
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


void StyleComboBox::addItem(QString lable, QString data)
{
    QListWidgetItem *item=new QListWidgetItem;
    item->setText(lable);
    item->setStatusTip(data);
    item->setSizeHint(QSize(pushButton->width()-SHADOW * 2,ITEMHIGHT));
    if(text->text().isEmpty())
        setPushKey(item);
    listWidget->addItem(item);
}

void StyleComboBox::closeListWidget()
{
    isDiskListOpen = false;
    icon->setStyleSheet("border-image:url(:/data/comboboxIcon_d.png);border:0px;");
    listWidget->close();
    if(LIGHTTHEME == themeStatus){
        pushButton->setStyleSheet(".QPushButton{background-color:#fff;border:1px solid rgba(192, 196,204,1);border-radius:4px;}"
                              ".QPushButton:hover{background-color:#fff;border:1px solid rgba(192,196,204,1);border-radius:4px;}");
    }else if(DARKTHEME == themeStatus){
        pushButton->setStyleSheet(".QPushButton{background-color:rgba(31,32,34,1);border:1px solid rgba(61,61,65,1);border-radius:4px;}"
                              ".QPushButton:hover{background-color:rgba(31,32,34,1);border:1px solid rgba(61,61,65,1);border-radius:4px;}");
    }
}
void StyleComboBox::setPushKey(QListWidgetItem *item)
{
    text->setText(item->text());
    text->setStatusTip(item->statusTip());
    closeListWidget();
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
    listWidget->setStyleSheet("QListWIdget{color:rgba(61,61,65,1);border:4px soild blue;}"
                              "QListWidget::Item{background-color:rgba(61,61,65,1);color:rgba(249,249,249,1);padding-left:10px;}"
                              "QListWidget::Item:hover{background-color:(26,246,246,1);border:1px soild red};");
    icon->setStyleSheet("border-image:url(:/data/comboboxIcon_d.png);border:0px;");
    pushButton->setStyleSheet(".QPushButton{background-color:rgba(31,32,34,1);border:1px solid rgba(61,61,65,1);}");
}

void StyleComboBox::setThemeLight()
{
    themeStatus = LIGHTTHEME;
    text->setStyleSheet("color:rgba(96,98,101,1);border:0px;");
    listWidget->setStyleSheet("QListWidget{border:4px soild blue;}"
                              "QListWidget::Item{background-color:rgba(255,255,255,1);color:rgba(96,98,102,1);padding-left:10px;}"
                              "QListWidget::Item:hover{background-color:(246,246,246,1);border:1px soild red};");
    icon->setStyleSheet("border-image:url(:/data/comboboxIcon_d.png);border:0px;");
    pushButton->setStyleSheet(".QPushButton{background-color:#fff;border:1px solid rgba(192, 196,204,1);border-radius:4px;}"
                              ".QPushButton:hover{background-color:#fff;border:1px solid rgba(192,196,204,1);border-radius:4px;}");
}
