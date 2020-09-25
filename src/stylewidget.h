#ifndef STYLEWIDGET_H
#define STYLEWIDGET_H

#define VALUE_DIS 2000

#include "stylewidgetshadow.h"

//控件
#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
//布局
#include <QBoxLayout>
//鼠标拖动事件
#include <QMouseEvent>
#include <QPoint>
class StyleWidget : public QWidget
{
    Q_OBJECT
signals:
    void allClose();
    void doSomethig();

public:
    StyleWidget(StyleWidgetAttribute swa, QString dialogTitleText,bool isDialog=false);
    ~StyleWidget();
    QWidget *body  = nullptr;//窗体
    void showOrHide();//切换显示和隐藏状态

private:
    virtual void paintEvent(QPaintEvent *ev);//重绘窗口
    void WidgetStyleClose();//点击关闭事件
    void myStyle(StyleWidgetAttribute swa);//设定样式
    StyleWidgetShadow *swshadow = nullptr;//阴影
    QWidget *title = nullptr;//标题栏
    QLabel *text = nullptr;//标题
    QLabel *icon = nullptr;//图标
    QPushButton *widgetClose = nullptr;//关闭窗口
    QPushButton *widgetMin = nullptr;//最小化窗口
    bool m_isDialog =false;//如果是窗口
    bool paintOnce=false;//只绘制一次
    bool m_isLeftButtonPressed = false;
    QPoint m_last;
};

#endif // STYLEWIDGET_H
