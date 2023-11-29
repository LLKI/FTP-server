QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++20
DEFINES += NOLFS
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
#INCLUDEPATH += $$PWD/openssl/include

SOURCES += \
    ftp.cpp \
    ftpconnectdlg.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    ftp.h \
    ftpconnectdlg.h \
    mainwindow.h

FORMS += \
    ftpconnectdlg.ui \
    mainwindow.ui


CONFIG += lrelease
CONFIG += embed_translations


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target



win32:CONFIG(release, debug|release): LIBS += -L$$PWD/ftplib/lib/ -lftplib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/ftplib/lib/ -lftplibd

INCLUDEPATH += $$PWD/ftplib/include
DEPENDPATH += $$PWD/ftplib/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/ftplib/lib/libftplib.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/ftplib/lib/libftplibd.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/ftplib/lib/ftplib.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/ftplib/lib/ftplibd.lib


DISTFILES += \
    ftplibd.dll
