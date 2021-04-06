#pragma once
#include "HTMLTextRenderElement.h"
#include <SFML/Graphics.hpp>
#include "Property.h"

class SFMLHTMLTextRender : public HTMLTextElemement
{
	sf::Font font;
	Property<int>::Unsubscribe fontSizeUnSubscribe;
	sf::Text textRender;
	sf::RenderWindow& window;
	bool changed = false;

public:
	SFMLHTMLTextRender(sf::RenderWindow& window,
		HTMLRenderElement::Parent& parent) :
		window(window), HTMLTextElemement(parent) {


		if (!font.loadFromFile("C://Windows/Fonts/arial.ttf"))
		{
			int k = 90;
		}

		textRender.setFont(font);

		// set the color
		textRender.setFillColor(sf::Color::Black);

		// set the text style
		//textRender.setStyle(sf::Text::Bold | sf::Text::Underlined);
	}

	void render() {
		HTMLRenderElement::render();
		window.draw(textRender);
	}

	void setWrapText()
	{
		std::string wrapText;
		wrapText.reserve(getBody().size());
		int line_size = 0;

		int start = 0;
		int numberOfLines = 0;
		for (int i = 0; i < getBody().size(); i++) {

			int advance = textRender.getFont()->getGlyph((sf::Uint32)getBody()[i], getFontSize().pixels, false).advance;
			if (advance + line_size > width.pixels) {
				int end = i;
				wrapText.append(getBody(), start, end - start);
				wrapText.append("\n");
				start = end;
				line_size = 0;
				numberOfLines++;
			}
			if (i + 1 == getBody().size()) {
				int end = getBody().size();
				wrapText.append(getBody(), start, end - start);
				numberOfLines++;
			}
			line_size += advance;

		}
		textRender.setString(wrapText);
		height.pixels = numberOfLines * textRender.getLineSpacing() * textRender.getCharacterSize();
		setHeight(height.pixels);
	}

	void setY(Pixels y) {
		HTMLRenderElement::setY(y);
		textRender.setPosition(x, y);
	}

	void setX(Pixels x) {
		HTMLRenderElement::setX(x);
		textRender.setPosition(x, y);
	}

	void setWidth(Dimension width) {
		HTMLRenderElement::setWidth(width);
		setWrapText();
	}

	void setHeight(Dimension height) {
		HTMLRenderElement::setHeight(height);
	}

	void setFontSize(Dimension size) {
		HTMLRenderElement::setFontSize(size);
		textRender.setCharacterSize(fontSize.pixels);
	}

	void setBody(const std::string& body) {
		HTMLTextElemement::setBody(body);
		setWrapText();
	}

	void setFontWeight(FontWeight weight) {
		HTMLTextElemement::setFontWeight(weight);
		switch (weight){
		case FontWeight::bold:
			textRender.setStyle(sf::Text::Bold);
			break;
		case FontWeight::normal:
			textRender.setStyle(sf::Text::Regular);
			break;
		default:
			break;
		}
	}
};

