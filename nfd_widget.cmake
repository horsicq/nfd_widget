include_directories(${CMAKE_CURRENT_LIST_DIR})

if (NOT DEFINED XDIALOGPROCESS_SOURCES)
    include(${CMAKE_CURRENT_LIST_DIR}/../FormatDialogs/xdialogprocess.cmake)
    set(NFD_WIDGET_SOURCES ${NFD_WIDGET_SOURCES} ${XDIALOGPROCESS_SOURCES})
endif()
if (NOT DEFINED DIALOGTEXTINFO_SOURCES)
    include(${CMAKE_CURRENT_LIST_DIR}/../FormatDialogs/dialogtextinfo.cmake)
    set(NFD_WIDGET_SOURCES ${NFD_WIDGET_SOURCES} ${DIALOGTEXTINFO_SOURCES})
endif()
if (NOT DEFINED XDIALOGPROCESS_SOURCES)
    include(${CMAKE_CURRENT_LIST_DIR}/../FormatDialogs/xdialogprocess.cmake)
    set(NFD_WIDGET_SOURCES ${NFD_WIDGET_SOURCES} ${XDIALOGPROCESS_SOURCES})
endif()

include(${CMAKE_CURRENT_LIST_DIR}/../XShortcuts/xshortcuts.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../SpecAbstract/specabstract.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../Controls/xcomboboxex.cmake)

set(NFD_WIDGET_SOURCES
    ${NFD_WIDGET_SOURCES}
    ${XDIALOGPROCESS_SOURCES}
    ${DIALOGTEXTINFO_SOURCES}
    ${XSHORTCUTS_SOURCES}
    ${SPECABSTRACT_SOURCES}
    ${XCOMBOBOXEX_SOURCES}
    ${CMAKE_CURRENT_LIST_DIR}/dialognfdscan.cpp
    ${CMAKE_CURRENT_LIST_DIR}/dialognfdscan.h
    ${CMAKE_CURRENT_LIST_DIR}/dialognfdscan.ui
    ${CMAKE_CURRENT_LIST_DIR}/dialognfdscandirectory.cpp
    ${CMAKE_CURRENT_LIST_DIR}/dialognfdscandirectory.h
    ${CMAKE_CURRENT_LIST_DIR}/dialognfdscandirectory.ui
    ${CMAKE_CURRENT_LIST_DIR}/dialognfdscanprocess.cpp
    ${CMAKE_CURRENT_LIST_DIR}/dialognfdscanprocess.h
    ${CMAKE_CURRENT_LIST_DIR}/dialognfdwidgetadvanced.cpp
    ${CMAKE_CURRENT_LIST_DIR}/dialognfdwidgetadvanced.h
    ${CMAKE_CURRENT_LIST_DIR}/dialognfdwidgetadvanced.ui
    ${CMAKE_CURRENT_LIST_DIR}/nfd_widget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/nfd_widget.h
    ${CMAKE_CURRENT_LIST_DIR}/nfd_widget.ui
    ${CMAKE_CURRENT_LIST_DIR}/nfdoptionswidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/nfdoptionswidget.h
    ${CMAKE_CURRENT_LIST_DIR}/nfdoptionswidget.ui
    ${CMAKE_CURRENT_LIST_DIR}/nfdwidgetadvanced.cpp
    ${CMAKE_CURRENT_LIST_DIR}/nfdwidgetadvanced.h
    ${CMAKE_CURRENT_LIST_DIR}/nfdwidgetadvanced.ui
)
