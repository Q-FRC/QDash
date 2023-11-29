# QFRCDashboard
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
Coming soon. Downloads will be made available via Github Actions and Jenkins for Linux and Windows, and hopefully BSD and MacOS.

## Usage Guide
Set your NT server URL with the menubar option, create a new tab in the Tab menu, and add a widget using the New Widget menu option. Save and load layouts with the File menu, and resize and edit widgets as desired by right-clicking and selecting Resize, Delete, etc.

(Note: row and column numbers are zero-indexed. Row and column spans, however, start from 1.)

## WIP
QFRCDashboard is not complete. There are tons of new features, bugfixes, that will be added in up-and-coming releases. In its current state, QFRCDashboard is *completely usable for the average team*.

## Forking
Follow the [GPL3](LICENSE) of this project, credit the original project, and make it clear that your application is not QFRCDashboard itself. Cool? Cool.

## Contributing
You are allowed to contribute as you please. However, if you constantly spam useless/troll issues or pull requests, you'll probably get flagged by GitHub. Maybe don't do that :>

If you're at all confused with the code structure, make an issue about it and I will help you.

## Building
Simple CMake, just run

```bash
cmake -S . -B build
cd build
make -j$(nproc)
```

Install target will work eventually. :)

## Miscellaneous Notes
This is not tested on Windows as I don't have a working Windows installation. Please test and let me know how it works.

QFRCDashboard is created solely for the purpose of A RELIABLE, LOW-OVERHEAD FRC dashboard. QFRCDashboard is NOT created with eye-candy or intense theming in mind. Contributions and suggestions to theming will be accepted and worked on, but without user request or contribution, QFRCDashboard will see very little updates to theming.
