/**
    Header Histogram
    @file Histogram.h
    @author Juliano Leonardo Soares
    @version 1.1 03/05/22
*/
#ifndef __HISTOGRAM_H__
#define __HISTOGRAM_H__

#include "../Bitmap/Bitmap.h"
#include <string>
#include <vector>

using namespace std;

class Histogram
{
private:
    Bitmap* img;                            // object manipulated
    int x, y;                               // initial positions
    int width, height;                      // dimensions
    int offsetX, offsetY;                   // dimensions
    int cR[256], cG[256], cB[256], cBW[256];// amount of colors
    int* mR, *mG, *mB, *mBW;                // max value RGB BW
    int  maxColor;                          // max color
    vector<float> rgb;                      // RGB

public:
    bool isBinary;
    Histogram();
    ~Histogram();
    Histogram(Bitmap* img, int x, int y, int xWidth, int yHeight);
    void DrawLines(int* counter, int i);
    void Render();
    int  SetPosition(int x);
    void GetValues();
};

#endif
