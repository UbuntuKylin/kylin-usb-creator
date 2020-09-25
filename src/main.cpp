#include "mainwindow.h"


//#define ITEMHEIGHT 60//标签高度
//#define TITLEH 0//标题栏高度
#define SHADOW 6//阴影宽度
//#define WIDGETRADIUS 4//窗口圆角
#define BUTTONRADIUS 0//按钮圆角
#define SHADOWALPHA 0.16//阴影透明度
#include "stylewidget.h"

#include <QDebug>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

//    StyleWidgetAttribute swa(WINDOWW,WINDOWH,0,WIDGETRADIUS,SHADOW,SHADOWALPHA,TITLEH);
//    StyleWidget w(swa,"789798",0);


    return a.exec();
}
