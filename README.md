
# Photoeditor

A lightweight BMP image editing tool with a graphical interface, built in C using the [IUP](https://www.tecgraf.puc-rio.br/iup/) toolkit.

## Features

* Open and save 24-bit uncompressed `.bmp` images
* Grayscale conversion
* Brightness adjustment 
* Color invert
* Horizontal and vertical flip
* Rotate (90°  clockwise )
* Crop to a custom region
* Blur (3x3 box blur)
* Sharpen (3x3 kernel)
* Undo ( 1 step)

## Requirements

* GCC and `make`
* GTK3 development headers (`libgtk-3-dev`) and `pkg-config`

The [IUP](https://www.tecgraf.puc-rio.br/iup/) library itself is **already bundled** in this repo under `main/third_party/iup/`, so you do **not** need to download or install IUP separately.

## Linux

```bash
sudo apt update
sudo apt install -y build-essential libgtk-3-dev pkg-config git

git clone https://github.com/rustinecohle/Photoeditor.git
cd Photoeditor/main
make
make run
```

## Windows

```powershell
cd $HOME
git clone https://github.com/rustinecohle/Photoeditor.git
cd Photoeditor\main
.\app.exe
```
## Screenshots

### Opening Panel
![Opening Panel](screenshots/opening-panel.jpg)


### Opening Images
![Opening Images](screenshots/opening-image.jpg)

### Inverted Image
![Inverted Image](screenshots/inverted-image.jpg)
