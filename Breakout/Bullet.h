#ifndef BULLET_H_
#define BULLET_H_


#include "Object.h"
#include <stdlib.h>
#include <time.h>
#include <iostream>

class Bullet: public Object {
public:
    Bullet(SDL_Renderer* renderer);
    ~Bullet();

    void Update(float delta);
    void Render(float delta);

private:
    SDL_Texture* texture;
};

#endif


