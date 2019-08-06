#ifndef Object_H_
#define Object_H_

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

class Object {
public:
    Object(SDL_Renderer* renderer);
    virtual ~Object();

    float x, y, width, height;

    virtual void Update(float delta);
    virtual void Render(float delta);

    bool IsCollides(Object* other);
protected:
    SDL_Renderer* renderer;

};

#endif
