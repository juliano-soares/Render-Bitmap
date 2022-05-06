/**
    Slider functions
    @file Slider.cpp
    @author Juliano Leonardo Soares
    @version 1.1 03/05/22
*/
#include "Slider.h"

#include "../../Modules/gl_canvas2d.h"
#include "../../Utils/Utils.h"
#include <iostream>

using namespace std;

/* Constructor */
Slider::~Slider() {}

/* Create new Slider
   @param id: slider id.
   @param img: reference to the object to be manipulated.
   @param x: Slider start x position.
   @param y: Slider start y position.
   @param w: Slider width.
   @param h: Slider height.
   @param bg: background color.
   @param minRange: minimum value.
   @param maxRange: maximum value.
   @param value: current value.
*/
Slider::Slider(int id, Bitmap** img, int x, int y, float w, float h, vector<float> bg, int minRange, int maxRange, float value = 0)
    : minRange(minRange), maxRange(maxRange), value(value)
{
    this->imgController = img;
    this->minX = x;
    this->x = x;
    this->y = y;
    this->width = w;
    this->height  = h;
    this->bgColor = bg;
    this->cirX = x;
    this->id = id;
}

/* Renders/draws a slider on the screen
*/
void Slider::Render()
{
    vector<float> rgb = Utils::RGBtoFloat(255,250,250);
    CV::color(rgb[0], rgb[1], rgb[2]);
    CV::rectFill(x, y, x + width, y + height);
    CV::color(bgColor[0], bgColor[1], bgColor[2]);
    CV::rectFill(cirX-10, y-10, cirX+10, y+10);
}

/* Function responsible for checking the mouse position within the limits of the slider.
   @param mouseX: X position in the mouse Cartesian.
   @param mouseY: Y position in the mouse Cartesian.
   @return a boolean if mouse is within button limits.
*/
bool Slider::Collided(int mouseX, int mouseY)
{
    if(mouseX >= (cirX-10) && mouseX <= (cirX + 10) && mouseY >= (y-8) && mouseY <= (y+8)){
        return true;
    }
    return false;
}


/* Function responsible for moving slider indicator.
   @param mouseX: X position in the mouse Cartesian.
   @return value: bar float value.
*/
float Slider::MoveIndicator(int mouseX)
{
    if(mouseX >= minX && mouseX < (minX + width)) {
        cirX = mouseX;
        value = cirX - x;
    }
    return value;
}

/* reset the initial values.
*/
void Slider::ResetPosition()
{
    cirX = x;
    value = x;
}
