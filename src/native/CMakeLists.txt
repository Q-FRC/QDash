qt_add_executable(${QDASH_NAME}
    main.cpp
    ${QRC_FILES}
    ${RCS_FILE}
)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}")
include_directories("$<TARGET_PROPERTY:ntcore,INTERFACE_INCLUDE_DIRECTORIES>")
add_compile_definitions("${QDASH_DEFINES}")

add_subdirectory(buildconfig)
add_subdirectory(helpers)
add_subdirectory(logging)
add_subdirectory(managers)
add_subdirectory(misc)
add_subdirectory(models)
add_subdirectory(nt)

set(BASE_PLUGINS
    QDashHelpers
    QDashLogging
    QDashManagers
    QDashMisc
    QDashModels
    QDashNt
)

foreach(PLUGIN ${BASE_PLUGINS})
    set(PLUGINS ${PLUGINS} ${PLUGIN}plugin ${PLUGIN})
endforeach()

target_link_libraries(${QDASH_NAME}
    PRIVATE
    BuildConfig
    ntcore

    ${QDASH_QT_LIBRARIES}
    ${PLUGINS}
    QDashQml
)

set_target_properties(${QDASH_NAME} PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
)

include(GNUInstallDirs)
install(TARGETS ${QDASH_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

set_target_properties(${QDASH_NAME} PROPERTIES
    MACOSX_BUNDLE TRUE
)

if(UNIX AND APPLE)
    # Mac bundle settings
    set(MACOSX_BUNDLE_BUNDLE_NAME "${QDASH_NAME}")
    set(MACOSX_BUNDLE_INFO_STRING "${QDASH_NAME}: A high-performance, reliable dashboard for FRC use.")
    set(MACOSX_BUNDLE_GUI_IDENTIFIER "org.${QDASH_ORG}.${QDASH_NAME}")
    set(MACOSX_BUNDLE_BUNDLE_VERSION "${QDASH_NUMERIC_TAG}")
    set(MACOSX_BUNDLE_SHORT_VERSION_STRING "${QDASH_NUMERIC_TAG}")
    set(MACOSX_BUNDLE_LONG_VERSION_STRING "${QDASH_NUMERIC_TAG}")
    set(MACOSX_BUNDLE_ICON_FILE ${QDASH_NAME}.icns)
    set(MACOSX_BUNDLE_COPYRIGHT "Copyleft 2023-2024 ${QDASH_ORG}")

    # install as bundle
    set(INSTALL_BUNDLE "full")
endif()
