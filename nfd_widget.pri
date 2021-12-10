QT       += concurrent

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

FORMS += \
    $$PWD/nfd_widget.ui

HEADERS += \
    $$PWD/nfd_widget.h

SOURCES += \
    $$PWD/nfd_widget.cpp

!contains(XCONFIG, formresult) {
    XCONFIG += formresult
    include($$PWD/../StaticScan/formresult.pri)
}

!contains(XCONFIG, dialogtextinfo) {
    XCONFIG += dialogtextinfo
    include($$PWD/../FormatDialogs/dialogtextinfo.pri)
}

DISTFILES += \
    $$PWD/LICENSE \
    $$PWD/README.md \
    $$PWD/nfd_widget.cmake
