QT       += core gui dbus network x11extras KWindowSystem

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

#添加子工程
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/dbusadaptor.cpp \
    src/include/menumodule.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/page1.cpp \
    src/page2.cpp \
    src/include/qtlocalpeer.cpp \
    src/include/qtlockedfile.cpp \
    src/include/qtlockedfile_unix.cpp \
    src/include/qtsingleapplication.cpp \
    src/include/xatom-helper.cpp \
    src/include/daemonipcdbus.cpp \
    src/stylecombobox.cpp \
    src/rootauthdialog.cpp \
    src/toolwindow.cpp

HEADERS += \
    src/dbusadaptor.h \
    src/include/daemonipcdbus.h \
    src/include/menumodule.h \
    src/mainwindow.h \
    src/page1.h \
    src/page2.h \
    src/include/qtlocalpeer.h \
    src/include/qtlockedfile.h \
    src/include/qtsingleapplication.h \
    src/include/xatom-helper.h \
    src/stylecombobox.h \
    src/rootauthdialog.h \
    src/toolwindow.h

LIBS +=-lpthread
LIBS +=-lX11

# bin file output dir
TARGET = kylin-usb-creator
target.source += $$TARGET
target.path = /usr/bin

icons.files = data/kylin-usb-creator.png
icons.path = /usr/share/pixmaps/

# gsettings
schemes.files = data/org.kylin-usb-creator-data.gschema.xml
schemes.path = /usr/share/glib-2.0/schemas/

desktop.path = /usr/share/applications
desktop.files = kylin-usb-creator.desktop

INSTALLS += target desktop icons schemes
CONFIG += link_pkgconfig

PKGCONFIG += gsettings-qt
# translations
#qm_files.files = src/translations/*.pm
#qm_files.path = /usr/share/kylin
TRANSLATIONS += src/translations/kylin-usb-creator_zh_CN.ts \
                src/translations/kylin-usb-creator_bo_CN.ts

# Default rules for deployment.
#qnx: target.path = /tmp/$${TARGET}/bin
#else: unix:!android: target.path = /opt/$${TARGET}/bin
#!isEmpty(target.path): INSTALLS += target

RESOURCES += data.qrc translations.qrc

QMAKE_CXXFLAGS +=  -Wno-unused-parameter

DISTFILES += src/translations/qt_zh_CN.qm \
    src/translations/kylin-usb-creator_bo_CN.qm \
    src/translations/kylin-usb-creator_zh_CN.qm
