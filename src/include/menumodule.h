#ifndef MENUMODULE_H
#define MENUMODULE_H
#define DEBUG_MENUMODULE 1
#define FITTHEMEWINDOW "org.ukui.style"


#include <QObject>
#include <QWidget>
#include <QMenu>
#include <QPushButton>
#include <QDebug>
#include <QString>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QRect>
#include <QScreen>
#include <QGSettings>
#include <QMetaEnum>
#include "daemonipcdbus.h"
class menuModule : public QWidget
{
    Q_OBJECT
public:
    explicit menuModule(QWidget *parent = nullptr);

signals:
    void menuModuleClose();
    void menuModuleSetThemeStyle(QString);
public:
    QPushButton *menuButton = nullptr;

public:
//    程序在实例化的时候需要传递的信息字段
    QString appName = "appName字段未填充!"; //格式kylin-usb-creator
    QString appShowingName = "app展示名字段未填充"; //格式kylin usb creator ,用来在前端展示
    QString appVersion = "appVersion字段未填充!";
    QString appDesc = "appDesc字段未填充!";
    QString iconPath = "iconPath字段未填充!";
    QString confPath = "gsetting文件路径未配置";
private:
    QMenu *m_menu = nullptr;
    QMenu *themeMenu = nullptr;
    QSize iconSize;
    QString appPath = "tools/kylin-usb-creator"; //拉起帮助菜单时使用appName字段
    QWidget *aboutWindow = nullptr;
    QGSettings *m_pGsettingThemeData = nullptr;
    QGSettings *m_pGsettingThemeStatus = nullptr;

    enum typeThemeStatus {
        themeAuto = 0,
        themeBlackOnly = 1,
        themeLightOnly = 2
    } themeStatus;

private:
    void init();
    QHBoxLayout* initTitleBar(); //关于窗口标题栏初始化
    QVBoxLayout* initBody();    // 关于窗口body初始化
    void initGsetting();
    void initAction();
    void setStyle();
    void triggerMenu(QAction* act); //主菜单动作
    void triggerThemeMenu(QAction* act); //主题动作
    void aboutAction();
    void initAbout(); //关于窗口初始化
    void helpAction();
    void setStyleByThemeGsetting();
    void setThemeLight();
    void setThemeDark();
    void updateTheme(); //点击菜单中的主题设置后更新一次主题
    void getLocalThemeSetting(); //监控程序gsetting文件修改
};

#endif // MENUMODULE_H
