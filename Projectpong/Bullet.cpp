#include "Bullet.h"

Bullet::Bullet(SDL_Renderer* renderer) {

    SDL_Surface* surface = IMG_Load("PNG//bullet.png");
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);


    y = 610;
}

Bullet::~Bullet() {
    //Clean resources
    SDL_DestroyTexture(texture);
}


void Bullet::Update(float delta)
{
    y-= 1.2;
}

void Bullet::Render(float delta)
{
    SDL_Rect bulletRect;
    bulletRect.x = x;
    bulletRect.y = y;
    bulletRect.w = 15;
    bulletRect.h = 12;
    SDL_RenderCopy(renderer, texture, 0, &bulletRect);
}



