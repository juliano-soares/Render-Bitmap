/**
    Config:
    In this file I instantiate all the necessary information
    to run the application and also manage what is shown and the
    actions the user takes.
    @file Config.cpp
    @author Juliano Leonardo Soares
    @version 1.1 03/05/22
*/

#include "Config.h"
#include "../Modules/gl_canvas2d.h"
#include "../Utils/Utils.h"
#include "../Components/Button/Button.h"
#include "../Components/Checkbox/Checkbox.h"
#include "../Components/Slider/Slider.h"
#include "../Components/Histogram/Histogram.h"
#include <iostream>
#include <algorithm>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <GL/glut.h>
#include <GL/freeglut_ext.h>

using namespace std;

Config::~Config() {}

/* Start the settings */
Config::Config()
{
    screenWidth = 1366;
    screenHeight = 720;

    img = new Bitmap(Utils::getImagePath("img4.bmp"));
    img2 = new Bitmap(Utils::getImagePath("img2.bmp"));
    img3 = new Bitmap(Utils::getImagePath("img3.bmp"));

    img->positionX = 350;
    img2->positionX = 575;
    img3->positionX = 800;
    img->positionY = 100;
    img2->positionY = 100;
    img3->positionY = 100;

    img->idImg = 0;
    img2->idImg = 1;
    img3->idImg = 2;

    img->isSelect = true;
    imageSel = img;
    vBtm.push_back(img3);
    vBtm.push_back(img2);
    vBtm.push_back(img);

    hist = new Histogram(img, 325, screenHeight, 300, 150);
    hist1 = new Histogram(img2, 325, screenHeight, 300, 150);
    hist2 = new Histogram(img3, 325, screenHeight, 300, 150);

    CreateSlides(img);
    CreateButtons(img);
    CreateCheckbox(img);
}

/* Renders/draws all components on the screen
 */
void Config::Render()
{

    renderAplication();
    histActive->Render();

    for (vector<Bitmap>::size_type i = 0; i != vBtm.size(); i++)
        vBtm[i]->RenderBitmap(vBtm[i]->positionX, vBtm[i]->positionY);

    for (vector<Slider>::size_type i = 0; i != sliders.size(); i++)
        sliders[i]->Render();

    for (vector<Button>::size_type i = 0; i != buttons.size(); i++)
        buttons[i]->Render();

    for (vector<Button>::size_type i = 0; i != checkboxes.size(); i++)
        checkboxes[i]->Render();

    if (idImgSel == 0)
    {
        if (isActivesHistograms)
            hist->Render();
    }
    else if (idImgSel == 1)
    {
        if (isActivesHistograms)
            hist1->Render();
    }
    else if (idImgSel == 2)
    {
        if (isActivesHistograms)
            hist2->Render();
    }

    Relogio();
}

/* Init Canvas
 */
void Config::InitCanvas()
{
    CV::init(&screenWidth, &screenHeight, "T1 - Bitmap Reader");
    rgb = Utils::RGBtoFloat(20, 29, 43); // Background-color
    CV::clear(rgb[0], rgb[1], rgb[2]);
    CV::run();
}

/* Writes all texts on the screen
 */
void Config::renderAplication()
{
    // menu side bar
    rgb = Utils::RGBtoFloat(38, 49, 67);
    CV::color(rgb[0], rgb[1], rgb[2]);
    CV::rectFill(0, 0, 300, screenHeight);

    // All texts
    rgb = Utils::RGBtoFloat(255, 250, 250);
    CV::color(rgb[0], rgb[1], rgb[2]);
    CV::text(20, 20, "Tamanho e Orientacao:");
    CV::text(20, 40, "Flip");
    CV::text(20, 100, "Rotacao");
    CV::text(20, 160, "Redimensionar");
    CV::text(20, 220, "Ajustes no Brilho e Cor:");
    CV::text(20, 240, "Brilho");
    CV::text(150, 240, "Contraste");
    CV::text(20, 300, "Cor:");
    CV::text(20, 360, "Efeitos:");

    // clock separator
    rgb = Utils::RGBtoFloat(255, 0, 0);
    CV::color(rgb[0], rgb[1], rgb[2]);
    CV::circleFill(122, 560, 2, 100);
    CV::circleFill(122, 570, 2, 100);
    CV::circleFill(172, 560, 2, 100);
    CV::circleFill(172, 570, 2, 100);
}

//**********************************************************
//                                                        //
// Fun��es responsaveis pelas a��es do usu�rio            //
//                                                        //
//**********************************************************

/* Function that receives keyboard input
   @params key: key value.
*/
void Config::Keyboard(int key)
{
    opcao = key;
    switch (key)
    {
    case 27:
        exit(0);
        break;
    }
};

/* Function that checks if the key has been released
   @params key: key value.
*/
void Config::keyboardUp(int key){};

/* Function that checks mouse states
   @params button: button id.
   @params state: button state.
   @params wheel: mouse scroll.
   @params direction: mouse scroll direction.
   @params mouseX: mouse coordinate on the X axis.
   @params mouseY: mouse coordinate on the Y axis.
*/
void Config::Mouse(int button, int state, int wheel, int direction, int mouseX, int mouseY)
{
    // checks the slider states and moves it
    if (mouseX != 0 && movSlider && imageSel != NULL)
    {
        float indicador = sliderSel->MoveIndicator(mouseX);
        ChangeValue(indicador, sliderSel);
    }

    // checks the states of the images and moves it
    if ((mouseX != 0 || mouseY != 0) && movImg && imageSel != NULL)
    {
        MoveImage(mouseX, mouseY, imageSel);
    }

    // checks if you clicked on something on the screen
    if (state == 0 && imageSel != NULL)
    {
        isMovSliders(mouseX, mouseY);
        isMovImage(mouseX, mouseY);
        isActivatedButton(mouseX, mouseY);
        isActivatedCheckbox(mouseX, mouseY);

        if (histActive->Collided(mouseX, mouseY))
        {
            isActivesHistograms = isActivesHistograms ? false : true;
            histActive->label = isActivesHistograms ? "Close Histogram" : "Open Histogram";
        }
    }

    // stop slider movement
    if (movSlider && state == 1)
    {
        movSlider = false;
    }

    // stop image movement
    if (movImg && state == 1)
    {
        movImg = false;
        cliqueY = 0;
        cliqueX = 0;
    }
    for (vector<Button>::size_type i = 0; i != buttons.size(); i++)
    {
        buttons[i]->Hover(mouseX, mouseY);
    }
    histActive->Hover(mouseX, mouseY);
}

//**********************************************************
// Fun��es responsaveis pelas imagens                     //
//**********************************************************

/* Function responsible for ordering the position of images.
   @params n: new first image.
*/
void Config::OrderBitmaps(int n)
{
    vector<Bitmap *> y;
    y.push_back(vBtm[n]);
    for (int i = vBtm.size() - 1; i >= 0; i--)
    {
        if (i != n)
            y.push_back(vBtm[i]);
    }
    vBtm.clear();
    for (int i = y.size() - 1; i >= 0; i--)
    {
        vBtm.push_back(y[i]);
    }
    ChangeStateButtons(imageSel);
}

/* function responsible for checking if the image was selected and reordering the images.
   @param mouseX: X position in the mouse Cartesian.
   @param mouseY: Y position in the mouse Cartesian.
*/
void Config::isMovImage(int mouseX, int mouseY)
{
    for (int i = 2; i >= 0; i--)
    {
        if (vBtm[i]->Collided(mouseX, mouseY))
        {
            if (!vBtm[i]->isSelect)
            {
                for (vector<Bitmap>::size_type i = 0; i != vBtm.size(); i++)
                    vBtm[i]->isSelect = false;
                idImgSel = vBtm[i]->idImg;
                vBtm[i]->isSelect = true;
                imageSel = vBtm[i];
                OrderBitmaps(i);
            }
            else
            {
                movImg = true;
            }
            cliqueY = mouseY - imageSel->positionY;
            cliqueX = mouseX - imageSel->positionX;
            return;
        }
    }
}

/* Function responsible for moving the image.
   @param mouseX: X position in the mouse Cartesian.
   @param mouseY: Y position in the mouse Cartesian.
*/
void Config::MoveImage(int mouseX, int mouseY, Bitmap *imageSel)
{
    imageSel->positionX = mouseX - cliqueX;
    imageSel->positionY = mouseY - cliqueY;

    if (imageSel->positionX <= 310)
        imageSel->positionX = 310;
    if (imageSel->positionY <= 10)
        imageSel->positionY = 10;
    if (imageSel->positionX >= screenWidth - 20)
        imageSel->positionX = screenWidth - 20;
    if (imageSel->positionY >= screenHeight - 20)
        imageSel->positionY = screenHeight - 20;
}

//**********************************************************
//                                                        //
// Fun��es responsaveis pelos Sliders                     //
//                                                        //
//**********************************************************

/* Function responsible for moving the slide.
   @param mouseX: X position in the mouse Cartesian.
   @param mouseY: Y position in the mouse Cartesian.
*/
void Config::isMovSliders(int mouseX, int mouseY)
{
    for (vector<Bitmap>::size_type i = 0; i != sliders.size(); i++)
    {
        if (sliders[i]->Collided(mouseX, mouseY))
        {
            movSlider = true;
            sliderSel = sliders[i];
        }
    }
}

/* Function responsible for changing the slider values.
   @param indicador: slider value.
   @param sliderSel: selected slider instance.
*/
void Config::ChangeValue(float indicador, Slider *sliderSel)
{
    if (sliderSel->id == 1) // ID = Brightness
    {
        imageSel->Brightness(indicador);
    }
    if (sliderSel->id == 2) // ID = Contrast
    {
        imageSel->Contrast(indicador);
    }
}

/* Function responsible for creating the sliders
   @param img: object instance to be manipulated.
*/
void Config::CreateSlides(Bitmap *img)
{
    vector<float> bg = Utils::RGBtoFloat(255, 250, 250);
    // Brightness
    sliders.push_back(new Slider(1, &img, 30, 265, 100, 2, bg, 1, 3, 1));
    // Contrast
    sliders.push_back(new Slider(2, &img, 150, 265, 100, 2, bg, 1, 100, 1));
}

//**********************************************************
//                                                        //
// Fun��es responsaveis pelos Bot�es                      //
//                                                        //
//**********************************************************

/* Function responsible for checking if the button was pressed.
   @param mouseX: X position in the mouse Cartesian.
   @param mouseY: Y position in the mouse Cartesian.
*/
void Config::isActivatedButton(int mouseX, int mouseY)
{
    for (vector<Button>::size_type i = 0; i != buttons.size(); i++)
    {
        if (buttons[i]->Collided(mouseX, mouseY))
        {
            ActionButton(buttons[i]);
        }
    }
}

/* Function responsible for triggering the action of the button.
   @param btn: object instance of button to be manipulated.
*/
void Config::ActionButton(Button *btn)
{
    if (btn->id == 1) // ID = Flip Vertical
        imageSel->MirrorImage(0);
    if (btn->id == 2) // ID = Flip Horizontal
        imageSel->MirrorImage(1);

    if (btn->id == 3) // ID = Rotate Left
        imageSel->RotateImage(1);
    if (btn->id == 4) // ID = Rotate Right
        imageSel->RotateImage(0);

    if (btn->id == 5) // ID = scale 1/2
        imageSel->scale(0);
    if (btn->id == 6) // ID = scale 1/4
        imageSel->scale(1);
    if (btn->id == 7) // ID = scale 1/6
        imageSel->scale(2);
    if (btn->id == 8) // ID = scale 1/8
        imageSel->scale(3);
    if (btn->id == 9) // ID = scale 1/10
        imageSel->scale(4);

    if (btn->id == 10)
    { // ID = Effect Binary
        imageSel->Binary(imageSel->imgData);
        imageSel->efBinary++;
    }
    if (btn->id == 11)
    { // ID = Effect Blur
        imageSel->Blur(imageSel->imgData);
        imageSel->efBlur++;
    }
    if (btn->id == 12)
    { // ID = Effect Monochrome
        imageSel->Monochrome(imageSel->imgData);
        imageSel->efMono++;
    }
    if (btn->id == 13)
    { // ID = Effect Sepia
        imageSel->Sepia(imageSel->imgData);
        imageSel->efSepia++;
    }
    if (btn->id == 14) // ID = Save
        imageSel->Write();
    if (btn->id == 15)
    { // ID = Reset
        imageSel->Reset();
        for (vector<Bitmap>::size_type i = 0; i != sliders.size(); i++)
            sliders[i]->ResetPosition();
        for (vector<Button>::size_type i = 0; i != checkboxes.size(); i++)
            checkboxes[i]->isChecked = false;
        UpdateCheckboxImage();
    }
}

/* Function responsible for creating the buttons.
   @param img: object instance to be manipulated.
*/
void Config::CreateButtons(Bitmap *img)
{
    // Cores dos botoes
    vector<float> bg = Utils::RGBtoFloat(26, 35, 39);
    vector<float> labelColor = Utils::RGBtoFloat(255, 255, 255);

    // Flip
    buttons.push_back(new Button(&img, 1, 20, 50, 120, 30, bg, "Vertical", labelColor));
    buttons.push_back(new Button(&img, 2, 150, 50, 120, 30, bg, "Horizontal", labelColor));

    // Rotacao
    buttons.push_back(new Button(&img, 3, 20, 110, 120, 30, bg, "Esquerda", labelColor));
    buttons.push_back(new Button(&img, 4, 150, 110, 120, 30, bg, "Direita", labelColor));

    // redimensionar
    buttons.push_back(new Button(&img, 5, 20, 170, 40, 30, bg, "1/2", labelColor));
    buttons.push_back(new Button(&img, 6, 65, 170, 40, 30, bg, "1/4", labelColor));
    buttons.push_back(new Button(&img, 7, 110, 170, 40, 30, bg, "1/6", labelColor));
    buttons.push_back(new Button(&img, 8, 155, 170, 40, 30, bg, "1/8", labelColor));
    buttons.push_back(new Button(&img, 9, 200, 170, 40, 30, bg, "1/10", labelColor));

    // Others
    buttons.push_back(new Button(&img, 10, 20, 380, 250, 30, bg, "Efeito Binario", labelColor));
    buttons.push_back(new Button(&img, 11, 20, 420, 250, 30, bg, "Efeito Borrar", labelColor));
    buttons.push_back(new Button(&img, 12, 20, 460, 250, 30, bg, "Monocromatico", labelColor));
    buttons.push_back(new Button(&img, 13, 20, 500, 250, 30, bg, "Efeito Sepia", labelColor));

    // Salvar e resetar
    buttons.push_back(new Button(&img, 14, 20, 650, 120, 30, bg, "Salvar", labelColor));
    buttons.push_back(new Button(&img, 15, 150, 650, 120, 30, bg, "Resetar", labelColor));

    histActive = new Button(&img, 16, 20, 600, 250, 30, Utils::RGBtoFloat(26, 35, 39), "Open Histogram", Utils::RGBtoFloat(255, 255, 255));
}

//**********************************************************
//                                                        //
// Fun��es responsaveis pelos Checkbox                    //
//                                                        //
//**********************************************************

/* Function responsible for checking if the checkbox was pressed.
   @param mouseX: X position in the mouse Cartesian.
   @param mouseY: Y position in the mouse Cartesian.
*/
void Config::isActivatedCheckbox(int mouseX, int mouseY)
{
    for (vector<Button>::size_type i = 0; i != checkboxes.size(); i++)
    {
        if (checkboxes[i]->Collided(mouseX, mouseY))
        {
            if (checkboxes[i]->isChecked)
            {
                checkboxes[i]->isChecked = false;
                imageSel->btmCheckbox[i] = false;
            }
            else
            {
                checkboxes[i]->isChecked = true;
                imageSel->btmCheckbox[i] = true;
            }
            UpdateCheckboxImage();
        }
    }
}

/* Function responsible for updating checkboxes.
 */
void Config::UpdateCheckboxImage()
{
    if (!checkboxes[0]->isChecked && !checkboxes[1]->isChecked && !checkboxes[2]->isChecked)
    {
        imageSel->channel = {1, 1, 1};
    }
    else
    {
        if (checkboxes[0]->isChecked)
            imageSel->channel[0] = 1;
        else
            imageSel->channel[0] = 0;
        if (checkboxes[1]->isChecked)
            imageSel->channel[1] = 1;
        else
            imageSel->channel[1] = 0;
        if (checkboxes[2]->isChecked)
            imageSel->channel[2] = 1;
        else
            imageSel->channel[2] = 0;
    }
}

/* Function responsible for creating the checkboxes
   @param img: object instance to be manipulated.
*/
void Config::CreateCheckbox(Bitmap *img)
{
    vector<float> labelColor = Utils::RGBtoFloat(255, 250, 250);
    checkboxes.push_back(new Checkbox(&img, 20, 310, 20, 20, false, "RED", Utils::RGBtoFloat(255, 99, 71), Utils::RGBtoFloat(255, 250, 250)));
    checkboxes.push_back(new Checkbox(&img, 100, 310, 20, 20, false, "GREEN", Utils::RGBtoFloat(50, 205, 50), Utils::RGBtoFloat(255, 250, 250)));
    checkboxes.push_back(new Checkbox(&img, 185, 310, 20, 20, false, "BLUE", Utils::RGBtoFloat(30, 144, 255), Utils::RGBtoFloat(255, 250, 250)));
}

void Config::ChangeStateButtons(Bitmap *img)
{
    for (vector<Button>::size_type i = 0; i != checkboxes.size(); i++)
    {
        checkboxes[i]->isChecked = img->btmCheckbox[i];
    }
}

//**********************************************************
//                                                        //
// Fun��es responsaveis pelo relogio                      //
//                                                        //
//**********************************************************

/* Function responsible for getting the pc time and designing the clock.
 */
void Config::Relogio()
{
    struct tm *data_hora_atual;
    time_t t = time(NULL);
    data_hora_atual = localtime(&t);

    int hora = data_hora_atual->tm_hour;
    int min = data_hora_atual->tm_min;
    int sec = data_hora_atual->tm_sec;
    vector<float> rgb = Utils::RGBtoFloat(255, 0, 0);
    CV::color(rgb[0], rgb[1], rgb[2]);

    Draw7SegmentDisplay(hora / 10, 80, 550);
    Draw7SegmentDisplay(hora % 10, 100, 550);
    Draw7SegmentDisplay(min / 10, 130, 550);
    Draw7SegmentDisplay(min % 10, 150, 550);
    Draw7SegmentDisplay(sec / 10, 180, 550);
    Draw7SegmentDisplay(sec % 10, 200, 550);
}

/* Function responsible for designing the 7-segment display.
   @param num: number to be drawn.
   @param posX: x position to start drawing.
   @param posY: y position to start drawing.
*/
void Config::Draw7SegmentDisplay(int num, int posX, int posY)
{
    float tam = 2;
    int gap = 2;
    int halgap = gap * 0.5;
    int side = tam * 6 + halgap;
    float x_h[6] = {0, tam, tam * 5, tam * 6, tam * 5, tam};
    float y_h[6] = {0, tam, tam, 0, -tam, -tam};

    float x_v[6] = {0, -tam, -tam, 0, tam, tam};
    float y_v[6] = {0, tam, tam * 5, tam * 6, tam * 5, tam};

    int posicaoX = posX;
    int posicaoY = posY;
    vector<float> rgb = Utils::RGBtoFloat(255, 0, 0); // Background-color
    CV::color(rgb[0], rgb[1], rgb[2]);
    //                 0     1     2     3     4     5     6     7     8     9
    int arrBits[] = {0x77, 0x30, 0x6D, 0x79, 0x3A, 0x5B, 0x5F, 0x70, 0x7F, 0x7B};

    if ((arrBits[num] >> 6 & 1) == 1)
    {
        CV::translate(posicaoX, posicaoY); // segmento A
        CV::polygonFill(x_h, y_h, 6);
    }
    if ((arrBits[num] >> 5 & 1) == 1)
    {
        CV::translate(posicaoX + side, posicaoY + halgap); // segmento B
        CV::polygonFill(x_v, y_v, 6);
    }
    if ((arrBits[num] >> 4 & 1) == 1)
    {
        CV::translate(posicaoX + side, posicaoY + side + gap); // segmento C
        CV::polygonFill(x_v, y_v, 6);
    }
    if ((arrBits[num] >> 3 & 1) == 1)
    {
        CV::translate(posicaoX, posicaoY + side + halgap); // segmento G
        CV::polygonFill(x_h, y_h, 6);
    }

    if ((arrBits[num] >> 2 & 1) == 1)
    {
        CV::translate(posicaoX, posicaoY + side + gap); // Segmento E
        CV::polygonFill(x_v, y_v, 6);
    }
    if ((arrBits[num] >> 1 & 1) == 1)
    {
        CV::translate(posicaoX, posicaoY + halgap); // segmento F
        CV::polygonFill(x_v, y_v, 6);
    }
    if ((arrBits[num] >> 0 & 1) == 1)
    {
        CV::translate(posicaoX, posicaoY + 2 * side + gap); // segmento D
        CV::polygonFill(x_h, y_h, 6);
    }
}
