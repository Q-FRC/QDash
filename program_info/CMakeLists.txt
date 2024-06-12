# Config stuff
if(UNIX AND NOT(APPLE))
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${Dashboard_APP_NAME}.desktop DESTINATION "share/applications")
    install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/${Dashboard_APP_NAME}.svg DESTINATION "share/icons/hicolor/scalable/apps")
    install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/${Dashboard_APP_NAME}-symbolic.svg DESTINATION "share/icons/hicolor/symbolic/apps" RENAME ${Dashboard_APP_NAME}.svg)
endif()

configure_file(${Dashboard_APP_NAME}.rc.in ${Dashboard_APP_NAME}.rc)
configure_file(${Dashboard_APP_NAME}.manifest.in ${Dashboard_APP_NAME}.manifest)
configure_file(${Dashboard_APP_NAME}.desktop.in ${Dashboard_APP_NAME}.desktop)
configure_file(${Dashboard_APP_NAME}.ico ${Dashboard_APP_NAME}.ico COPYONLY)
