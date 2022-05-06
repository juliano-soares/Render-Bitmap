/**
    Utils functions
    @file Utils.h
    @author Juliano Leonardo Soares
    @version 1.1 03/05/22
*/
#ifndef ___UTILS__H___
#define ___UTILS__H___

#include <vector>
#include <string>

using namespace std;

class Utils
{
public:
    static vector<float> RGBtoFloat(int r, int g, int b);
    static string getImagePath(const char* filename);
    static string getCurrentPath();
};

typedef struct
{
    unsigned char  b;
    unsigned char  g;
    unsigned char  r;
} __attribute__((__packed__))
Matrix;
#endif

