# Config stuff
if(UNIX AND NOT(APPLE))
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${QDASH_NAME}.desktop DESTINATION "share/applications")
    install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/${QDASH_NAME}.svg DESTINATION "share/icons/hicolor/scalable/apps")
    install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/${QDASH_NAME}-symbolic.svg DESTINATION "share/icons/hicolor/symbolic/apps" RENAME ${QDASH_NAME}.svg)
endif()

set(QDASH_VERSION_WIN ${QDASH_VERSION_MAJOR}.${QDASH_VERSION_MINOR}.${QDASH_VERSION_HOTFIX}.0)

configure_file(${QDASH_NAME}.rc.in ${QDASH_NAME}.rc @ONLY)
configure_file(${QDASH_NAME}.manifest.in ${QDASH_NAME}.manifest @ONLY)
configure_file(${QDASH_NAME}.desktop.in ${QDASH_NAME}.desktop)
configure_file(${QDASH_NAME}.ico ${QDASH_NAME}.ico COPYONLY)
configure_file(win_install.nsi.in win_install.nsi @ONLY)
configure_file(${QDASH_NAME}.icns ${QDASH_NAME}.icns COPYONLY)
