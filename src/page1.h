#ifndef PAGE1_H
#define PAGE1_H

#define NOUDISK "noUDisk"
#define UDISK_NAME_MAX_LENGTH 30//U盘名称最大长度
#define COMBOBOXW 370 //下拉框宽度
#define COMBOBOXH 30//下拉框高度
#define ITEMHEIGHT 30//下拉框标签高度
#define COMBOBOXRADIUS 6//下拉框窗口圆角

#include "stylewidget.h"
#include "stylecombobox.h"
#include <QDebug>
//控件
#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
//布局
#include <QBoxLayout>
//打开文件
#include <QFileDialog>
//硬盘信息
#include <QStorageInfo>

class Page1 : public QWidget
{
    Q_OBJECT
public:
    explicit Page1( StyleWidgetAttribute);
    void ifStartBtnChange();//开始制作按钮是否可以点击
//
signals:
    void makeStart(QString key,QString sourcePath,QString targetPath);
//    void changePage1Btn();
public slots:
    void doSomethig();
    void allClose();
    void onDialogYesClick();
    void dealWrongPasswd();
private:
    bool event(QEvent *event);
    void creatStartSlots();//开始制作
    void initControlQss();//初始化控件及其样式
    void getStorageInfo();//获取磁盘信息
    void dialogInitControlQss(StyleWidgetAttribute page_swa);//初始化对话框控件及其样式
    bool mouseIsLeave();//鼠标是否离开
    void dealDialogCancel();

    //QComboBox *comboUdisk = nullptr;//U盘列表
    StyleComboBox *comboUdisk = nullptr;//U盘列表
    QLabel *tabIso = nullptr;//选择镜像标签
    QLabel *tabUdisk = nullptr;//选择U盘标签
    QLabel *warnningIcon = nullptr;//警告标志
    QLabel *warnningText = nullptr;//警告标语
    QLineEdit *urlIso = nullptr;//显示镜像路径
    QPushButton *findIso = nullptr;//浏览文件按钮
    QPushButton *creatStart = nullptr;//开始制作
    StyleWidget *styleDialog = nullptr;//提醒对话框
    QLineEdit *dialogKey= nullptr;//

    bool paintOnce=false;//只绘制一次

    StyleWidgetAttribute swa;//属性

    bool leaveThis=true;

};

#endif // PAGE1_H
