#ifndef ITEM_H_
#define ITEM_H_

#define BIGBALL 1
#define EXPAND 2
#define TRIPLE 3

const float ITEM_SPEED = 1.5;


#include "Object.h"
#include <stdlib.h>
#include <time.h>
#include <iostream>

class Item: public Object {
public:
    Item(SDL_Renderer* renderer);
    ~Item();

    void setSpeed(const int& speed) { this->speed = speed; }
    void Update(float delta);
    void Render(float delta);
    int itemChosen;

private:
    SDL_Texture* texture[3];
    int speed;
};

#endif


