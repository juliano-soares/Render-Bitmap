/**
    Checkbox functions
    @file Checkbox.cpp
    @author Juliano Leonardo Soares
    @version 1.1 03/05/22
*/
#include "Checkbox.h"
#include "../../Modules/gl_canvas2d.h"
#include "../../Utils/Utils.h"
#include <iostream>

using namespace std;

/* Constructor */
Checkbox::~Checkbox() {}

/* Create new Checkbox
   @param img: reference to the object to be manipulated.
   @param x: button start x position.
   @param y: button start y position.
   @param w: button width.
   @param h: button height.
   @param isChecked: beleano to mark as verified
   @param bg: background color.
   @param label: label button.
   @param labelColor: label color.
*/
Checkbox::Checkbox(Bitmap** img, int x, int y, float w, float h, bool isChecked, string label, vector<float> labelColor, vector<float> bg)
    : isChecked(isChecked), label(label), labelColor(labelColor)
{
    this->imgController = img;
    this->x = x;
    this->y = y;
    this->width = w;
    this->height = h;
    this->bgColor = bg;
}

/* Renders/draws a checkbox on the screen
*/
void Checkbox::Render()
{
    CV::color(bgColor[0], bgColor[1], bgColor[2]);
    CV::rectFill(x, y, x + width, y + height);
    vector<float> c = Utils::RGBtoFloat(65,105,225);
    CV::color(c[0], c[1], c[2]);
    CV::circle(x + width/2, y + height/2, width/3, 100);
    CV::color(labelColor[0], labelColor[1], labelColor[2]);
    CV::text(x+width+5, y+5+height/2, label.c_str());
    if (isChecked) {
        vector<float> rgb = Utils::RGBtoFloat(26,35,39);
        CV::color(rgb[0], rgb[1], rgb[2]);
        CV::circleFill(x + width/2, y + height/2, width/3, 100);
    }
}

/* Function responsible for checking the mouse position within the limits of the checkbox.
   @param mouseX: X position in the mouse Cartesian.
   @param mouseY: Y position in the mouse Cartesian.
   @return a boolean if mouse is within button limits.
*/
bool Checkbox::Collided(int mouseX, int mouseY)
{
    if(mouseX >= x && mouseX <= (x + width) && mouseY >= y && mouseY <= (y + height))
        return true;
    return false;
}
