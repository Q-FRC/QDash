#/bin/bash

# ICO

inkscape -w 16 -h 16 -o QFRCDashboard_16.png QFRCDashboard.svg
inkscape -w 24 -h 24 -o QFRCDashboard_24.png QFRCDashboard.svg
inkscape -w 32 -h 32 -o QFRCDashboard_32.png QFRCDashboard.svg
inkscape -w 48 -h 48 -o QFRCDashboard_48.png QFRCDashboard.svg
inkscape -w 64 -h 64 -o QFRCDashboard_64.png QFRCDashboard.svg
inkscape -w 128 -h 128 -o QFRCDashboard_128.png QFRCDashboard.svg

convert QFRCDashboard_128.png QFRCDashboard_64.png QFRCDashboard_48.png QFRCDashboard_32.png QFRCDashboard_24.png QFRCDashboard_16.png QFRCDashboard.ico

rm -f QFRCDashboard_*.png
