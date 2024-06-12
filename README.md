# QFRCDashboard
A reliable, high-performance, low-footprint dashboard for use with FRC.

## Download
Coming soon. Downloads will be made available via Github Actions and Jenkins for Linux and Windows, and hopefully BSD and MacOS.

## Usage Guide
Set your NT server URL with the menubar option, create a new tab in the Tab menu, and add a widget using the New Widget menu option. Save and load layouts with the File menu, and resize and edit widgets as desired by right-clicking and selecting Resize, Delete, etc.

(Note: row and column numbers are zero-indexed. Row and column spans, however, start from 1.)

## WIP
This is very much WIP, don't expect everything to work. Also, I'm not a UI designer, so the final look and feel of the application is absolutely not what you see now. You can check the [todo.md](todo.md) file to see what my next plans are, however be aware that this is not exhaustive. Support for theming (possibly through custom QRCs) will come later.

For now, please feel free to use this and give me feedback via issues, or, if you're brave enough to dive into the realm of unaudited Qt code, make a PR.

## Forking
Follow the [GPL3](LICENSE) of this project, credit the original project, and make it clear that your application is not QFRCDashboard itself. Cool? Cool.

## Contributing
You are allowed to contribute as you please. However, if you constantly spam useless/troll issues or pull requests, you'll probably get flagged by GitHub. Maybe don't do that :>

If you're at all confused with the code structure, make an issue about it and I will help you.

## Building
Simple CMake, just run

```bash
CMake -S . -B build
cd build
make -j$(nproc)
```

I don't know how Windows works with CMake, so compile it however Windows does it.

## Miscellaneous Notes
This is not tested on Windows as I don't have a working Windows installation. Please test and let me know how it works.

This was not created with the intent of looking the best or having the most features. This was created solely to solve the performance and reliability issues of existing solutions, most notably SmartDashboard and Shuffleboard. Feel free to help make it look better, but please don't complain that it doesn't have 3500 themes individually color-coded to every FRC team in existence. :)
