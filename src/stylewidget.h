#ifndef STYLEWIDGET_H
#define STYLEWIDGET_H

#define VALUE_DIS 2000

#include "stylewidgetshadow.h"
#include <QWidget>
#include <QGraphicsEffect>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QBoxLayout> //布局
#include <QMouseEvent> //鼠标拖动事件
#include <QPoint>
#include <QListWidget>

class StyleWidget : public QWidget
{
    Q_OBJECT
signals:
    void allClose();

public:
    StyleWidget(StyleWidgetAttribute swa, QString dialogTitleText,bool isDialog=false);
    ~StyleWidget();
    QWidget *body  = nullptr;//窗体
    void showOrHide();//切换显示和隐藏状态
    void setThemeLight(); //  设置主题为浅色状态
    void setThemeDark();
    QPoint windowPos = QPoint(0,0);

private:
    bool event(QEvent *event); //鼠标离开菜单栏事件
    virtual void paintEvent(QPaintEvent *ev);//重绘窗口
    void WidgetStyleClose();//点击关闭事件
//    void WidgetStyleMenu();//点击打开菜单事件
    void myStyle(StyleWidgetAttribute swa);//设定样式
    void initMenuListWidget(StyleWidgetAttribute swa);//初始化菜单窗口
    void on_menuButton_click();//菜单栏点击事件
    bool isMouseLeavedMenuWidget(); //鼠标是否离开菜单栏


    StyleWidgetShadow *swshadow = nullptr;//U盘列表阴影
    StyleWidgetShadow *menuShadow = nullptr; //菜单阴影
//    QGraphicsBlurEffect *titleBlur = nullptr; //状态栏模糊
    QWidget *title = nullptr;//标题栏
    QLabel *text = nullptr;//标题
    QLabel *icon = nullptr;//图标
    QPushButton *widgetClose = nullptr;//关闭窗口
    QPushButton *widgetMin = nullptr;//最小化窗口
    QPushButton *widgetMenu = nullptr;//菜单
    QListWidget *menuListWidget = nullptr;//菜单列表部分
    bool m_isDialog =false;//如果是窗口
    bool paintOnce=false;//只绘制一次
    bool m_isLeftButtonPressed = false;
    QPoint m_last;
    StyleWidgetAttribute m_swa;

};

#endif // STYLEWIDGET_H
