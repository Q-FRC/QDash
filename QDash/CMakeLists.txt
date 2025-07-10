set(QRC_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/fields.qrc
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/accents.qrc
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/icons.qrc
)

add_subdirectory(Config)
add_subdirectory(Constants)
add_subdirectory(Dialogs)
add_subdirectory(Fields)
add_subdirectory(Items)
add_subdirectory(Widgets)
add_subdirectory(Main)

set(BASE_PLUGINS
    QDashQmlConstants
    QDashQmlConfig
    QDashQmlDialogs
    QDashQmlFields
    QDashQmlItems
    QDashQmlMain
)

foreach(PLUGIN ${BASE_PLUGINS})
    set(QML_PLUGINS ${QML_PLUGINS} ${PLUGIN}plugin ${PLUGIN})
endforeach()

qt_add_library(QDashQml STATIC)

target_link_libraries(QDashQml
    PRIVATE
    BuildConfig

    ${QDASH_QT_LIBRARIES}
    ${QML_PLUGINS}
    QDashQmlWidgets
)

add_subdirectory(Native)
