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

#include "page1.h"
#include "page2.h"
#include <QWidget>
#include <QTimer>
#include <QSystemTrayIcon> //状态栏
#include <QGSettings>
//滑动效果
#include <QWidget>
#include <QStackedWidget>
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
//    void handleIconClickedSub(const QString&);
//    void passwdCheck();
private:
    void init(); //初始化mainwindow相关的设置
    int changePage();
    void myStyle();//设定样式
    void test();
    void createTrayActions();
    void initGsetting();
    void setThemeStyle();

    void handleIconClickedSub();
    QTimer *dbustimer = nullptr;
    QStackedWidget *stackedWidget= nullptr;
    //页面小圆点
    QLabel *pointLable1 = nullptr;
    QLabel *pointLable2 = nullptr;
    QLabel *pointLable3 = nullptr;
    Page1 *page1=nullptr;
    Page2 *page2=nullptr;
    QSystemTrayIcon *m_mainTray = nullptr;
    QTimer *timer;
//    bool isInPage2  = false; //程序是否处在页面2
    QGSettings *m_pGsettingThemeData = nullptr;
    QGSettings *m_pGsettingAppData  = nullptr;
};
#endif // MAINWINDOW_H
