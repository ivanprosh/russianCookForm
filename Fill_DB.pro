#-------------------------------------------------
#
# Project created by QtCreator 2016-02-08T13:00:50
#
#-------------------------------------------------

QT       += core gui sql widgets

#greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Fill_DB
TEMPLATE = app

#LIBS+= Qt5PlatformSupport.lib

SOURCES += main.cpp\
        sqlform.cpp \
    helpdelegate.cpp \
    mytablewidget.cpp \
    receptimage.cpp

HEADERS  += sqlform.h \
    linecontainer.h \
    helpdelegate.h \
    mytablewidget.h \
    u_error.h \
    receptimage.h \
    global.h

OTHER_FILES +=

RESOURCES += \
    res.qrc
