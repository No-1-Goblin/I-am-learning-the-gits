#include "input_app.h"
#include <system/platform.h>
#include <graphics/sprite_renderer.h>
#include <graphics/font.h>
#include <input/input_manager.h>
#include <input/touch_input_manager.h>
#include <system/debug_log.h>


bool isInside(const gef::Sprite& sprite, const gef::Vector2& point) {
	float spriteLeftBounds = sprite.position().x() - sprite.width() / 2;
	float spriteRightBounds = sprite.position().x() + sprite.width() / 2;
	float spriteTopBounds = sprite.position().y() + sprite.height() / 2;
	float spriteBottomBounds = sprite.position().y() - sprite.height() / 2;
	if (point.x > spriteLeftBounds && point.x < spriteRightBounds && point.y < spriteTopBounds && point.y > spriteBottomBounds) {
		return true;
	}
	else {
		return false;
	}
}

InputApp::InputApp(gef::Platform& platform) :
	Application(platform),
	sprite_renderer_(NULL),
	font_(NULL),
	input_manager_(NULL),
	active_touch_id_(-1)
{
	sprite.set_colour(gef::Colour(0, 1, 0).GetABGR());
	sprite.set_height(64);
	sprite.set_width(64);
}

void InputApp::Init()
{
	sprite_renderer_ = gef::SpriteRenderer::Create(platform_);
	input_manager_ = gef::InputManager::Create(platform_);

	// make sure if there is a panel to detect touch input, then activate it
	if (input_manager_ && input_manager_->touch_manager() && (input_manager_->touch_manager()->max_num_panels() > 0))
		input_manager_->touch_manager()->EnablePanel(0);

	InitFont();
}

void InputApp::CleanUp()
{
	CleanUpFont();

	delete input_manager_;
	input_manager_ = NULL;

	delete sprite_renderer_;
	sprite_renderer_ = NULL;
}

bool InputApp::Update(float frame_time)
{
	fps_ = 1.0f / frame_time;

	if (input_manager_)
	{
		input_manager_->Update();

		ProcessTouchInput();
	}
	
	/*if (movementFrames) {
		sprite.set_position(gef::Vector4(sprite.position().x() + movementDirection.x, sprite.position().y() + movementDirection.y, sprite.position().z(), sprite.position().w()));
		movementFrames--;
	}*/

	return true;
}

void InputApp::Render()
{
	sprite_renderer_->Begin();
	sprite_renderer_->DrawSprite(sprite);
	DrawHUD();
	sprite_renderer_->End();
}
void InputApp::InitFont()
{
	font_ = new gef::Font(platform_);
	font_->Load("comic_sans");
}

void InputApp::CleanUpFont()
{
	delete font_;
	font_ = NULL;
}

void InputApp::DrawHUD()
{
	if(font_)
	{
		// if a touch is active lets draw some text
		if (active_touch_id_ != -1)
		{
			font_->RenderText(
				sprite_renderer_,
				gef::Vector4(touch_position_.x, touch_position_.y, -0.9f),
				1.0f, 0xffffffff, gef::TJ_LEFT,
				"(%.1f, %.1f)",
				touch_position_.x, touch_position_.y);
		}

		// display frame rate
		font_->RenderText(sprite_renderer_, gef::Vector4(850.0f, 510.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "FPS: %.1f", fps_);
	}
}

void InputApp::ProcessTouchInput()
{
	const gef::TouchInputManager* touch_input = input_manager_->touch_manager();
	if (touch_input && (touch_input->max_num_panels() > 0))
	{
		// get the active touches for this panel
		const gef::TouchContainer& panel_touches = touch_input->touches(0);

		// go through the touches
		for (gef::ConstTouchIterator touch = panel_touches.begin(); touch != panel_touches.end(); ++touch)
		{
			// if active touch id is -1, then we are not currently processing a touch
			if (active_touch_id_ == -1)
			{
				// check for the start of a new touch
				if (touch->type == gef::TT_NEW)
				{
					active_touch_id_ = touch->id;

					// do any processing for a new touch here
					// we're just going to record the position of the touch
					new_touch_position_ = touch->position;
					sprite.set_colour(gef::Colour(1, 0, 0).GetABGR());
					sprite.set_position(gef::Vector4(new_touch_position_.x, new_touch_position_.y, 0, 0));
					touch_position_ = touch->position;
				}
			}
			else if (active_touch_id_ == touch->id)
			{
				// we are processing touch data with a matching id to the one we are looking for
				if (touch->type == gef::TT_ACTIVE)
				{
					// update an active touch here
					// we're just going to record the position of the touch
					touch_position_ = touch->position;
					if (isInside(sprite, touch_position_)) {
						sprite.set_position(gef::Vector4(new_touch_position_.x, new_touch_position_.y, 0, 0));
					}
				}
				else if (touch->type == gef::TT_RELEASED)
				{
					// the touch we are tracking has been released
					// perform any actions that need to happen when a touch is released here
					// we're not doing anything here apart from resetting the active touch id
					release_position_ = touch->position;
					sprite.set_colour(gef::Colour(0, 1, 0).GetABGR());
					movementDirection = gef::Vector2(release_position_ - new_touch_position_);
					movementDirection /= 60.f;
					movementFrames = 60;
					active_touch_id_ = -1;
				}
			}
		}
	}
}
