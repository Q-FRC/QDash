#/bin/bash

# ICO

inkscape -w 16 -h 16 -o QDash_16.png QDash.svg
inkscape -w 24 -h 24 -o QDash_24.png QDash.svg
inkscape -w 32 -h 32 -o QDash_32.png QDash.svg
inkscape -w 48 -h 48 -o QDash_48.png QDash.svg
inkscape -w 64 -h 64 -o QDash_64.png QDash.svg
inkscape -w 128 -h 128 -o QDash_128.png QDash.svg

convert QDash_128.png QDash_64.png QDash_48.png QDash_32.png QDash_24.png QDash_16.png QDash.ico

rm -f QDash_*.png

inkscape -w 1024 -h 1024 -o QDash_1024.png QDash.svg

png2icns QDash.icns QDash_1024.png

rm -f QDash_*.png
