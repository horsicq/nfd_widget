QT       += concurrent

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

FORMS += \
    $$PWD/nfd_widget.ui

HEADERS += \
    $$PWD/nfd_widget.h

SOURCES += \
    $$PWD/nfd_widget.cpp

!contains(XCONFIG, die_script) {
    XCONFIG += die_script
    include($$PWD/../die_script/die_script.pri)
}
