QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/databasehandle.cpp \
    src/dbviewdialog.cpp \
    src/mainwindow.cpp \
    src/setdialog.cpp \
    main.cpp

HEADERS += \
    inc/databasehandle.h \
    inc/mainwindow.h \
    inc/setdialog.h \
    inc/dbviewdialog.h

FORMS += \
    uidesign/dbviewdialog.ui \
    uidesign/setdialog.ui \
    uidesign/mainwindow.ui

QT += sql
QT += serialport

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
