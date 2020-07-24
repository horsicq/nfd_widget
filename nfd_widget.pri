QT       += concurrent

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

FORMS += \
    $$PWD/nfd_widget.ui

HEADERS += \
    $$PWD/nfd_widget.h

SOURCES += \
    $$PWD/nfd_widget.cpp

!contains(XCONFIG, staticscan) {
    XCONFIG += staticscan
    include($$PWD/../staticscan/staticscan.pri)
}

!contains(XCONFIG, dialoginfo) {
    XCONFIG += dialoginfo
    include($$PWD/../FormatDialogs/dialoginfo.pri)
}
