#-------------------------------------------------
#
# Project created by QtCreator 2016-09-19T09:16:42
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = chess
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    board.cpp \
    mytextedit.cpp \
    mypushbutton.cpp \
    loginwigdet.cpp \
    myimage.cpp

HEADERS  += widget.h \
    board.h \
    mytextedit.h \
    mypushbutton.h \
    loginwigdet.h \
    myimage.h

RESOURCES += \
    image.qrc
RC_FILE = myapp.rc
