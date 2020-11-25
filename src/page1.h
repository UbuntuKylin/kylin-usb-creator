#ifndef PAGE1_H
#define PAGE1_H

#define NOUDISK "noUDisk"
#define UDISK_NAME_MAX_LENGTH 27//U盘名称最大长度
#define COMBOBOXW 370 //下拉框宽度
#define COMBOBOXH 30//下拉框高度
#define ITEMHEIGHT 30//下拉框标签高度
#define COMBOBOXRADIUS 6//下拉框窗口圆角
#define DARKTHEME "dark"
#define LIGHTTHEME "light"

#include "stylewidget.h"
#include "stylecombobox.h"
#include "rootauthdialog.h"

#include <QDebug>
#include <QRect>
#include <QWidget> //控件
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QTimer>
#include <QBoxLayout> //布局
#include <QFileDialog>//打开文件
#include <QStorageInfo>//硬盘信息
#include <QFileSystemWatcher>
#include <QGraphicsDropShadowEffect>

class Page1 : public QWidget
{
    Q_OBJECT
public:
    explicit Page1();
    bool ifStartBtnChange();//开始制作按钮是否可以点击
    void setThemeStyleLight(); //设置浅色主题
    void setThemeStyleDark(); //设置深色主题

signals:
    void makeStart(QString key,QString sourcePath,QString targetPath);
    void diskLabelRefresh(); //  已选中U盘刷新
    void setStyleWidgetStyle(const QString);

public slots:
    void allClose();
    void refreshDiskList();
    void dealRightPasswd(); //处理密码正确
    void dealAuthDialogClose();  //处理授权框关闭
    void dealComboBoxChangeButton(); //combobox通知page1检查开始按钮是否可以亮起


private:
    bool event(QEvent *event); // 鼠标离开U盘列表事件
    void creatStartSlots();//开始制作
    void initControlQss();//初始化控件及其样式
    void getStorageInfo();//获取磁盘信息
    void dialogInitControlQss();//初始化对话框控件及其样式
    bool mouseIsLeaveUdiskWidget();//鼠标是否离开U盘列表
    void dealDialogCancel();     // 处理授权框关闭及取消
    void udiskPlugWatcherInit(); //U盘插拔监控初始化

    QString themeStatus = LIGHTTHEME; //主题指示器
    QWidget *rootWindowTitle = nullptr; //root授权框状态栏
    QLabel *rootDialogTitleText = nullptr; //root授权框标题
    QPushButton *rootDialogClose = nullptr; //root授权框关闭按钮
    QPushButton *rootDialogMin = nullptr; //root授权框最小化按钮
    QLabel *dialogKeyLable = nullptr; //root授权框输入密码label
    QLabel *divingLine = nullptr;    //授权框1px分割线
    QTimer *diskRefreshDelay = nullptr; //U盘插入后等待系统挂载的时间
    StyleComboBox *comboUdisk = nullptr;//U盘列表
    QLabel *tabIso = nullptr;//选择镜像标签
    QLabel *tabUdisk = nullptr;//选择U盘标签
    QLabel *warnningIcon = nullptr;//警告标志
    QLabel *warnningText = nullptr;//警告标语tabUdisk
    QLineEdit *urlIso = nullptr;//显示镜像路径
    QPushButton *findIso = nullptr;//浏览文件按钮
    QPushButton *creatStart = nullptr;//开始制作
    rootAuthDialog *authDialog = nullptr;//root授权对话框
    QGraphicsDropShadowEffect *shadowEffect = nullptr; //root授权框阴影
    QWidget *authDialogContentWidget = nullptr; //root授权对话框的子窗口，用来在上边加载控件。
    QFileSystemWatcher *udiskplugwatcher = nullptr; //U盘插拔监控器
    StyleWidgetAttribute swa;//属性
    QLabel *dialogWarningIcon = nullptr; //授权框警告Icon
    QLabel *dialogWarningLable = nullptr; //授权框警告label
    QLabel *dialogWarningLable2 = nullptr;
    QPushButton *dialogNo = nullptr;
    QPushButton *dialogYes = nullptr;

    bool paintOnce=false;//只绘制一次
    bool leaveThis=true;

};

#endif // PAGE1_H
