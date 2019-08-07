#include "Bullet.h"

Bullet::Bullet(SDL_Renderer* renderer): Object(renderer) {

    SDL_Surface* surface = IMG_Load("PNG//bullet.png");
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);


}

Bullet::~Bullet() {
    //Clean resources
    SDL_DestroyTexture(texture);
}


void Bullet::Update(float delta)
{
    y-= 30;
}




