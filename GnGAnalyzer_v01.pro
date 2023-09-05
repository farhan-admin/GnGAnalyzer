TEMPLATE = app
TARGET = GnGAnalyzer_v0.1
UI_DIR = $$PWD  #This line solves the problem of UI not getting updated, dicussed in link https://stackoverflow.com/questions/4382652/qt-designer-does-not-update-the-gui
DESTDIR = ./debug
QT += core gui widgets
CONFIG += debug
DEFINES += _WINDOWS WIN64 QT_WIDGETS_LIB

FORMS += \
    cfilesel.ui \
    chdrdisplay.ui \
    csegydlg.ui \
    cselsorthdrs.ui \
    infodialog.ui \
    mainwindow.ui

HEADERS += \
    AllEnums.h \
    cColorBar.h \
    cColorMap.h \
    cGLWindow.h \
    cLocXYZ.h \
    cSegYReadWrite.h \
    cSegYTempla.h \
    cSeisData.h \
    cSeisGather.h \
    cSeisHdr.h \
    cSeisHdrTableView.h \
    cbinhdr.h \
    cerrwarninfo.h \
    csegyFunc.h \
    csegydlg.h \
    csortorder.h \
    infodialog.h \
    mainwindow.h \
    ui_csegydlg.h \
    ui_cselsorthdrs.h \
    ui_mainwindow.h

SOURCES += \
    cGLWindow.cpp \
    cLocXYZ.cpp \
    cSegYReadWrite.cpp \
    cSegYTempla.cpp \
    cSeisData.cpp \
    cSeisHdrTableView.cpp \
    cerrwarninfo.cpp \
    csegyFunc.cpp \
    csegydlg.cpp \
    infodialog.cpp \
    main.cpp \
    mainwindow.cpp
