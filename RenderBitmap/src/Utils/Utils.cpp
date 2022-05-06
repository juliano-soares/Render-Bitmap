/**
    Utils functions
    @file Utils.cpp
    @author Juliano Leonardo Soares
    @version 1.1 03/05/22
*/
#include "../Modules/gl_canvas2d.h"
#include "Utils.h"
#include <unistd.h>
#include <iostream>

using namespace std;

static float calcRtoF (float i)
{
    float maxRGB = 255;
    return (i / maxRGB);
}

vector<float> Utils::RGBtoFloat (int r, int g, int b)
{
    vector<float> rgb;
    rgb.push_back(calcRtoF(r));
    rgb.push_back(calcRtoF(g));
    rgb.push_back(calcRtoF(b));
    return rgb;
}

string Utils::getCurrentPath() {
    char buf[100];
    getcwd(buf, sizeof(buf));
    string path = buf;
    return path;
}

string Utils::getImagePath (const char* filename) {
    string path = getCurrentPath();
    const char* aux;
    #ifdef WIN32
        aux = "\\RenderBitmap\\resources\\";
        path += aux;
    #else
        aux = "/RenderBitmap/resources/";
        path += aux;
    #endif
    path += filename;
    return path;
}
