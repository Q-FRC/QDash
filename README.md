![QFRCDashboard](QFRCDashboard-display.png "QFRCDashboard")
A reliable, high-performance, low-footprint dashboard for use with FRC.

## Lightweight
Dashboards don't have to be resource hogs. In fact, dashboards should be designed to take up as little resources as possible. Dashboards that use up resources like nobody's business will cause **packet loss** and **comms issues** when run on driver stations!

Because of this, QFRCDashboard has been specifically designed to use up as little resources as possible. Compared to WPILib's Shuffleboard (measured on a ThinkPad X220--percentages include all CPU cores):

| Metric            | Shuffleboard  | QFRCDashboard |
| ----------------- | ------------- | ------------- |
| Memory (Base)     | 530MB         | <200MB        |
| Memory (Heavy Use)| 600MB-1.2GB   | 200-250MB     |
| CPU (Base)        | 2-10%         | 0-1%          |
| CPU (Heavy Use)   | 10-30%        | 0-3%          |

Network usage has not been tested, but an eye test shows significantly greater network activity with Shuffleboard than QFRCDashboard.

QFRCDashboard excels with its lightweight performance thanks to many factors:
- The efficiency of C++ and Qt vs. Java and JavaFX
- No menu that subscribes to every topic at once
- Shared subscriptions between duplicate topics
- Widgets only update and repaint when they need to
- Minimal, deferred repaints

Thanks to these factors, QFRCDashboard is the best choice for a high-performance dashboard with no compromises.

## Download
Windows and Linux builds are available via GitHub Actions. Currently, all use WPILib 2024 beta 3. Release builds are available either through Actions or in the releases tab:

[![Release](https://github.com/binex-dsk/QFRCDashboard/actions/workflows/trigger_release.yml/badge.svg)](https://github.com/binex-dsk/QFRCDashboard/actions/workflows/trigger_release.yml)

Development/debug builds are also available on a per-commit basis. These builds are generally unstable and also have far larger executable sizes, but are useful for users who wish to have the latest features or for developers who want to contribute and test.

[![Debug](https://github.com/binex-dsk/QFRCDashboard/actions/workflows/trigger_builds.yml/badge.svg)](https://github.com/binex-dsk/QFRCDashboard/actions/workflows/trigger_builds.yml)

## Usage Guide
Set your NT server URL with the menubar option, create a new tab in the Tab menu, and add a widget using the New Widget menu option. Save and load layouts with the File menu, and resize and edit widgets as desired by right-clicking and selecting Resize, Delete, etc, or resize and drag with the mouse.

(Note: row and column numbers are zero-indexed. Row and column spans, however, start from 1.)

## WIP
QFRCDashboard is not complete. There are tons of new features, bugfixes, that will be added in up-and-coming releases. In its current state, QFRCDashboard is *completely usable for the average team*.

## Forking
Follow the [GPL3](LICENSE) of this project, credit the original project, and make it clear that your application is not QFRCDashboard itself. Cool? Cool.

## Contributing
You are allowed to contribute as you please. However, if you constantly spam useless/troll issues or pull requests, you'll probably get flagged by GitHub. Maybe don't do that :>

If you're at all confused with the code structure, make an issue about it and I will help you.

## Building
This project uses CMake. Additionally, you must initialize git submodules first.

```bash
git submodule update --init
cmake -S . -B build
cd build
make -j$(nproc)
```

You can use CMake's install commands to install for packaging and system installs.

```bash
sudo cmake --install build --prefix /usr
cmake --install build --prefix ${PKGDIR}
```

### Submodules
If you don't want to install protobuf and ntcore to your system, CMake options are provided for such:

- `USE_SYSTEM_NTCORE` (default ON)
- `USE_SYSTEM_PROTOBUF` (default ON)

Examples:

```bash
cmake -S . -B build -DUSE_SYSTEM_NTCORE=OFF
cmake -S . -B build -DUSE_SYSTEM_NTCORE=OFF -DUSE_SYSTEM_PROTOBUF=OFF
```

Though you can use the system ntcore with the bundled protobuf, this is not recommended and is entirely untested.

### Ubuntu
```bash
sudo apt install qt6-base-dev build-essential qt6-multimedia-dev ninja-build
git clone https://github.com/binex-dsk/QFRCDashboard.git
cd QFRCDashboard
mkdir build && cd build
cmake ..
make -j
```

### Windows
- Install Qt from [here](https://www.qt.io/download-qt-installer-oss). Take note of where you download it!
- Install CMake https://cmake.org/download/ (add to `PATH`)
```bash
git clone https://github.com/binex-dsk/QFRCDashboard.git
cd QFRCDashboard
mkdir build && cd build
cmake -DCMAKE_PREFIX_PATH="C:\\Qt6\\6.6.1\\msvc2019_64" ..
cmake --build .
C:\Qt6\6.6.1\msvc2019_64\bin\windeployqt.exe .
```
OR use [CLion](https://www.jetbrains.com/clion/)

## Miscellaneous Notes
QFRCDashboard is created solely for the purpose of A RELIABLE, LOW-OVERHEAD FRC dashboard. QFRCDashboard is NOT created with eye-candy or intense theming in mind. Contributions and suggestions to theming will be accepted and worked on, but without user request or contribution, QFRCDashboard will see very little updates to theming.
