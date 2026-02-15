cmake_minimum_required(VERSION 3.19)
project(Quantite LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Network)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

qt_standard_project_setup()

set(CMAKE_AUTOUIC_SEARCH_PATHS
    ${CMAKE_CURRENT_SOURCE_DIR}/UI
)

qt_add_executable(Quantite
    WIN32 MACOSX_BUNDLE

    Source/main.cpp
    Source/mainwindow.cpp
    Source/Quantite.cpp
    Source/DataBreakInfoModel.cpp
    Source/QuantiteViewModel.cpp
    
    Include/mainwindow.h
    Include/Quantite.hpp 
    Include/DataBreakInfoModel.hpp 
    Include/QuantiteViewModel.hpp

    UI/mainwindow.ui
)

target_include_directories(Quantite
    PRIVATE
        D:/msys64/mingw64/User/Include
        ${CMAKE_CURRENT_SOURCE_DIR}/Include
)

target_link_directories(Quantite
    PRIVATE
        D:/msys64/mingw64/User/Lib
)

target_link_libraries(Quantite
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::Network
        IO
        Network
)

include(GNUInstallDirs)

install(TARGETS Quantite
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET Quantite
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
