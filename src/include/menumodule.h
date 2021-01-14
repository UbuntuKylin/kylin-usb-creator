#ifndef MENUMODULE_H
#define MENUMODULE_H
#define FITTHEMEWINDOW "org.ukui.style"
#define APPDATA "org.kylin-usb-creator-data.settings"

#include <QObject>
#include <QWidget>
#include <QMenu>
#include <QPushButton>
#include <QKeyEvent>
#include <QToolButton>
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
    explicit menuModule(QWidget *);
    void themeUpdate();

signals:
    void menuModuleClose();
    void menuModuleSetThemeStyle(QString);
    void pullupHelp();
public:
    QToolButton *menuButton = nullptr;

public:
//    程序在实例化的时候需要传递的信息字段,打开debug开关后这些字段会被自动填充
    QString appName = "tools/kylin-usb-creator"; //格式kylin-usb-creator
//    QString appShowingName = "kylin usb creator"; //格式kylin usb creator ,用来在前端展示
    QString appVersion = "1.0.0";
    QString appDesc = "appDesc字段未填充!";
    QString iconPath = ":/data/kylin-usb-creator.svg";
    QString confPath = "org.kylin-usb-creator-data.settings";

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

public slots:
    void dealSystemGsettingChange(const QString);
private:
    void keyPressEvent(QKeyEvent *event);
    void init();
    QHBoxLayout* initTitleBar(); //关于窗口标题栏初始化
    QVBoxLayout* initBody();    // 关于窗口body初始化
    void initGsetting();
    void initAction();
    void setStyle();
    void triggerMenu(QAction* act); //主菜单动作4
    void triggerThemeMenu(QAction* act); //主题动作
    void aboutAction();
    void initAbout(); //关于窗口初始化
    void helpAction();
    void setThemeFromLocalThemeSetting(QList<QAction* >); //获取本地主题配置
    void setStyleByThemeGsetting(); //通过外部主题配置设置主题
    void setThemeStyle();
    void setThemeLight();
    void setThemeDark();
    QLabel* titleText = nullptr;
    QLabel* bodyAppName = nullptr;
    QLabel* bodyAppVersion = nullptr;
    QLabel* bodySupport = nullptr;
    void refreshThemeBySystemConf();    //通过系统配置更改主题
};

#endif // MENUMODULE_H
