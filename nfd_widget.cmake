include_directories(${CMAKE_CURRENT_LIST_DIR})

include(${CMAKE_CURRENT_LIST_DIR}/../FormatDialogs/dialogtextinfo.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../FormatDialogs/xdialogprocess.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../XShortcuts/xshortcuts.cmake)

set(NFD_WIDGET_SOURCES
    ${DIALOGTEXTINFO_SOURCES}
    ${XDIALOGPROCESS_SOURCES}
    ${XSHORTCUTS_SOURCES}
    ${CMAKE_CURRENT_LIST_DIR}/dialognfdscan.cpp
    ${CMAKE_CURRENT_LIST_DIR}/dialognfdscan.ui
    ${CMAKE_CURRENT_LIST_DIR}/dialognfdscandirectory.cpp
    ${CMAKE_CURRENT_LIST_DIR}/dialognfdscandirectory.ui
    ${CMAKE_CURRENT_LIST_DIR}/dialognfdscanprocess.cpp
    ${CMAKE_CURRENT_LIST_DIR}/nfdwidgetadvanced.cpp
    ${CMAKE_CURRENT_LIST_DIR}/nfdwidgetadvanced.ui
    ${CMAKE_CURRENT_LIST_DIR}/nfd_widget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/nfd_widget.ui
    ${CMAKE_CURRENT_LIST_DIR}/nfdoptionswidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/nfdoptionswidget.ui
    ${CMAKE_CURRENT_LIST_DIR}/dialognfdwidgetadvanced.cpp
    ${CMAKE_CURRENT_LIST_DIR}/dialognfdwidgetadvanced.ui
)
