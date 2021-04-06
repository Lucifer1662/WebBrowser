#pragma once
#include "HTMLImageRender.h"
#include <SFML/Graphics.hpp>
#include "Dimension.h"

class SFMLHTMLImageRender : public HTMLImageRender
{
	sf::Texture texture;
	sf::Sprite sprite;
	std::optional<std::string> source;
	sf::RenderWindow& window;
	bool usingTextureWidth = true;
	bool usingTextureHeight = true;

public:
	SFMLHTMLImageRender(sf::RenderWindow& window, Parent& parent) : HTMLImageRender(parent), window(window), sprite(texture) {
		sprite.setTexture(texture);
	}

	void render() {
		window.draw(sprite);
	}

	void setSource(const std::string& newSource) {
		if (source && source.value() != newSource || !source) {
			source = newSource;
			texture.loadFromFile(source.value());
			setWidthImplicit(texture.getSize().x);
			int y = texture.getSize().y;
			setHeightImplicit(texture.getSize().y);
		}
	}

	void setWidth(Dimension width) {
		usingTextureWidth = false;
		HTMLImageRender::setWidth(width);
		updateTextureRect();
	}

	void setWidthImplicit(Pixels width) {
		bool old = usingTextureWidth;
		if(usingTextureWidth)
			setWidth(width);
		usingTextureWidth = old;
	}


	void setHeight(Dimension height) {
		usingTextureHeight = false;
		HTMLImageRender::setHeight(height);
		updateTextureRect();
	}

	void setHeightImplicit(Pixels height) {
		bool old = usingTextureHeight;
		if(usingTextureHeight)
			setHeight(height);
		usingTextureHeight = old;
	}

	void updateTextureRect() {
		sprite.setTextureRect(sf::IntRect(
			0, 0, getWidth().pixels, getHeight().pixels
		));
	}

	void setY(Pixels y) {
		HTMLImageRender::setY(y);
		sprite.setPosition(x, y);
	}

	void setX(Pixels x) {
		HTMLImageRender::setX(x);
		sprite.setPosition(x, y);
	}
};

