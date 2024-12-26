![QFRCDashboard](QFRCDashboard-display.png "QFRCDashboard")
A reliable, high-performance, low-footprint dashboard for use with FRC.

A mirror of this repository is available on my [git](https://git.swurl.xyz/Q-FRC/Dashboard.git) instance.

## Lightweight
Dashboards don't have to be resource hogs. In fact, dashboards should be designed to take up as few resources as possible. Dashboards that use up resources like nobody's business will cause **packet loss** and **comms issues** when run on driver stations!

Because of this, QFRCDashboard has been specifically designed to use up as few resources as possible. Compared to WPILib's Shuffleboard (measured on a ThinkPad X220--percentages include all CPU cores):

| Metric            | Shuffleboard  | QFRCDashboard |
| ----------------- | ------------- | ------------- |
| Memory (Base)     | 530MB         | 100MB         |
| Memory (Heavy Use)| 600MB-1.2GB   | 150MB         |
| CPU (Base)        | 2-10%         | 0-1%          |
| CPU (Heavy Use)   | 10-30%        | 0-2%          |

Network activity hasn't been specifically measured, but expect better network performance out of QFRCDashboard. (Note that Shuffleboard's numbers may vary. Sometimes I've seen it eat the entire CPU on computers 5x as powerful as mine.)

QFRCDashboard excels with its lightweight performance thanks to many factors:
- Qt & QML's high efficiency
- No menu that subscribes to every topic at once
- Shared subscriptions between duplicate topics
- Widgets only update and repaint when they need to

## Download
Windows, Linux, and macOS builds are available via GitHub Actions. Currently, all use WPILib 2024.3.1. Release builds are available either through Actions or in the releases tab:

[![Release](https://github.com/Q-FRC/Dashboard/actions/workflows/trigger_release.yml/badge.svg)](https://github.com/Q-FRC/Dashboard/actions/workflows/trigger_release.yml)

Development/debug builds are also available on a per-commit basis. These builds are generally unstable, have far larger executable sizes, and will perform slower, but are useful for users who wish to have the latest features or for developers who want to contribute and test.

[![Debug](https://github.com/Q-FRC/Dashboard/actions/workflows/trigger_builds.yml/badge.svg)](https://github.com/Q-FRC/Dashboard/actions/workflows/trigger_builds.yml)

## Forking
Follow the [GPL3](LICENSE) of this project, credit the original project, and make it clear that your application is not QFRCDashboard itself.

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

- `USE_SYSTEM_NTCORE` (default OFF)
- `USE_SYSTEM_PROTOBUF` (default OFF)

Examples:

```bash
cmake -S . -B build -DUSE_SYSTEM_NTCORE=ON
cmake -S . -B build -DUSE_SYSTEM_NTCORE=ON -DUSE_SYSTEM_PROTOBUF=ON
```

Using the system ntcore with the bundled protobuf is unnecessary and will break things. If you have ntcore installed to the system, protobuf will also be installed.

### Linux
```bash
# or whatever your distribution uses
sudo apt install qt6-base-dev build-essential qt6-multimedia-dev ninja-build
git clone https://github.com/Q-FRC/Dashboard.git
cd QFRCDashboard
mkdir build && cd build
cmake ..
make -j$(nproc)
```

### Windows
- Install Qt from [here](https://www.qt.io/download-qt-installer-oss). Take note of where you download it!
  * Note that you will need to create a Qt account.
- Install CMake https://cmake.org/download/ (add to `PATH`)
```bash
git clone https://github.com/Q-FRC/Dashboard.git
cd QFRCDashboard
mkdir build && cd build
cmake -DCMAKE_PREFIX_PATH="C:\\Qt6\\6.6.1\\msvc2019_64" ..
cmake --build .
C:\Qt6\6.6.1\msvc2019_64\bin\windeployqt.exe .
```
OR use [CLion](https://www.jetbrains.com/clion/) or Qt Creator from the online installer.
