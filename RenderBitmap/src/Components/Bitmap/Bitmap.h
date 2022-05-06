/**
    Header Bitmap images
    @file Bitmap.h
    @author Juliano Leonardo Soares
    @version 1.1 03/05/22
*/
#ifndef __BITMAP__H___
#define __BITMAP__H___

#include "../../Utils/Utils.h"
#include <string>

#define HEADER_SIZE 14     // sizeof(HEADER) vai dar 16 devido ao alinhamento de bytes
#define INFOHEADER_SIZE 40 // sizeof(INFOHEADER) da 40 e esta correto.
#define uchar unsigned char

using namespace std;

typedef struct
{
    unsigned short int type;      /* Magic identifier            */
    unsigned int size;            /* File size in bytes          */
    unsigned short int reserved1, reserved2;
    unsigned int offset;          /* Offset to image data em bytes*/
} HEADER;

typedef struct
{
    unsigned int size;            /* Header size in bytes      */
    int width, height;            /* Width and height of image */
    unsigned short int planes;    /* Number of colour planes   */
    unsigned short int bits;      /* Bits per pixel            */
    unsigned int compression;     /* Compression type          */
    unsigned int imagesize;       /* Image size in bytes       */
    int xresolution, yresolution; /* Pixels per meter          */
    unsigned int ncolours;        /* Number of colours         */
    unsigned int impcolours;      /* Important colours         */
} INFOHEADER;

class Bitmap
{
private:
    int width, height, imagesize, bytesPerLine, bits;
    int tamX, tamY;

    HEADER header;
    INFOHEADER info;
    void load(const char *path);

public:
    Matrix **imgData;          // image matrix in use
    Matrix **imgDataInput;     // input image matrix
    Matrix **imgDataCurrent;   // current image matrix

    int constBrightness = 0; // value of brightness
    int constContrast = 0;     // value of contrast
    int efBinary = 0;          // number of times the binary effect was applied
    int efSepia = 0;           // number of times the sepia effect was applied
    int efBlur = 0;            // number of times the blur effect was applied
    int efMono = 0;            // number of times the monochrome effect was applied

    int index;                 // image index
    int idImg;                 // image id
    int positionX = 0;         // initial x position of the image
    int positionY = 0;         // initial y position of the image

    vector<bool> btmCheckbox = {false, false, false}; // flag if checkboxes has been applied
    bool isBinary = false;     // flag if binary has been applied
    bool isSelect = false;     // flag if selected has been applied

    vector<int> channel;       // colors channel
    string path;               // image path

    // Constructor
    Bitmap();
    Bitmap(string fileName);

    // Getters
    Matrix **CreateBitmap(int h, int w);
    Matrix **GetImage(void);
    int GetWidth(void);
    int GetHeight(void);

    // States
    void RenderBitmap(int px, int py);
    void RenderSelection(int px, int py);
    void CurrentStateImage(void);
    bool Collided(int mouseX, int mouseY);

    // Size and orientation
    void MirrorImage(int direction);
    void RotateImage(int direction);
    void scale(int type);

    // Brightness and color adjustments
    void Brightness(float brightness);
    void Contrast(int contrast);

    // Effects
    void Binary(Matrix **matriz);
    void Monochrome(Matrix **matriz);
    void Blur(Matrix **matriz);
    void Sepia(Matrix **matriz);

    // Management
    void Write(void);
    void Reset(void);
    void DeleteBitmap(Matrix **matrix, int h, int w);
};

#endif
