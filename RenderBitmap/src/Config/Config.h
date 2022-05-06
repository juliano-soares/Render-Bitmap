/**
    Header Config
    This file stores as variables used in config to manage an application.
    @file Bitmap.h
    @author Juliano Leonardo Soares
    @version 1.1 03/05/22
*/
#ifndef ___CONFIG__H___
#define ___CONFIG__H___

#include "../Components/Button/Button.h"
#include "../Components/Slider/Slider.h"
#include "../Components/Histogram/Histogram.h"
#include "../Components/Checkbox/Checkbox.h"
#include <vector>

using namespace std;

class Config
{
private:
    vector<float> rgb;
    int opcao = 50;
    int mouseX, mouseY;

public:
    int idImgSel = 0;
    bool isActivesHistograms = false;
    Button* histActive;
    Histogram* hist;
    Histogram* hist1;
    Histogram* hist2;

    int cliqueX = 0;
    int cliqueY = 0;

    bool movSlider = false;
    bool movImg = false;
    Slider* sliderSel;
    Bitmap* imageSel = NULL;

    Bitmap* img;
    Bitmap* img2;
    Bitmap* img3;

    int screenWidth, screenHeight;
    int difX, difY;
    vector<Bitmap*> vBtm;
    vector<Slider*> sliders;
    vector<Button*> buttons;
    vector<Checkbox*> checkboxes;

    Slider* sliderT;
    Config();
    ~Config();

    // Function Canvas
    void InitCanvas();
    void Render();
    void renderAplication();

    // User Actions
    void Keyboard(int key);
    void keyboardUp(int key);
    void Mouse(int button, int state, int wheel, int direction, int mouseX, int mouseY);

    // Function Image
    void OrderBitmaps(int n);
    void isMovImage(int mouseX, int mouseY);
    void MoveImage(int mouseX, int mouseY, Bitmap* imageSel);
    void ChangeStateButtons(Bitmap* img);

    // Function Slider
    void isMovSliders(int mouseX, int mouseY);
    void ChangeValue(float indicador,Slider* sliderSel);
    void CreateSlides(Bitmap* img);

    // Function Buttons
    void isActivatedButton(int mouseX, int mouseY);
    void ActionButton(Button* btn);
    void CreateButtons(Bitmap* img);

    // Function Checkbox
    void isActivatedCheckbox(int mouseX, int mouseY);
    void UpdateCheckboxImage();
    void CreateCheckbox(Bitmap* img);

    // Others
    void Draw7SegmentDisplay(int num, int posX, int posY);
    void Relogio();
};

#endif
