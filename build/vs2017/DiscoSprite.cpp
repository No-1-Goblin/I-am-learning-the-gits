#include "DiscoSprite.h"
#include <graphics/colour.h>
#include <random>

void DiscoSprite::update(float frame_time) {
	set_colour(gef::Colour((rand() % 1000) / 1000.f, (rand() % 1000) / 1000.f, (rand() % 1000) / 1000.f).GetABGR());
}