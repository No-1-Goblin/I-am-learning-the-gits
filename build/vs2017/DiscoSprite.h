#pragma once

#include <graphics/sprite.h>
#include <input/input_manager.h>

class DiscoSprite :
    public gef::Sprite
{
public:
    void update(float frame_time, gef::InputManager* input_manager_);
protected:
    Int32 active_touch_id_;
	gef::Vector2 touch_position_;
	gef::Vector2 new_touch_position_;
	gef::Vector2 release_position_;
	gef::Vector2 movementDirection;
	int movementFrames = 0;
};

