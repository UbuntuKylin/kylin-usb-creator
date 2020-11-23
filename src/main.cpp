#include "mainwindow.h"
//#define ITEMHEIGHT 60//标签高度
//#define TITLEH 0//标题栏高度
#define SHADOW 6//阴影宽度
//#define WIDGETRADIUS 4//窗口圆角
#define BUTTONRADIUS 0//按钮圆角
#define SHADOWALPHA 0.16//阴影透明度
#include "stylewidget.h"
#include <QTranslator>
#include <QDebug>
#include <QApplication>
#include <QStringList>
#include <QStandardPaths>
#include <fcntl.h>
#include <QLibraryInfo>
#include <syslog.h>

int main(int argc, char *argv[])
{
//    高清屏幕自适应
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

//    添加vnc支持
    QStringList homePath = QStandardPaths::standardLocations(QStandardPaths::HomeLocation);
    int fd = open(QString(homePath.at(0) + "/.config/kylin-usb-creator%1.lock").arg(getenv("DISPLAY")).toUtf8().data(), O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd < 0) { exit(1); }
    if (lockf(fd, F_TLOCK, 0)) {
        syslog(LOG_ERR, "Can't lock single file, kylin-usb-creator is already running!");
        qDebug()<<"Can't lock single file, kylin-usb-creator is already running!";
        exit(0);
    }

    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":data/logo/96.png"));

//    标准对话框汉化
#ifndef QT_NO_TRANSLATION
    QString translatorFileName = QLatin1String("qt_");
    translatorFileName += QLocale::system().name();
    QTranslator *translator = new QTranslator();
    if (translator->load(translatorFileName, QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
        a.installTranslator(translator);
    else
        qDebug() << "Failed to load Chinese translation file.";
#endif

    // 应用内翻译
    QTranslator app_trans;
    QString locale = QLocale::system().name();
    QString trans_path;
    if(QDir("/usr/share/kylin-usb-creator/src/translations").exists()){
        trans_path = "/usr/share/kylin-usb-creator/translations";
    }else{
        trans_path = qApp->applicationDirPath() + "/src/translations";
    }
//    qDebug()<<"app_trans:"<<app_trans;
    qDebug()<<"trans_path:"<<trans_path;
    if(locale == "zh_CN"){
        if(!app_trans.load("kylin-usb-creator_zh_CN.qm",trans_path))
        {
            syslog(LOG_ERR, "Load translation file kylin-usb-creator_zh_CN.qm error",trans_path);
        }else{
            a.installTranslator(&app_trans);
        }
    }else if(locale == "bo_CN")
    {
        if(!app_trans.load("kylin-usb-creator_bo_CN.qm",trans_path))
        {
            syslog(LOG_ERR, "Load translation file kylin-usb-creator_bo_CN.qm error",trans_path);
        }else{
            a.installTranslator(&app_trans);
        }
    }

    MainWindow w;

    return a.exec();
}
