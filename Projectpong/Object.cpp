#include "Object.h"

SDL_Renderer* Object::renderer = nullptr;

Object::Object() {
    x = 0;
    y = 0;
    width = 1;
    height = 1;
}

Object::~Object() {
}

void Object::Update(float delta) {
}

void Object::Render(float delta) {
}

bool Object::IsCollides(Object* other) {
    if (x + width > other->x && x < other->x + other->width &&
        y + height > other->y && y < other->y + other->height) {
        return true;
    }
    return false;
}

