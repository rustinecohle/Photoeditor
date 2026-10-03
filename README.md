# Photoeditor 3.0

A lightweight BMP image editor with a graphical interface, written in C and built with the IUP toolkit.

## Features

- Open and save 24-bit uncompressed BMP images
- Convert images to grayscale
- Adjust brightness and invert colors
- Flip horizontally or vertically
- Rotate images by 90 degrees
- Crop to a selected region
- Apply blur and sharpen filters
- Undo the most recent edit

## Version 3.0

- Fixed the Cancel buttons in the Brightness and Crop dialogs so canceling closes the dialog without applying the entered changes.

## Linux

The `v3` branch includes the Linux IUP shared library under `main/third_party/iup/`. Install the build tools and GTK 3 development files, then build and run the `v3` branch:

```bash
sudo apt update
sudo apt install -y build-essential libgtk-3-dev pkg-config git

git clone --branch v3 --single-branch https://github.com/rustinecohle/Photoeditor.git Photoeditor-v3
cd Photoeditor-v3/main

make IUP_CFLAGS="-Ithird_party/iup/include" IUP_LIBS="-Lthird_party/iup/lib -liup"
LD_LIBRARY_PATH="$PWD/third_party/iup/lib${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}" make run
```

The `IUP_CFLAGS` and `IUP_LIBS` values tell `make` to use the IUP headers and library bundled with this branch. `LD_LIBRARY_PATH` lets Linux find the bundled IUP library when launching the application.

To rebuild after changing source files, run the `make` command again. To remove generated build files:

```bash
make clean
```

## Windows

The Windows build requires GCC/MinGW and a Windows-compatible IUP installation. Configure the paths in `main/Makefile` for your installation, then run `mingw32-make` and `mingw32-make run` from the `main` directory.

## Screenshots

### Opening Panel
![Opening Panel](screenshots/opening-panel.jpg)

### Opening Images
![Opening Images](screenshots/opening-image.jpg)

### Inverted Image
![Inverted Image](screenshots/inverted-image.jpg)
