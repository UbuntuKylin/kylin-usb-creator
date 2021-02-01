#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#define FITTHEMEWINDOW "org.ukui.style"
#define APPDATA "org.kylin-usb-creator-data.settings"
#define WINDOWW 680 //窗口宽度
#define WINDOWH 510//窗口高度
#define TITLEH 40//标题栏高度
#define SHADOW 6//阴影宽度
#define WIDGETRADIUS 6//窗口圆角
#define SHADOWALPHA 0.16//阴影透明度


//#ifndef MENUMODULE_H
//#define MENUMODULE_H

//#endif
#include "page1.h"
#include "page2.h"
#include <QWidget>
#include <QTimer>
#include <QSystemTrayIcon> //状态栏
#include <QGSettings>
#include <QMenu>
#include <QPushButton>
#include <QWidget>
#include <QMessageBox>
#include <QStackedWidget>
#include <QGraphicsDropShadowEffect>
#include <QKeyEvent>
#include "include/daemonipcdbus.h" //拉起帮助手册
#include "include/menumodule.h"
//#include "include/menumodule.h"

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void setMakeStart();
    void dealWrongPasswd();
public slots:
    void makeStart(); //点击授权按钮后由页面1跳转到页面2
    void makeFinish();
    void returnMain();
    void handleIconClickedSub();
    void setThemeStyle(QString);
    void dealMenuModulePullupHelp();
    void doubleCheck();
    void dealTest();
private:
    // 标题栏资源
    QWidget *title = nullptr;
    QWidget *about = nullptr;
    QLabel *titleIcon = nullptr;
    QLabel *titleText = nullptr;
    QPushButton *titleMin = nullptr;
    QPushButton *titleClose = nullptr;
    QPushButton *titleMenu = nullptr;
    QMenu *Menu = nullptr;
    QAction *actionTheme = nullptr;
    QAction *actionHelp = nullptr;
    QAction *actionAbout = nullptr;
    QAction *actionQuit = nullptr;

    void keyPressEvent(QKeyEvent* event);
    
    void init(); //初始化mainwindow相关的设置
    int changePage();
    void myStyle();//设定样式
    void createTrayActions();
    void initGsetting();
    void statusbarInit();
    void setThemeDark();
    void setThemeLight();
    void aboutClick();
    QWidget *bottomWidget = nullptr;
    menuModule *menu = nullptr;
    QTimer *dbustimer = nullptr;
    QStackedWidget *stackedWidget= nullptr;
    QLabel *pointLable1 = nullptr;  //页面小圆点
    QLabel *pointLable2 = nullptr;
    QLabel *pointLable3 = nullptr;
    Page1 *page1=nullptr;
    Page2 *page2=nullptr;
    DaemonIpcDbus *m_DaemonIpcDbus = nullptr;   //拉起帮助手册
    QSystemTrayIcon *m_mainTray = nullptr;
    QTimer *timer;
    QGSettings *m_pGsettingThemeData = nullptr;
    QGSettings *m_pGsettingAppData  = nullptr;
};
#endif // MAINWINDOW_H
