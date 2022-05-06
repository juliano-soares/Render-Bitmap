/**
    Functions to open and manage Bitmap images
    @file Bitmap.cpp
    @author Juliano Leonardo Soares
    @version 1.1 03/05/22
*/
#include "Bitmap.h"
#include "../../Modules/gl_canvas2d.h"

#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>

#include "../../Utils/Utils.h"

using namespace std;

//**********************************************************
//                   Constructors                         //
//**********************************************************

/* Constructor */
Bitmap::Bitmap(){}

/* Load a Bitmap image.
   @param fileName: Bitmap image name.
   @return bitmap class instance.
*/
Bitmap::Bitmap(string fileName)
{
    width = height = 0;
    channel = {1,1,1};
    path = fileName;
    if(!path.empty() && path.size() > 0)
        load(path.c_str());
    else {
        printf("Arquivo Bitmap nao encontrado");
        return;
    }
}

//**********************************************************
//                      Getters                           //
//**********************************************************

/* Create new matrix for a bitmap.
   @param width: width of matrix.
   @param height: height of matrix.
   @return newDataMatrix: image data matrix.
*/
Matrix** Bitmap::CreateBitmap(int width, int height)
{
    Matrix** newDataMatrix = new Matrix*[height];
    for(int i = 0; i < height; i++)
        newDataMatrix[i] = new Matrix[width];
    return newDataMatrix;
}

/* Function responsible for returning the image data matrix.
   @return an matrix of image data.
*/
Matrix** Bitmap::GetImage(void)
{
    return imgData;
}

/* Function responsible for returning the width of the image.
   @return an integer of the width of the image.
*/
int Bitmap::GetWidth(void)
{
  return width;
}

/* Function responsible for returning the height of the image.
   @return an integer of the height of the image.
*/
int Bitmap::GetHeight(void)
{
  return height;
}

//**********************************************************
//                       States                           //
//**********************************************************

/* Render/draw the Bitmap image.
   @param px: X coordinate where the Bitmap starts to be drawn.
   @param py: Y coordinate where the Bitmap starts to be drawn.
*/
void Bitmap::RenderBitmap(int px, int py)
{
    if(isSelect) {
        RenderSelection(px, py);
    }

    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            vector<float> rgb = Utils::RGBtoFloat(imgData[i][j].r , imgData[i][j].g , imgData[i][j].b);
            CV::color(rgb[0] * channel[0], rgb[1] * channel[1], rgb[2] * channel[2]);
            CV::point(px + j, py + height - i);
        }
    }
}

/* Renders/draws a square around the selected image.
   @param px: X coordinate where the Bitmap starts to be drawn.
   @param py: Y coordinate where the Bitmap starts to be drawn.
*/
void Bitmap::RenderSelection(int px, int py)
{
    vector<float> rgb = Utils::RGBtoFloat(175, 175, 175);
    CV::color(rgb[0], rgb[1], rgb[2]);

    CV::rectFill(px - 5, py - 5, px, py + height + 5);
    CV::rectFill(px + width, py - 5, px + 5 + width, py + height + 5);
    CV::rectFill(px - 5, py - 5, px + width, py + 5);
    CV::rectFill(px - 5, py + height, px + width + 5, py + height + 5);
}

/* Function responsible for getting the information of the image
   at the moment to receive new changes.
*/
void Bitmap::CurrentStateImage(void)
{
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            float r = constBrightness * imgDataInput[i][j].r + constContrast;
            float g = constBrightness * imgDataInput[i][j].g + constContrast;
            float b = constBrightness * imgDataInput[i][j].b + constContrast;
            imgDataCurrent[i][j].r = r;
            imgDataCurrent[i][j].g = g;
            imgDataCurrent[i][j].b = b;
        }
    }

    for(int k = 0; k < efBinary; k++)
        Binary(imgDataCurrent);
    for(int k = 0; k < efSepia; k++)
        Sepia(imgDataCurrent);
    for(int k = 0; k < efBlur; k++)
        Blur(imgDataCurrent);
    for(int k = 0; k < efMono; k++)
        Monochrome(imgDataCurrent);
}

/* Function responsible for checking the mouse position within the limits of the image.
   @param mouseX: X position in the mouse Cartesian.
   @param mouseY: Y position in the mouse Cartesian.
   @return a boolean if mouse is within image limits.
*/
bool Bitmap::Collided(int mouseX, int mouseY)
{
    if(mouseX >= positionX && mouseX <= (positionX + width) && mouseY >= positionY && mouseY <= (positionY + height))
        return true;
    return false;
}

//**********************************************************
//              Size and orientation                      //
//**********************************************************

/* Function responsible for checking the mouse position within the limits of the image.
   @param direction: integer variable to signal flip direction on the image being: 0 - Vertical; 1 - Horizontal.
*/
void Bitmap::MirrorImage(int direction)
{
    if(direction){
        for (int i = 0; i < height; i++) {
            int y = width - 1;
            for (int j = 0; j < width/2; j++) {
                swap(imgData[i][j], imgData[i][y]);
                y--;
            }
        }
    } else {
        for (int i = 0; i < width; i++) {
            int y = height - 1;
            for (int j = 0; j < height/2; j++) {
                swap(imgData[j][i], imgData[y][i]);
                y--;
            }
        }
    }
}

/* Function responsible for rotating the image.
   @param direction: integer variable to signal flip direction in the image being: 0 - Right; 1 - Left.
*/
void Bitmap::RotateImage(int direction)
{
    Matrix** temp = CreateBitmap(width, height);
    if (direction) {
        for (int i = 0; i < height; i++) {
            int y = 0;
            for (int j = width-1; j >= 0; j--) {
                temp[y][i] = imgData[i][j];
                y++;
            }
        }
    }
    else {
        int y = height - 1;
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                temp[j][y] = imgData[i][j];
            }
            y--;
        }
    }
    DeleteBitmap(imgData, height, width);
    swap(height, width);
    imgData = CreateBitmap(width, height);
    imgData = temp;
}

/* Function responsible for scaling the image.
   @param direction: integer variable to signal new image scale size:
                     0 - 1/2; 1 - 1/4; 2 - 1/6; 3 - 1/8; 4 - 1/10.
*/
void Bitmap::scale(int type)
{
    int h = height;
    int w = width;

    if(type == 0){
        h = height/2;
        w = width/2;
    }
    else if (type == 1){
        h = height/4;
        w = width/4;
    }
    else if (type == 2){
        h = height/6;
        w = width/6;
    }
    else if (type == 3) {
        h = height/8;
        w = width/8;
    }
    else if (type == 4){
        h = height/10;
        w = width/10;
    }

    Matrix** temp = CreateBitmap(w, h);
    int ix = 0;
    for (int i = 0; ix < h; i += 2) {
        int jx = 0;
        for (int j = 0; jx < w; j += 2) {
             temp[ix][jx] = imgData[i][j];
             jx++;
        }
        ix++;
    }
    DeleteBitmap(imgData, height, width);
    imgData = CreateBitmap(w, h);
    imgData = temp;
    height = h;
    width = w;
}

//**********************************************************
//         Brightness and color adjustments               //
//**********************************************************

/* Function responsible for changing the image brightness.
   @param brightness: int variable value of brightness.
*/
void Bitmap::Brightness(float brightness)
{
    brightness = (brightness + 5) / 30;
    CurrentStateImage();
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            float r = imgDataCurrent[i][j].r;
            float g = imgDataCurrent[i][j].g;
            float b = imgDataCurrent[i][j].b;
            b = brightness * b;
            g = brightness * g;
            r = brightness * r;
            if (r > 255) r = 255;
            if (b > 255) b = 255;
            if (g > 255) g = 255;
            imgData[i][j].r = r;
            imgData[i][j].g = g;
            imgData[i][j].b = b;
            constBrightness = brightness;
        }
    }
}

/* Function responsible for changing the image contrast.
   @param contrast: float variable value of contrast.
*/
void Bitmap::Contrast(int contrast)
{
    contrast = (int)(contrast + 5);
    CurrentStateImage();
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            float r = imgDataCurrent[i][j].r;
            float g = imgDataCurrent[i][j].g;
            float b = imgDataCurrent[i][j].b;
            b = b + contrast;
            g = g + contrast;
            r = r + contrast;
            if (r > 255) r = 255;
            if (b > 255) b = 255;
            if (g > 255) g = 255;
            imgData[i][j].r = r;
            imgData[i][j].g = g;
            imgData[i][j].b = b;
            constContrast = contrast;
        }
    }
}

//**********************************************************
//                       Effects                          //
//**********************************************************

/* function responsible for applying the binary effect on the image.
   @param matrix: image information matrix.
*/
void Bitmap::Binary(Matrix** matrix)
{
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            float luminance = .299 * imgData[i][j].r + .587 * imgData[i][j].g + .114 * imgData[i][j].b;
            matrix[i][j].r = luminance;
            matrix[i][j].g = luminance;
            matrix[i][j].b = luminance;
        }
    }
    isBinary = true;
}

/* function responsible for applying the monochrome effect on the image.
   @param matrix: image information matrix.
*/
void Bitmap::Monochrome(Matrix** matrix)
{
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            float luminance = .299 * imgData[i][j].r + .587 * imgData[i][j].g + .114 * imgData[i][j].b;
            if(luminance > 120) {
                matrix[i][j].r = 0;
                matrix[i][j].g = 0;
                matrix[i][j].b = 0;
            } else {
                matrix[i][j].r = 255;
                matrix[i][j].g = 255;
                matrix[i][j].b = 255;
            }
        }
    }
}

/* function responsible for applying the blur effect on the image.
   @param matrix: image information matrix.
*/
void Bitmap::Blur(Matrix** matrix)
{
    int sumBlue, sumGreen, sumRed;
    float countColor;
    Matrix temp[height][width]; // para nao ir alterando os valores de dt no loop
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            sumBlue = sumGreen = sumRed = 0;
            countColor = 0.00;
            for (int k = -1; k < 2; k++) {
                if (j + k < 0 || j + k > height - 1) continue; // se  vizinho "sai" da imagem
                for (int h = -1; h < 2; h++) {
                    if (i + h < 0 || i + h > width - 1) continue;// se vizinho "sai" da imagem
                    sumBlue += matrix[j+k][i+h].r;
                    sumGreen += matrix[j+k][i+h].g;
                    sumRed += matrix[j+k][i+h].b;
                    countColor++;
                }
            }
            temp[j][i].r = round(sumBlue/countColor);
            temp[j][i].g = round(sumGreen/countColor);
            temp[j][i].b = round(sumRed/countColor);
        }
    }
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            matrix[j][i].r = temp[j][i].r;
            matrix[j][i].g = temp[j][i].g;
            matrix[j][i].b = temp[j][i].b;
        }
    }
}

/* function responsible for applying the sepia effect on the image.
   @param matrix: image information matrix.
*/
void Bitmap::Sepia(Matrix** matrix)
{
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            float r = matrix[i][j].r;
            float g = matrix[i][j].g;
            float b = matrix[i][j].b;
            r = ((r + g + b) / 3) + 20 * 2;
            g = ((r + g + b) / 3) + 20;
            if (r > 255) r = 255;
            if (g > 255) g = 255;
            matrix[i][j].r = r;
            matrix[i][j].g = g;
            matrix[i][j].b = ((r + g + b) / 3);
        }
    }
}

//**********************************************************
//                       Management                       //
//**********************************************************

/* Load a bitmap image.
   @param fileName: image path.
*/
void Bitmap::load(const char* fileName)
{
    FILE *fp = fopen(fileName, "rb");
    if( fp == NULL )
    {
        printf("\nErro ao abrir arquivo %s para leitura", fileName);
        return;
    }
    path = fileName;

    fread(&header.type,      sizeof(unsigned short int), 1, fp);
    fread(&header.size,      sizeof(unsigned int),       1, fp);
    fread(&header.reserved1, sizeof(unsigned short int), 1, fp);
    fread(&header.reserved2, sizeof(unsigned short int), 1, fp);
    fread(&header.offset,    sizeof(unsigned int),       1, fp);

    fread(&info.size,        sizeof(unsigned int),       1, fp);
    fread(&info.width,       sizeof(int),                1, fp);
    fread(&info.height,      sizeof(int),                1, fp);
    fread(&info.planes,      sizeof(unsigned short int), 1, fp);
    fread(&info.bits,        sizeof(unsigned short int), 1, fp);
    fread(&info.compression, sizeof(unsigned int),       1, fp);
    fread(&info.imagesize,   sizeof(unsigned int),       1, fp);
    fread(&info.xresolution, sizeof(int),                1, fp);
    fread(&info.yresolution, sizeof(int),                1, fp);
    fread(&info.ncolours,    sizeof(unsigned int),       1, fp);
    fread(&info.impcolours,  sizeof(unsigned int),       1, fp);

    width  = info.width;
    height = info.height;
    tamX = info.width;
    tamY = info.height;
    bits   = info.bits;
    bytesPerLine =(3 * (width + 1) / 4) * 4;
    imagesize    = bytesPerLine*height;

    if( header.type != 19778 )
    {
        printf("\nError: Arquivo BMP invalido");
        getchar();
        exit(0);
    }

    if( width*height*3 != imagesize )
    {
        printf("\nWarning: Arquivo BMP nao tem largura multipla de 4");
        getchar();
    }

    if( info.compression != 0 )
    {
        printf("\nError: Formato BMP comprimido nao suportado");
        getchar();
        return;
    }
    if( bits != 24 )
    {
        printf("\nError: Formato BMP com %d bits/pixel nao suportado", bits);
        getchar();
        return;
    }

    if( info.planes != 1 )
    {
        printf("\nError: Numero de Planes nao suportado: %d", info.planes);
        getchar();
        return;
    }

    imgData = CreateBitmap(width, height);
    imgDataInput = CreateBitmap(width, height);
    imgDataCurrent = CreateBitmap(width, height);
    fseek(fp, header.offset, SEEK_SET); // bug fix

    // Convert BGR to RGB
    Matrix(*image)[width] = (Matrix(*)[width])calloc(height, width * sizeof(Matrix));
    for (int i = 0; i < height; i++) {
        fread(image[i], sizeof(Matrix), width, fp);
    }
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            imgData[i][j] = image[i][j];
            imgDataInput[i][j] = image[i][j];
            imgDataCurrent[i][j] = image[i][j];
        }
    }

    fclose(fp);
}

/* Save a bitmap image.
*/
void Bitmap::Write()
{
    path = path.substr(0, path.find(".bmp")) + "Save.bmp";
    const char* name = path.c_str();
    FILE *fp = fopen(name, "wb");
    info.width = width;
    info.height = height;
    info.bits = bits;
    int padding  = (4 - (width * sizeof(Matrix)) % 4) % 4;
    header.size = 54 + (width * height * 3) + (padding *3);

    fwrite(&header.type,      sizeof(unsigned short int), 1, fp);
    fwrite(&header.size,      sizeof(unsigned int),       1, fp);
    fwrite(&header.reserved1, sizeof(unsigned short int), 1, fp);
    fwrite(&header.reserved2, sizeof(unsigned short int), 1, fp);
    fwrite(&header.offset,    sizeof(unsigned int),       1, fp);

    fwrite(&info.size,        sizeof(unsigned int),       1, fp);
    fwrite(&info.width,       sizeof(int),                1, fp);
    fwrite(&info.height,      sizeof(int),                1, fp);
    fwrite(&info.planes,      sizeof(unsigned short int), 1, fp);
    fwrite(&info.bits,        sizeof(unsigned short int), 1, fp);
    fwrite(&info.compression, sizeof(unsigned int),       1, fp);
    fwrite(&info.imagesize,   sizeof(unsigned int),       1, fp);
    fwrite(&info.xresolution, sizeof(int),                1, fp);
    fwrite(&info.yresolution, sizeof(int),                1, fp);
    fwrite(&info.ncolours,    sizeof(unsigned int),       1, fp);
    fwrite(&info.impcolours,  sizeof(unsigned int),       1, fp);

    bytesPerLine =(3 * (width + 1) / 4) * 4;
    imagesize    = bytesPerLine*height;

    Matrix(*image)[width] = (Matrix(*)[width])calloc(height, width * sizeof(Matrix));
    for(int i = 0; i < height; i++)
        for(int j = 0; j < width; j++) {
            image[i][j].r = imgData[i][j].r * channel[0];
            image[i][j].g = imgData[i][j].g * channel[1];
            image[i][j].b = imgData[i][j].b * channel[2];
        }

    for (int i = 0; i < height; i++) {
        fwrite(image[i], sizeof(Matrix), width, fp);
        fseek(fp, padding, SEEK_CUR);
    }
    for (int k = 0; k < padding; k++)
            fputc(0x00, fp);
    fclose(fp);
}

/* Resets the selected image to the original.
*/
void Bitmap::Reset()
{
    DeleteBitmap(imgData, height, width);
    imgData = CreateBitmap(tamX, tamY);
    height = tamY;
    width = tamX;
    for(int i = 0; i < tamX; i++) {
        for(int j = 0; j < tamY; j++) {
            imgData[i][j] = imgDataInput[i][j];
        }
    }
    isBinary = false;
    efMono = 0;
    efBinary = 0;
    efSepia = 0;
    efBlur = 0;
}

/* Delete a bitmap image
   @param matrix: information array of the image to be deleted.
   @param width: width of matrix
   @param height: height of matrix
*/
void Bitmap::DeleteBitmap(Matrix** matrix, int width, int height)
{
    for(int i = 0; i < height; i++)
        delete matrix[i];
    delete matrix;
}
