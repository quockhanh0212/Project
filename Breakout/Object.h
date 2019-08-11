#ifndef Object_H_
#define Object_H_

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

class Object {
public:
    Object();
    virtual ~Object();

    float x, y, width, height;

    virtual void Update(float delta);
    virtual void Render(float delta);

    bool IsCollides(Object* other);

    static void setRenderer(SDL_Renderer* _renderer) { renderer = _renderer; }
protected:
    static SDL_Renderer* renderer;

};

#endif
