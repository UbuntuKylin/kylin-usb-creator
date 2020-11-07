#ifndef STYLECOMBOBOX_H
#define STYLECOMBOBOX_H

#define UDISKLISTWIDGETWIDTH 330
#define DARKTHEME "dark"
#define LIGHTTHEME "light"

#include "stylewidgetshadow.h"
#include <QDebug>
#include <QWidget>
#include <QPushButton>
#include <QListWidget>
#include <QBoxLayout>
#include <QLabel>

class StyleComboBox : public QWidget
{
    Q_OBJECT
public:
    explicit StyleComboBox(StyleWidgetAttribute scb_swa);
    void deleteShadow();
    QListWidget *listWidget = nullptr;//U盘列表列表部分
//    QListWidget *menuListWidget = nullptr;//菜单列表部分
    void addItem(QString lable, QString data);//添加到列表
    StyleWidgetShadow *swshadow = nullptr;//列表阴影
    void clearDiskList(); //清空U盘的listWidget
    void setThemeDark(); //深色模式
    void setThemeLight(); //浅色模式

public slots:
    QString getDiskPath();
    void setPushKey(QListWidgetItem *item);//将值传递到按键
    void dealDiskLabelRefresh(); //插拔后U盘选中label的刷新

signals:
    void ifStartButtonChange();  //通知page1刷新开始制作按钮状态


private:
    QPushButton *pushButton = nullptr;//点击部分
    void on_diskButton_click(); //U盘栏点击事件
//    void on_menuButton_click(); //菜单栏点击事件
    StyleWidgetAttribute swa; //属性
    bool isDiskListOpen = false;
    bool mouseInListWidget=false;
    QLabel *text=nullptr; // U盘框选中的部分
    QLabel *icon=nullptr;   // U盘框上图标
};

#endif // STYLECOMBOBOX_H
