QT       += concurrent

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    $$PWD/dialognfdscan.h \
    $$PWD/dialognfdscandirectory.h \
    $$PWD/dialognfdscanprocess.h \
    $$PWD/dialognfdwidgetadvanced.h \
    $$PWD/nfd_widget.h \
    $$PWD/nfdoptionswidget.h \
    $$PWD/nfdwidgetadvanced.h

SOURCES += \
    $$PWD/dialognfdscan.cpp \
    $$PWD/dialognfdscandirectory.cpp \
    $$PWD/dialognfdscanprocess.cpp \
    $$PWD/dialognfdwidgetadvanced.cpp \
    $$PWD/nfd_widget.cpp \
    $$PWD/nfdoptionswidget.cpp \
    $$PWD/nfdwidgetadvanced.cpp

FORMS += \
    $$PWD/dialognfdscan.ui \
    $$PWD/dialognfdscandirectory.ui \
    $$PWD/dialognfdwidgetadvanced.ui \
    $$PWD/nfd_widget.ui \
    $$PWD/nfdoptionswidget.ui \
    $$PWD/nfdwidgetadvanced.ui

!contains(XCONFIG, staticscan) {
    XCONFIG += staticscan
    include($$PWD/../SpecAbstract/staticscan.pri)
}

!contains(XCONFIG, xshortcuts) {
    XCONFIG += xshortcuts
    include($$PWD/../XShortcuts/xshortcuts.pri)
}

!contains(XCONFIG, dialogtextinfo) {
    XCONFIG += dialogtextinfo
    include($$PWD/../FormatDialogs/dialogtextinfo.pri)
}

!contains(XCONFIG, xdialogprocess) {
    XCONFIG += xdialogprocess
    include($$PWD/../FormatDialogs/xdialogprocess.pri)
}

DISTFILES += \
    $$PWD/LICENSE \
    $$PWD/README.md \
    $$PWD/nfd_widget.cmake
