#ifndef BALL_H_
#define BALL_H_

#include "Object.h"

#include <math.h>
#include <iostream>
//#include <iostream>
#include <stdio.h>
#include <string>
#include <fstream>
#include <vector>


// Define a ball speed in pixels per second
const float BALL_SPEED = 550;

class Ball: public Object {
public:
    Ball(SDL_Renderer* renderer);
    ~Ball();

    void Update(float delta);
    void Render(float delta);

    void SetDirection(float dirx, float diry);

    float dirx, diry;

    void bigBall();
    void normalBall();
    void setBallSpeed();

private:
    SDL_Texture* texture;
    float speed = BALL_SPEED;
    unsigned int lastTime = 0;
};

#endif
