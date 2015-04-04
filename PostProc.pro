#-------------------------------------------------
#
# Project created by QtCreator 2015-04-01T12:03:13
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PostProc
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mesh_element.cc \
    mesh_file.cc \
    mesh.cc \
    meshplot.cpp

HEADERS  += mainwindow.h \
    mesh_element.h \
    mesh_file.h \
    mesh.h \
    meshplot.h
