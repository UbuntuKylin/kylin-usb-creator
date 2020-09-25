#ifndef PAGE2_H
#define PAGE2_H

#include <QWidget>
#include <QDebug>
#include <QPushButton>
#include <QLabel>
#include <QMovie>
#include <QProcess>
#include <QBoxLayout>
#include <QString>
#include <QFileInfo>
class Page2 : public QWidget
{
    Q_OBJECT
public:
    explicit Page2(QWidget *parent = nullptr);
    qint64 getFileSize(QString filePath);//获取文件大小，返回单位mb
signals:
    void makeFinish();
    void returnMain();
    void swToPage2();//

public slots:
    void startMaking(QString key,QString sourcePath,QString targetPath); //接收page1传来的制作参数
    void finishEvent();
private:
    void playLoadingGif();
    void playFinishGif();
    QMovie *movieLoading=nullptr;
    QMovie *movieFinish=nullptr;
    QLabel *lableMovie=nullptr;
    QLabel *lableText=nullptr;
    QLabel *lableNum=nullptr;
    QPushButton *returnPushButton=nullptr;
    int frameCount=0;
    QProcess *command_dd;
    void readBashStandardErrorInfo();
    qint64 sourceFileSize = 0;
    bool isInPage2 = false;
    QTimer *timer;
};

#endif // PAGE2_H
