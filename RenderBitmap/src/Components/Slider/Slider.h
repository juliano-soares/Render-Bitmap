/**
    Header Slider
    @file Slider.h
    @author Juliano Leonardo Soares
    @version 1.1 03/05/22
*/
#ifndef __SLIDER_H__
#define __SLIDER_H__

#include "../Bitmap/Bitmap.h"
#include <string>

using namespace std;

class Slider
{
public:
    Bitmap** imgController; // object manipulated
    int id;                 // slider id
    int cirX, x, y;         // initial positions
    int height, width;      // dimensions
    int minX;               // minimum X position
    int minRange;           // slide min range
    int maxRange;           // slide max range
    float value;            // change value
    bool moved = false;     // flag moved
    string name;            // slide name
    vector<float> bgColor;  // background color

    Slider();
    ~Slider();
    Slider(int id, Bitmap** img, int x, int y, float w, float h, vector<float> bg, int minRange, int maxRange, float inc);
    void Render();
    float MoveIndicator(int mouseX);
    bool Collided(int mouseX, int mouseY);
    void ResetPosition();
};

#endif


