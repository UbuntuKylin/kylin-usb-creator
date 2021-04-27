#ifndef PAGE2_H
#define PAGE2_H

#define DARKTHEME "dark"
#define LIGHTTHEME "light"

#include <QWidget>
#include <QDebug>
#include <QPushButton>
#include <QLabel>
#include <QMovie>
#include <QProcess>
#include <QBoxLayout>
#include <QString>
#include <QFileInfo>
#include <QStorageInfo>
#include <QTimer>
#include <QDBusConnection>

class Page2 : public QWidget
{
    Q_OBJECT
public:
    explicit Page2(QWidget *parent = nullptr);
    qint64 getFileSize(QString filePath); //获取文件大小，返回单位mb
    void setThemeStyleDark();
    void setThemeStyleLight();

signals:
    void makeFinish();
    void returnMain();
    void swToPage2();//

public slots:
    void startMaking();
    void finishEvent();
    void dealWorkingProgress(int);
    void dealMakeFinish(QString);
private:
    enum typeMovieStatus {
        loading = 0,
        finish = 1,
        failed = 2
        } movieStatus;   //枚举类型的动画进度
    void playLoadingGif();
    void playFinishGif();
    void playErrorGif();
    bool isMakingSuccess(); //进度到100%之后检查制作是否成功
    void movieRefresh(); //切换主题时重新加载动画
    QString uDiskPath = ""; //U盘路径
    QString themeStatus = LIGHTTHEME;
    QMovie *movieLoading_l=nullptr; //过程动画，黑白两套
    QMovie *movieFinish_l=nullptr;
    QMovie *movieLoading_d=nullptr;
    QMovie *movieFinish_d=nullptr;
    QPixmap errLabel;
    QLabel *lableMovie=nullptr;
    QLabel *lableText=nullptr;
    QLabel *lableNum=nullptr;
    QPushButton *returnPushButton=nullptr; // 回page1的按钮
    QProcess *command_dd;

    int frameCount=0;
    qint64 sourceFileSize = 0;
    QTimer *timer;
};

#endif // PAGE2_H
