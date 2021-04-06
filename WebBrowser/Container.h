#pragma once
#define SFML_STATIC
#include "Entity.h"
#include "DOMText.h"
#include <SFML/Graphics.hpp>

class Container : public Entity
{

	void render();
};

