cd build
mv src/native/QDash.app .
chmod a+x QDash.app/Contents/MacOS/QDash
mkdir QDash.app/Contents/Resources -p
cp ../dist/QDash.icns QDash.app/Contents/Resources

macdeployqt QDash.app \
            -qmldir ../src/qml \
            -qmlimport=$QML_SOURCES_PATHS \
            -verbose=1 \
            -appstore-compliant

tar czf ../QDash.tar.gz QDash.app
