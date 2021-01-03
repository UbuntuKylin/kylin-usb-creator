#include "rootauthdialog.h"

rootAuthDialog::rootAuthDialog(QWidget *parent)
{
    Init();
}

void rootAuthDialog::Init()
{
    btnCancel = new QPushButton;
    btnOk = new QPushButton;
    dialogKey = new QLineEdit;
    dialogKey->setPlaceholderText(tr("Input password"));
    dialogKey->setEchoMode(QLineEdit::Password);
    setWindowModality(Qt::WindowModal); //设置属性为模态窗口
    connect(btnOk,&QPushButton::clicked,this,&rootAuthDialog::checkPassWord);
    connect(btnCancel,&QPushButton::clicked,[=]{
        emit cancelCheck();}); //取消信号
    connect(dialogKey,&QLineEdit::returnPressed,btnOk,&QAbstractButton::click,Qt::UniqueConnection);
}

void rootAuthDialog::checkPassWord()
{
    m_key = dialogKey->text();
    command_sudo = new QProcess();
    connect(command_sudo,&QProcess::readyReadStandardOutput,this,&rootAuthDialog::readBashOutput);
    connect(command_sudo,&QProcess::readyReadStandardError,this,&rootAuthDialog::readBashOutput);
    command_sudo->start("bash");
    command_sudo->waitForStarted();
    QString str1 = "echo " + m_key + "| sudo -S -l ";
    command_sudo->write(str1.toLocal8Bit() + "\n");
}

void rootAuthDialog::readBashOutput()
{
    QByteArray cmdout = command_sudo->readAllStandardOutput();
    QByteArray cmderr = command_sudo->readAllStandardError();
    QString err = cmderr;
    QString str = cmdout;
    if(str.contains("secure_path"))
    {
        emit passwdCorrect();
        dialogKey->clear();
        dialogKey->setPlaceholderText(tr("please enter the password"));
        this->close();
        return ;
    }else if(err.contains("对不起") || err.contains("Sorry"))
    {
        dealWrongPasswd();
        return ;
    }else if(err.contains("sudoers"))
    {
        dealNotSudoers();
        return ;
    }
}

void rootAuthDialog::dealWrongPasswd()
{
    dialogKey->clear();
    dialogKey->setPlaceholderText(tr("Wrong password!Try again"));
    command_sudo->kill();
    command_sudo->waitForFinished(-1);
    dialogKey->setStyleSheet("QLineEdit{border:1px solid rgba(245, 108, 108, 1);font-size:14px;}");

}

void rootAuthDialog::dealNotSudoers()
{
    dialogKey->clear();
    dialogKey->setPlaceholderText(tr("Current user is not in the sudoers file,please change another account or change authority"));
    command_sudo->kill();
    command_sudo->waitForFinished(-1);
}


bool rootAuthDialog::event(QEvent *event){
    qDebug()<<event->type();
    return QWidget::event(event);
}

