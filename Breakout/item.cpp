#include "Item.h"

SDL_Texture* Item::texture[3] = {NULL, NULL, NULL};
void Item::loadTexture()
{
    SDL_Surface* surface = IMG_Load("PNG//bigball.png");
    texture[0] = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("PNG//expand.png");
    texture[1] = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("PNG//triple.png");
    texture[2] = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

}
Item::Item(SDL_Renderer* renderer) {
    itemChosen = rand()%3;

    width = 40;
    height = 40;

    x = rand()%800 + 10;

}

Item::~Item() {
    //Clean resources
    SDL_DestroyTexture(texture[3]);
}


void Item::Update(float delta)
{
    y+= speed;
}


void Item::Render(float delta)
{
    SDL_Rect itemRect;
    itemRect.x = x;
    itemRect.y = y;
    itemRect.w = width;
    itemRect.h = height;

    SDL_RenderCopy(renderer, texture[itemChosen], 0, &itemRect);
}



