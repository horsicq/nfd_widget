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

!contains(XCONFIG, dialogtextinfo) {
    XCONFIG += dialogtextinfo
    include($$PWD/../FormatDialogs/dialogtextinfo.pri)
}
