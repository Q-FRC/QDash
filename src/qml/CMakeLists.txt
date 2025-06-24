add_subdirectory(basewidget)
add_subdirectory(config)
add_subdirectory(constants)
add_subdirectory(dialogs)
add_subdirectory(fields)
add_subdirectory(items)
add_subdirectory(widgets)
add_subdirectory(main)

set(BASE_PLUGINS
    QDashQmlBaseWidget
    QDashQmlConstants
    QDashQmlConfig
    QDashQmlDialogs
    QDashQmlFields
    QDashQmlItems
    QDashQmlWidgets
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
)
