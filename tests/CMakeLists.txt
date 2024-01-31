cmake_minimum_required(VERSION 3.5)

enable_testing()

project(Tests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

macro(AddTest _name _src)
    add_executable(${_name} ${_src})

    target_link_libraries(${_name} PRIVATE QFRCDashboardLib Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::MultimediaWidgets Qt${QT_VERSION_MAJOR}::Charts ntcore BuildConfig Qt${QT_VERSION_MAJOR}::Test)

    target_include_directories(${_name} PRIVATE "$<TARGET_PROPERTY:ntcore,INTERFACE_INCLUDE_DIRECTORIES>")
    target_include_directories(${_name} PRIVATE ../lib/ui/)
    target_include_directories(${_name} PRIVATE ../lib/include)
    target_include_directories(${_name} PRIVATE ${_Dashboard_AUTOGEN}/include)
    target_include_directories(${_name} PRIVATE ../lib/)

    add_test(NAME ${_name} COMMAND ${_name})

    # include(GNUInstallDirs)
    # install(TARGETS ${_name}
    #     BUNDLE DESTINATION .
    #     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    #     RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    # )

    configure_file(test.json test.json COPYONLY)

endmacro()

AddTest(SaveLoad tst_saveload.cpp)
AddTest(WidgetDeletion tst_delete.cpp)
AddTest(WidgetUpdate tst_update.cpp)
