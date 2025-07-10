qt_add_library(QDashQmlWidgets STATIC)

add_subdirectory(Base)
add_subdirectory(Misc)
add_subdirectory(Sendable)
add_subdirectory(Primitive)

target_link_libraries(QDashQmlWidgets
        PRIVATE
        QDashQmlWidgetsBaseplugin
        QDashQmlWidgetsMiscplugin
        QDashQmlWidgetsSendableplugin
        QDashQmlWidgetsPrimitiveplugin
)
