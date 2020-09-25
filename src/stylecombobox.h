#ifndef STYLECOMBOBOX_H
#define STYLECOMBOBOX_H

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
    QListWidget *listWidget = nullptr;//列表部分
    void addItem(QString lable, QString data);//添加到列表
    StyleWidgetShadow *swshadow = nullptr;//列表阴影


public slots:
    QString getDiskPath();
    void setPushKey(QListWidgetItem *item);//将值传递到按键

signals:


private:
    QPushButton *pushButton = nullptr;//点击部分
    void on_pushButton_click();//点击事件
    StyleWidgetAttribute swa;//属性
    bool mouseInListWidget=false;
    QLabel *text=nullptr;
    QLabel *icon=nullptr;
};

#endif // STYLECOMBOBOX_H
