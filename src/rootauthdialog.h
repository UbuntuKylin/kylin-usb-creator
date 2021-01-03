#ifndef ROOTAUTHDIALOG_H
#define ROOTAUTHDIALOG_H

#include <QWidget>
#include <QProcess>
#include <QString>
#include <QByteArray>
#include <QDebug>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QStyleOption>
#include <QPainter>
#include <QPushButton>
#include <QEvent>

class rootAuthDialog:public QWidget
{
    Q_OBJECT
public:
    rootAuthDialog(QWidget *parent = nullptr); // 构造函数
    QPushButton *btnOk = nullptr;
    QPushButton *btnCancel = nullptr;
    QLineEdit *dialogKey = nullptr;   //输入框

signals:
    void passwdCorrect();
    void cancelCheck();

public slots:
    void checkPassWord();

private:
    void Init();
    void readBashOutput();
    void dealWrongPasswd(); //处理密码错误

    void dealNotSudoers();  //处理当前用户不在sudoers名单中
//    void dealTooShort(); //未输入密码直接点击了确认的情况
    QProcess *command_sudo = nullptr;

    QString m_key ;     //密码
    bool event(QEvent *event);
};

#endif // ROOTAUTHDIALOG_H
