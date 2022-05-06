/**
    Header Checkbox
    @file Checkbox.h
    @author Juliano Leonardo Soares
    @version 1.1 03/05/22
*/
#ifndef __CHECKBOX_H__
#define __CHECKBOX_H__

#include "../Bitmap/Bitmap.h"
#include "../Histogram/Histogram.h"
#include <string>

using namespace std;

class Checkbox
{
public:
    Bitmap** imgController;     // object manipulated
    int  width, height;         // dimensions
    int x, y;                   // initial position
    bool isChecked;             // flag is checked
    string label;               // label
    vector<float> bgColor;      // background color
    vector<float> labelColor;   // label color

    Checkbox();
    ~Checkbox();
    Checkbox(Bitmap** img, int x, int y, float w, float h, bool isChecked, string label, vector<float> labelColor, vector<float> bg);
    void Render();
    bool Collided(int mouseX,int mouseY);
};

#endif

