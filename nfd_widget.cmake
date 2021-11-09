include_directories(${CMAKE_CURRENT_LIST_DIR})

include(${CMAKE_CURRENT_LIST_DIR}/../StaticScan/formresult.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../FormatDialogs/dialogtextinfo.cmake)

set(NFD_WIDGET_SOURCES
    ${FORMRESULT_SOURCES}
    ${DIALOGTEXTINFO_SOURCES}
    ${CMAKE_CURRENT_LIST_DIR}/nfd_widget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/nfd_widget.ui
)
