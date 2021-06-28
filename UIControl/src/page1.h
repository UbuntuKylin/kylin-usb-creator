#ifndef PAGE1_H
#define PAGE1_H

#define NOUDISK "noUDisk"
#define UDISK_NAME_MAX_LENGTH 27//U盘名称最大长度
#define COMBOBOXW 370 //下拉框宽度
#define COMBOBOXH 30//下拉框高度
#define ITEMHEIGHT 30//下拉框标签高度
#define COMBOBOXRADIUS 6//下拉框窗口圆角
#define DARKTHEME "dark"
#define LIGHTTHEME "light"

#include <QEvent>
#include <QDebug>
#include <QRect>
#include <QComboBox>
#include <QWidget> //控件
#include <QLabel>
#include <QProcess>
#include <QComboBox>
#include <QMessageBox>
#include <QLineEdit>
#include <QDBusMessage>
#include <QDBusConnection>
#include <QPushButton>
#include <QTimer>
#include <QBoxLayout> //布局
#include <QFileDialog>//打开文件
#include <QStorageInfo>//硬盘信息
#include <QFileSystemWatcher>
#include <QGraphicsDropShadowEffect>
#include <QScreen>
#include <QCoreApplication>
#include <QTextCodec>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonArray>
#include <QApplication>
class AvailableDiskInfo
{
public:
    AvailableDiskInfo(QString path = "unknowpath",QString name = "unknowmodel",QString capacity = "unknowcapacity"):devicePath(path),displayName(name),diskCapicity(capacity){}
public:
    QString devicePath;   //设备路径ex:/dev/sdb
    QString displayName;
    QString diskCapicity;
//    TODO:分区数获取，按照分区全部卸载。（目前只卸载了第一个分区然后就写入
    qint8 partsNums = 0;    //为0时视作没有分区，直接向块设备写入
};

class Page1 : public QWidget
{
    Q_OBJECT
public:
    explicit Page1();
    bool ifStartBtnChange(); //开始制作按钮是否可以点击
    void setThemeStyleLight(); //设置浅色主题
    void setThemeStyleDark(); //设置深色主题
    QLineEdit *urlIso = nullptr;//显示镜像路径
    QComboBox *comboUdisk = nullptr;//U盘列表

signals:
    void makeStart(QString sourcePath,QString targetPath); //make start
    void diskLabelRefresh(); //  已选中U盘刷新
    void setStyleWidgetStyle(const QString);
    void isoIllegal();

public slots:
    void refreshDiskList();
    void dealRightPasswd(); //处理密码正确
    void dealAuthDialogClose();  //处理授权框关闭
    void dealComboBoxChangeButton(); //combobox通知page1检查开始按钮是否可以亮起

private:
    void creatStartSlots();    //开始制作
    void initControlQss();//初始化控件及其样式
    void getStorageInfo();//获取磁盘信息
    bool mouseIsLeaveUdiskWidget();//鼠标是否离开U盘列表
    void dealDialogCancel();     // 处理授权框关闭及取消
    void udiskPlugWatcherInit(); //U盘插拔监控初始化
    bool isCapicityAvailable(QString); //容量过滤
    void getUdiskPathAndCap();    //获取U盘路径和容量
    void getUdiskName();    //获取U盘第一个分区的命名
    bool checkISO(const QString fileName); //ISO合法性检验
    QJsonArray QStringToJsonArray(const QString jsonString);

    QList<AvailableDiskInfo*> diskInfos; // U盘信息
    QString themeStatus = LIGHTTHEME; //主题指示器
    QTimer *diskRefreshDelay = nullptr; //U盘插入后等待系统挂载的时间
    QLabel *tabIso = nullptr;//选择镜像标签
    QLabel *tabUdisk = nullptr;//选择U盘标签
    QLabel *warnningIcon = nullptr;//警告标志
    QLabel *warnningText = nullptr;//警告标语tabUdisk
    QPushButton *findIso = nullptr;//浏览文件按钮
    QPushButton *creatStart = nullptr;//开始制作
    QFileSystemWatcher *udiskplugwatcher = nullptr; //U盘插拔监控器
    QPushButton *dialogNo = nullptr;
    QPushButton *dialogYes = nullptr;
    QString isoPath;    //iso文件的路径
    bool paintOnce=false;//只绘制一次
    bool leaveThis=true;

};


#endif // PAGE1_H
