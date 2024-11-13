cmake_minimum_required(VERSION 3.16)

project(09_Custom_Component_In_Main VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.4 REQUIRED COMPONENTS Quick)

qt_standard_project_setup()

qt_add_executable(09_Custom_Component_In_Main
    main.cpp
    qml.qrc
)

qt_add_qml_module(09_Custom_Component_In_Main
    URI Custom_Component_In_Main
    VERSION 1.0
    QML_FILES main.qml
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(09_Custom_Component_In_Main PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.app2-ComponentsInPlace
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(09_Custom_Component_In_Main
    PRIVATE Qt6::Quick
)

include(GNUInstallDirs)
install(TARGETS 09_Custom_Component_In_Main
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
target_compile_definitions(09_Custom_Component_In_Main
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(09_Custom_Component_In_Main
  PRIVATE Qt6::Core Qt6::Quick)
