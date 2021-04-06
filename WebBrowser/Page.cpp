#include "Page.h"

Page::Page(sf::RenderWindow& window)
{
	width = window.getSize().x;
	height = window.getSize().y;
}
