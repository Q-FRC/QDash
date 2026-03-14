# SPDX-FileCopyrightText: Copyright 2026 crueter
# SPDX-License-Identifier: GPL-3.0-or-later

# Config stuff
if(UNIX AND NOT(APPLE))
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/org.Q-FRC.QDash.desktop DESTINATION "share/applications")
    install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/org.Q-FRC.QDash.svg DESTINATION "share/icons/hicolor/scalable/apps")
    install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/org.Q-FRC.QDash-symbolic.svg DESTINATION "share/icons/hicolor/symbolic/apps" RENAME org.Q-FRC.QDash.svg)
endif()

string(REPLACE "-beta" "." QDASH_NUMERIC_TAG ${GIT_TAG})
string(REPLACE "-rc" "." QDASH_NUMERIC_TAG ${QDASH_NUMERIC_TAG})
string(REPLACE "." ";" tmp_list_tag ${QDASH_NUMERIC_TAG})
list(LENGTH tmp_list_tag length_tmp)
if (length_tmp LESS 4)
    set(QDASH_VERSION_WIN ${QDASH_NUMERIC_TAG}.0)
else()
    set(QDASH_VERSION_WIN ${QDASH_NUMERIC_TAG})
endif()

string(REPLACE "." "," QDASH_VERSION_COMMA "${QDASH_VERSION_WIN}")

configure_file(QDash.rc.in QDash.rc @ONLY)
configure_file(QDash.manifest.in QDash.manifest @ONLY)
configure_file(org.Q-FRC.QDash.desktop org.Q-FRC.QDash.desktop COPYONLY)
configure_file(QDash.ico QDash.ico COPYONLY)
configure_file(win_install.nsi.in win_install.nsi @ONLY)
# configure_file(QDash.icns QDash.icns COPYONLY)
