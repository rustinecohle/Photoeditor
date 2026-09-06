#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iup.h>

#include "openbmp.h"
#include "grayscale.h"
#include "brightness.h"
#include "invert.h"
#include "flip.h"
#include "rotate.h"
#include "crop.h"
#include "blur.h"
#include "sharpen.h"


BMPImage *image = NULL;
Ihandle *imageBox = NULL;



void updateImageDisplay(void);




/* from here the undo process begins */

unsigned char *previousPixels = NULL;
int previousWidth = 0;
int previousHeight = 0;
int hasUndo = 0;


void clearUndo(void)
{
    if (previousPixels != NULL) {
        free(previousPixels);
        previousPixels = NULL;
    }

    hasUndo = 0;
}


void saveUndo(void)
{
    if (image == NULL)
        return;

    /*
      clearing the previous steps and only remembering latest step
    */

    if (previousPixels != NULL) {
        free(previousPixels);
        previousPixels = NULL;
    }

    int size = image->width * image->height * 3;

    previousPixels = malloc(size);

    if (previousPixels == NULL)
        return;

    memcpy(
        previousPixels,
        image->pixels,
        size
    );

    previousWidth = image->width;
    previousHeight = image->height;

    hasUndo = 1;
}


void undo(Ihandle *ih)
{
    (void)ih;

    if (image == NULL) {
        IupMessage("Undo", "No image is open.");
        return;
    }

    if (!hasUndo) {
        IupMessage("Undo", "Nothing to undo.");
        return;
    }

    int size = previousWidth * previousHeight * 3;

    unsigned char *pixels = malloc(size);

    if (pixels == NULL) {
        IupMessage("Error", "Not enough memory for undo.");
        return;
    }

    memcpy(
        pixels,
        previousPixels,
        size
    );

    free(image->pixels);

    image->pixels = pixels;
    image->width = previousWidth;
    image->height = previousHeight;

    image->infoheader.width = previousWidth;
    image->infoheader.height = previousHeight;

  

    free(previousPixels);
    previousPixels = NULL;
    hasUndo = 0;

    updateImageDisplay();
}


/* for displaying image*/

void updateImageDisplay(void)
{
    if (image == NULL || imageBox == NULL)
        return;

    int width = image->width;
    int height = image->height;

    int size = width * height * 3;

    unsigned char *rgb = malloc(size);

    if (rgb == NULL)
        return;



    for (int i = 0; i < width * height * 3; i++) {
        rgb[i] = image->pixels[i];
    }

    Ihandle *iupImage =
        IupImageRGB(width, height, rgb);

    if (iupImage != NULL) {

        IupSetAttributeHandle(
            imageBox,
            "IMAGE",
            iupImage
        );

        char rasterSize[64];

        sprintf(
            rasterSize,
            "%dx%d",
            width,
            height
        );

        IupSetAttribute(
            imageBox,
            "RASTERSIZE",
            rasterSize
        );

        IupRefresh(IupGetDialog(imageBox));

        IupUpdate(imageBox);
    }

    free(rgb);
}


/* openings */

int open_cb(Ihandle *ih)
{
    (void)ih;

    Ihandle *dialog = IupFileDlg();

    IupSetAttribute(
        dialog,
        "DIALOGTYPE",
        "OPEN"
    );

    IupSetAttribute(
        dialog,
        "FILTER",
        "*.bmp"
    );

    IupSetAttribute(
        dialog,
        "FILTERINFO",
        "BMP Files"
    );

    IupSetAttribute(
        dialog,
        "TITLE",
        "Open BMP Image"
    );

    IupPopup(
        dialog,
        IUP_CENTER,
        IUP_CENTER
    );

    char *status =
        IupGetAttribute(dialog, "STATUS");

    if (status != NULL &&
        strcmp(status, "-1") != 0) {

        char *filename =
            IupGetAttribute(dialog, "VALUE");

        if (filename != NULL) {

            BMPImage *newImage =
                openBMP(filename);

            if (newImage != NULL) {

                if (image != NULL)
                    freeBMP(image);

                image = newImage;

                clearUndo();

                updateImageDisplay();
            }
            else {
                IupMessage(
                    "Error",
                    "Could not open BMP image."
                );
            }
        }
    }

    IupDestroy(dialog);

    return IUP_DEFAULT;
}


/* saving */

int save_cb(Ihandle *ih)
{
    (void)ih;

    if (image == NULL) {
        IupMessage(
            "Error",
            "Open an image first."
        );
        return IUP_DEFAULT;
    }

    Ihandle *dialog = IupFileDlg();

    IupSetAttribute(
        dialog,
        "DIALOGTYPE",
        "SAVE"
    );

    IupSetAttribute(
        dialog,
        "FILTER",
        "*.bmp"
    );

    IupSetAttribute(
        dialog,
        "FILTERINFO",
        "BMP Files"
    );

    IupSetAttribute(
        dialog,
        "TITLE",
        "Save BMP Image"
    );

    IupPopup(
        dialog,
        IUP_CENTER,
        IUP_CENTER
    );

    char *status =
        IupGetAttribute(dialog, "STATUS");

    if (status != NULL &&
        strcmp(status, "-1") != 0) {

        char *filename =
            IupGetAttribute(dialog, "VALUE");

        if (filename != NULL) {

            int result = saveBMP(filename, image);

            if (!result) {
                IupMessage(
                    "Error",
                    "Could not save BMP image."
                );
            }
            else {
                IupMessage(
                    "Success",
                    "Image saved successfully."
                );
            }
        }
    }

    IupDestroy(dialog);

    return IUP_DEFAULT;
}


/* gray */

int grayscale_cb(Ihandle *ih)
{
    (void)ih;

    if (image == NULL) {
        IupMessage(
            "Error",
            "Open an image first."
        );
        return IUP_DEFAULT;
    }

    saveUndo();

    grayscale(image);

    updateImageDisplay();

    return IUP_DEFAULT;
}


/* 
   BRIGHTNESS 
   */

int brightness_cb(Ihandle *ih)
{
    (void)ih;

    if (image == NULL) {
        IupMessage(
            "Error",
            "Open an image first."
        );
        return IUP_DEFAULT;
    }

    Ihandle *levelText =
        IupText(NULL);

    IupSetAttribute(
        levelText,
        "VALUE",
        "0"
    );

    IupSetAttribute(
        levelText,
        "RASTERSIZE",
        "100x25"
    );

    Ihandle *ok =
        IupButton("Apply", NULL);

    Ihandle *cancel =
        IupButton("Cancel", NULL);

    Ihandle *box =
        IupVbox(

            IupLabel(
                "Enter brightness change"
                " (-255 to 255):"
            ),

            levelText,

            IupHbox(
                ok,
                cancel,
                NULL
            ),

            NULL
        );

    Ihandle *dialog =
        IupDialog(box);

    IupSetAttribute(
        dialog,
        "TITLE",
        "Adjust Brightness"
    );

    IupSetAttribute(
        dialog,
        "RASTERSIZE",
        "260x150"
    );

    IupSetCallback(
        ok,
        "ACTION",
        (Icallback)IupExitLoop
    );

    IupSetCallback(
        cancel,
        "ACTION",
        (Icallback)IupExitLoop
    );

    IupPopup(
        dialog,
        IUP_CENTER,
        IUP_CENTER
    );

    char *levelValue =
        IupGetAttribute(
            levelText,
            "VALUE"
        );

    int level = atoi(levelValue);

    IupDestroy(dialog);

    if (level < -255 || level > 255) {
        IupMessage(
            "Error",
            "Brightness value must be"
            " between -255 and 255."
        );
        return IUP_DEFAULT;
    }

    saveUndo();

    brightness(image, level);

    updateImageDisplay();

    return IUP_DEFAULT;
}


/* inverse */

int invert_cb(Ihandle *ih)
{
    (void)ih;

    if (image == NULL) {
        IupMessage(
            "Error",
            "Open an image first."
        );
        return IUP_DEFAULT;
    }

    saveUndo();

    invert(image);

    updateImageDisplay();

    return IUP_DEFAULT;
}


/* 
   HORIZONTAL FLIP
   */

int horizontal_cb(Ihandle *ih)
{
    (void)ih;

    if (image == NULL) {
        IupMessage(
            "Error",
            "Open an image first."
        );
        return IUP_DEFAULT;
    }

    saveUndo();

    horizontalFlip(image);

    updateImageDisplay();

    return IUP_DEFAULT;
}


/* 
   VERTICAL FLIP
  */

int vertical_cb(Ihandle *ih)
{
    (void)ih;

    if (image == NULL) {
        IupMessage(
            "Error",
            "Open an image first."
        );
        return IUP_DEFAULT;
    }

    saveUndo();

    verticalFlip(image);

    updateImageDisplay();

    return IUP_DEFAULT;
}


/* rotation */

int rotate_cb(Ihandle *ih)
{
    (void)ih;

    if (image == NULL) {
        IupMessage(
            "Error",
            "Open an image first."
        );
        return IUP_DEFAULT;
    }

    saveUndo();

    rotate(image);

    updateImageDisplay();

    return IUP_DEFAULT;
}


/* cropping */

int crop_cb(Ihandle *ih)
{
    (void)ih;

    if (image == NULL) {
        IupMessage(
            "Error",
            "Open an image first."
        );
        return IUP_DEFAULT;
    }


    Ihandle *xText =
        IupText(NULL);

    Ihandle *yText =
        IupText(NULL);

    Ihandle *widthText =
        IupText(NULL);

    Ihandle *heightText =
        IupText(NULL);


    IupSetAttribute(
        xText,
        "VALUE",
        "0"
    );

    IupSetAttribute(
        yText,
        "VALUE",
        "0"
    );


    char value[32];


    sprintf(
        value,
        "%d",
        image->width / 2
    );

    IupSetAttribute(
        widthText,
        "VALUE",
        value
    );


    sprintf(
        value,
        "%d",
        image->height / 2
    );

    IupSetAttribute(
        heightText,
        "VALUE",
        value
    );


    IupSetAttribute(
        xText,
        "RASTERSIZE",
        "100x25"
    );

    IupSetAttribute(
        yText,
        "RASTERSIZE",
        "100x25"
    );

    IupSetAttribute(
        widthText,
        "RASTERSIZE",
        "100x25"
    );

    IupSetAttribute(
        heightText,
        "RASTERSIZE",
        "100x25"
    );


    Ihandle *ok =
        IupButton("Crop", NULL);

    Ihandle *cancel =
        IupButton("Cancel", NULL);


    Ihandle *box =
        IupVbox(

            IupHbox(
                IupLabel("Start X:"),
                xText,
                NULL
            ),

            IupHbox(
                IupLabel("Start Y:"),
                yText,
                NULL
            ),

            IupHbox(
                IupLabel("Width:"),
                widthText,
                NULL
            ),

            IupHbox(
                IupLabel("Height:"),
                heightText,
                NULL
            ),

            IupHbox(
                ok,
                cancel,
                NULL
            ),

            NULL
        );


    Ihandle *dialog =
        IupDialog(box);


    IupSetAttribute(
        dialog,
        "TITLE",
        "Crop Image"
    );

    IupSetAttribute(
        dialog,
        "RASTERSIZE",
        "300x220"
    );


    IupSetCallback(
        ok,
        "ACTION",
        (Icallback)IupExitLoop
    );

    IupSetCallback(
        cancel,
        "ACTION",
        (Icallback)IupExitLoop
    );


    IupPopup(
        dialog,
        IUP_CENTER,
        IUP_CENTER
    );


    char *xValue =
        IupGetAttribute(
            xText,
            "VALUE"
        );

    char *yValue =
        IupGetAttribute(
            yText,
            "VALUE"
        );

    char *wValue =
        IupGetAttribute(
            widthText,
            "VALUE"
        );

    char *hValue =
        IupGetAttribute(
            heightText,
            "VALUE"
        );


    int startX = atoi(xValue);
    int startY = atoi(yValue);

    int cropWidth =
        atoi(wValue);

    int cropHeight =
        atoi(hValue);


    IupDestroy(dialog);


    if (startX < 0 ||
        startY < 0 ||
        cropWidth <= 0 ||
        cropHeight <= 0 ||
        startX + cropWidth > image->width ||
        startY + cropHeight > image->height) {

        IupMessage(
            "Error",
            "Invalid crop area."
        );

        return IUP_DEFAULT;
    }


    saveUndo();


    crop(
        image,
        startX,
        startY,
        cropWidth,
        cropHeight
    );


    updateImageDisplay();


    return IUP_DEFAULT;
}


/* blurring  */

int blur_cb(Ihandle *ih)
{
    (void)ih;

    if (image == NULL) {
        IupMessage(
            "Error",
            "Open an image first."
        );
        return IUP_DEFAULT;
    }

    saveUndo();

    blur(image);

    updateImageDisplay();

    return IUP_DEFAULT;
}


/* shapening with kernel aplifying idea */

int sharpen_cb(Ihandle *ih)
{
    (void)ih;

    if (image == NULL) {
        IupMessage(
            "Error",
            "Open an image first."
        );
        return IUP_DEFAULT;
    }

    saveUndo();

    sharpen(image);

    updateImageDisplay();

    return IUP_DEFAULT;
}


/* exits*/

int exit_cb(Ihandle *ih)
{
    (void)ih;

    if (image != NULL) {
        freeBMP(image);
        image = NULL;
    }

    clearUndo();

    return IUP_CLOSE;
}




int main(int argc, char **argv)
{
    IupOpen(&argc, &argv);


    /* iup buttons i used ai for the tutorials */

    Ihandle *openButton =
        IupButton("Open BMP", NULL);

    Ihandle *undoButton =
        IupButton("Undo", NULL);

    Ihandle *saveButton =
        IupButton("Save BMP", NULL);

    Ihandle *grayscaleButton =
        IupButton("Grayscale", NULL);

    Ihandle *brightnessButton =
        IupButton("Brightness", NULL);

    Ihandle *invertButton =
        IupButton("Invert", NULL);

    Ihandle *horizontalButton =
        IupButton("Horizontal Flip", NULL);

    Ihandle *verticalButton =
        IupButton("Vertical Flip", NULL);

    Ihandle *rotateButton =
        IupButton("Rotate", NULL);

    Ihandle *cropButton =
        IupButton("Crop", NULL);

    Ihandle *blurButton =
        IupButton("Blur", NULL);

    Ihandle *sharpenButton =
        IupButton("Sharpen", NULL);

    Ihandle *exitButton =
        IupButton("Exit", NULL);


    /* callbacks*/

    IupSetCallback(
        openButton,
        "ACTION",
        (Icallback)open_cb
    );

    IupSetCallback(
        undoButton,
        "ACTION",
        (Icallback)undo
    );

    IupSetCallback(
        saveButton,
        "ACTION",
        (Icallback)save_cb
    );

    IupSetCallback(
        grayscaleButton,
        "ACTION",
        (Icallback)grayscale_cb
    );

    IupSetCallback(
        brightnessButton,
        "ACTION",
        (Icallback)brightness_cb
    );

    IupSetCallback(
        invertButton,
        "ACTION",
        (Icallback)invert_cb
    );

    IupSetCallback(
        horizontalButton,
        "ACTION",
        (Icallback)horizontal_cb
    );

    IupSetCallback(
        verticalButton,
        "ACTION",
        (Icallback)vertical_cb
    );

    IupSetCallback(
        rotateButton,
        "ACTION",
        (Icallback)rotate_cb
    );

    IupSetCallback(
        cropButton,
        "ACTION",
        (Icallback)crop_cb
    );

    IupSetCallback(
        blurButton,
        "ACTION",
        (Icallback)blur_cb
    );

    IupSetCallback(
        sharpenButton,
        "ACTION",
        (Icallback)sharpen_cb
    );

    IupSetCallback(
        exitButton,
        "ACTION",
        (Icallback)exit_cb
    );


    /* display */

    imageBox =
        IupLabel(NULL);

    IupSetAttribute(
        imageBox,
        "ALIGNMENT",
        "ACENTER"
    );

    IupSetAttribute(
        imageBox,
        "EXPAND",
        "YES"
    );



    unsigned char placeholderPixel[3] =
        { 240, 240, 240 };

    Ihandle *placeholderImage =
        IupImageRGB(1, 1, placeholderPixel);

    IupSetAttributeHandle(
        imageBox,
        "IMAGE",
        placeholderImage
    );




    Ihandle *row1 =
        IupHbox(
            openButton,
            undoButton,
            saveButton,
            exitButton,
            NULL
        );


    Ihandle *row2 =
        IupHbox(
            grayscaleButton,
            brightnessButton,
            invertButton,
            NULL
        );


    Ihandle *row3 =
        IupHbox(
            horizontalButton,
            verticalButton,
            rotateButton,
            cropButton,
            NULL
        );


    Ihandle *row4 =
        IupHbox(
            blurButton,
            sharpenButton,
            NULL
        );


    Ihandle *controls =
        IupVbox(
            row1,
            row2,
            row3,
            row4,
            NULL
        );


    IupSetAttribute(
        controls,
        "GAP",
        "5"
    );


    

    Ihandle *mainBox =
        IupVbox(
            controls,
            imageBox,
            NULL
        );


    IupSetAttribute(
        mainBox,
        "MARGIN",
        "10x10"
    );

    IupSetAttribute(
        mainBox,
        "GAP",
        "10"
    );


    Ihandle *dialog =
        IupDialog(mainBox);


    IupSetAttribute(
        dialog,
        "TITLE",
        "BMP Image Manipulation Software"
    );


    IupSetAttribute(
        dialog,
        "RASTERSIZE",
        "1000x700"
    );


    IupShowXY(
        dialog,
        IUP_CENTER,
        IUP_CENTER
    );


    IupMainLoop();


    if (image != NULL) {
        freeBMP(image);
        image = NULL;
    }

    clearUndo();


    IupClose();


    return 0;
}
