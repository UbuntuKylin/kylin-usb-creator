#define SHADOW 6//阴影宽度
#define BUTTONRADIUS 0//按钮圆角
#define SHADOWALPHA 0.16//阴影透明度
#include "include/qtsingleapplication.h"
#include <QTranslator>
#include <QDebug>
#include <QApplication>
#include <QStringList>
#include <QStandardPaths>
#include <ukui-log4qt.h>
#include <fcntl.h>
#include <QLibraryInfo>
#include "include/xatom-helper.h"
#include "mainwindow.h"
#include "dbusadaptor.h"
#include "include/qtsingleapplication.h"

// 拉起最小化窗口
void activeMainwindow()
{
    if(QGSettings::isSchemaInstalled(APPDATA))
    {
        QGSettings *p = new QGSettings(APPDATA);
        qDebug()<<"main.cpp activeMainwindow value:"<<p->get("mode").toString();
        p->set("mode","stat4");
        qDebug()<<"main.cpp after set value:"<<p->get("mode").toString();
    }
}
int main(int argc, char *argv[])
{
//    init log module
    initUkuiLog4qt("usb-boot-maker");
//    高清屏幕自适应
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    QtSingleApplication a(argc,argv);
    qApp->setProperty("noChangeSystemFontSize",true);
//    TODO: 整合qt的标准翻译和自行定制的qm翻译
//    标准对话框汉化(QT标准翻译
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
            trans_path = qApp->applicationDirPath() + ":/src/translations";
        }
        if(locale == "zh_CN"){
            qDebug()<<app_trans.load(":/src/translations/kylin-usb-creator_zh_CN.qm");
            if(!app_trans.load(":/src/translations/kylin-usb-creator_zh_CN.qm"))
            {
                qDebug()<<"Load translation file kylin-usb-creator_zh_CN.qm error";
            }else{
                a.installTranslator(&app_trans);
            }
        }else if(locale == "bo_CN")
        {
            if(!app_trans.load("kylin-usb-creator_bo_CN.qm",trans_path))
            {
                qDebug()<<"Load translation file kylin-usb-creator_bo_CN.qm error";
            }else{
                a.installTranslator(&app_trans);
            }
        }

    if(a.isRunning()){
        a.sendMessage(QApplication::arguments().length() > 1 ? QApplication::arguments().at(0):a.applicationFilePath());
        qWarning()<<"kylin-usb-creator is already running";
        return EXIT_SUCCESS;
    }else {
        MainWindow w;
        w.handleIconClickedSub();

        QObject::connect(&a,SIGNAL(messageReceived(const QString&)),&w,SLOT(handleIconClickedSub()));
        return a.exec();
    }
    return a.exec();
}
