/**
    Histogram functions
    @file Histogram.cpp
    @author Juliano Leonardo Soares
    @version 1.1 03/05/22
*/
#include "Histogram.h"
#include "../../Modules/gl_canvas2d.h"
#include "../../Utils/Utils.h"
#include <iostream>
#include <algorithm>

using namespace std;

/* Constructor */
Histogram::~Histogram() {}

/* Create new Histogram
   @param img: reference to the object to be manipulated.
   @param x: Histogram start x position.
   @param y: Histogram start y position.
   @param width: Histogram width.
   @param height: Histogram height.
*/
Histogram::Histogram(Bitmap* img, int x, int y, int width, int height)
{
    this->width = width;
    this->height = height;
    this->x = x + 60;
    this->y = y - 60;
    this->img = img;
    this->maxColor = 0;
    this->isBinary = img->isBinary;
}

/* Renders/draws a slider on the screen
*/
void Histogram::Render()
{
    offsetX = width/256;
    offsetY = height/100;
    rgb = Utils::RGBtoFloat(255, 255, 255);
    CV::color(rgb[0], rgb[1], rgb[2]);
    CV::rectFill(x - 80, y + 50, x + width + 40, y - height - 50);

    CV::color(0, 0, 0);
    CV::line(x, y, x + width, y);
    CV::line(x + width, y, x + width - 20, y + 10);
    CV::line(x + width, y, x + width - 20, y - 10);
    CV::text(x - 4, y + 16, "0");
    CV::text(x + (127 * offsetX) - 14, y + 16, "127");
    CV::text(x + (255 * offsetX) - 14, y + 16, "255");
    for(int i = 0; i < 256; i++) {
        if (i == 0 || i == 127 || i == 255)
            CV::line(x + (i * offsetX), y - 6, x + (i * offsetX), y + 6);
        if (i % 10 == 0)
            CV::line(x + (i * offsetX), y - 2, x + (i * offsetX), y + 2);
    }

    CV::color(0, 0, 0);
    CV::line(x, y - height, x, y);
    CV::line(x, y - height, x + 10, y - height + 20);
    CV::line(x, y - height, x - 10, y - height + 20);
    CV::text(x - 15, y - 4, "0");
    char buffer[7];
    int mid = maxColor/2;
    sprintf(buffer, "%d", mid);
    CV::text(x - SetPosition(mid), y - (50 * offsetY) - 4, buffer);
    sprintf(buffer, "%d", maxColor);
    CV::text(x - SetPosition(mid), y - (100 * offsetY) - 4, buffer);
    for(int i = 0; i < 100; i++) {
        if (i == 0 || i == 50 || i == 100)
            CV::line(x - 6, y - (i * offsetY), x + 6, y - (i * offsetY));
        if (i % 10 == 0)
            CV::line(x - 4, y - (i * offsetY), x + 4, y - (i * offsetY));
    }

    GetValues();
    for (int i = 0; i < 256; i++) {
        if(!isBinary){
            CV::color(0, 0, 0);
            CV::text(x + 75, y - height - 20, "Histogram - RGB");
            if (img->channel[0]) {
                CV::color(1, 0, 0);
                DrawLines(cR, i);
            }
            if (img->channel[1]) {
                CV::color(0, 1, 0);
                DrawLines(cG, i);
            }
            if (img->channel[2]) {
                CV::color(0, 0, 1);
                DrawLines(cB, i);
            }
        }
        if (isBinary) {
            CV::color(0, 0, 0);
            CV::text(x + 75, y - height - 20, "Histogram - Binary");
            CV::color(0, 0, 0);
            DrawLines(cBW, i);
        }
    }
}

/* draw the lines
   @param amount: number of lines on axis.
   @param i: position
*/
void Histogram::DrawLines(int* amount, int i)
{
    if (maxColor != 0) {
        int value = (int)((amount[i] * 100) / maxColor);
        CV::rectFill(x + (i * offsetX), y, x + (i * offsetX) + offsetX, y - (value * offsetY));
    }
}

/* Sets position histogram
*/
int Histogram::SetPosition(int x)
{
    if (x/100 < 1) return 25;
    else if (x/100 < 10) return 35;
    else if (x/ 100 < 100) return 45;
    else if (x/ 100 < 1000) return 55;
    else if (x/100 < 10000) return 65;
    return 0;
}

/* count the colors
*/
void Histogram::GetValues()
{
    mR = mG = mB = mBW = 0;
    for(int i = 0; i < 256; i++) {
        cR[i] = 0;
        cG[i] = 0;
        cB[i] = 0;
        cBW[i] = 0;
    }
    Matrix** matrix = img->GetImage();
    for(int i = 0; i < img->GetHeight(); i++)
        for(int j = 0; j < img->GetWidth(); j++) {
            Matrix c = matrix[i][j];
            cR[(int)c.r]++;
            cG[(int)c.g]++;
            cB[(int)c.b]++;
            int lum = (c.r * .299) + (c.g * .587) + (c.b * .114);
            cBW[lum]++;
        }
    mR = max_element(begin(cR), end(cR));
    mG = max_element(begin(cG), end(cG));
    mB = max_element(begin(cB), end(cB));
    mBW = max_element(begin(cBW), end(cBW));
    maxColor = max(*mR, *mG);
    maxColor = max(maxColor, *mB);
}



