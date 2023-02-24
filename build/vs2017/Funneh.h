#pragma once
#include <graphics/sprite.h>
#include <input/input_manager.h>
#include <input/touch_input_manager.h>
#include <graphics/colour.h>

class Funneh:
    public gef::Sprite
{
private:
    gef::InputManager * inputMan;

Int32 active_touch_id_;
gef::Vector2 touch_position_;
gef::Vector2 new_touch_position_;
gef::Vector2 release_position_;
gef::Vector2 movementDirection;
int movementFrames = 0;
public:
    Funneh();
    void Initialize(gef::InputManager* nInput) {

    }
    void Update(float dt) {

    }

};

