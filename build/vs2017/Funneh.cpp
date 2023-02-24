#include "Funneh.h"

Funneh::Funneh() {
	set_colour(gef::Colour(0, 0, 1).GetABGR());
	set_height(64);
	set_width(64);

}

void Funneh::Initialize(gef::InputManager* nInput) {
	inputMan = nInput;
}
void Funneh::Update(float dt) {

	const gef::TouchInputManager* touch_input = inputMan->touch_manager();
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
					set_colour(gef::Colour(1, 0, 0).GetABGR());
					set_position(gef::Vector4(new_touch_position_.x, new_touch_position_.y, 0, 0));
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

					set_position(gef::Vector4(touch->position.x,touch->position.y,0));

					touch_position_ = touch->position;
				}
				else if (touch->type == gef::TT_RELEASED)
				{
					// the touch we are tracking has been released
					// perform any actions that need to happen when a touch is released here
					// we're not doing anything here apart from resetting the active touch id
					release_position_ = touch->position;
					movementDirection /= 60.f;
					movementFrames = 60;
					active_touch_id_ = -1;
				}
			}
		}
	}

}