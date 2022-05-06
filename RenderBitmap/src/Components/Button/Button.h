/**
    Header Button
    @file Button.h
    @author Juliano Leonardo Soares
    @version 1.1 03/05/22
*/
#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "../Bitmap/Bitmap.h"
#include "../Histogram/Histogram.h"
#include <string>

using namespace std;

class Button
{
public:
    int x, y;                   // initial positions
    int width, height;          // width and height
    int id;                     // button id

    Bitmap** imgController;     // ref to object
    vector<float> bgColor;      // background color
    string label;               // label
    vector<float> labelColor;   // label color

    Button();
    ~Button();
    Button(Bitmap** img, int id, int x, int y, float w, float h, vector<float> bg, const string label, vector<float> labelColor);
    void Render();
    bool Collided(int mx, int my);
    string_size_1(const string label);
    void Hover(int mouseX, int mouseY);
};

#endif

