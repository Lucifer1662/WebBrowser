//#include "Text.h"
//
//Text::Text(sf::RenderWindow& window, DOMText& domText, ParentEntity parent): 
//	window(window), domText(domText){
//	this->parent = parent;
//
//	
//
//	if (!font.loadFromFile("C://Windows/Fonts/arial.ttf"))
//	{
//		int k = 90;
//	}
//
//	textRender.setFont(font);
//
//	auto fontSizeSub = [&](const Dimension& old, const Dimension& current) {
//		changed = true;
//		textRender.setCharacterSize(toPixels(current));
//	};
//	fontSizeSub(domText.fontSize, domText.fontSize);
//	domText.fontSize.subscribe(fontSizeSub);
//
//	auto widthSub = [&](const Dimension& old, const Dimension& current) {
//		width = toPixels(current);
//		
//		changed = true;
//		setWrapText();
//	};
//	widthSub(domText.width, domText.width);
//	domText.width.subscribe(widthSub);
//
//	auto bodySub = [&](const std::string& old, const std::string& current) {
//		changed = true;
//		textRender.setString(current);
//		setWrapText();
//	};
//	bodySub(domText.body, domText.body);
//	domText.body.subscribe(bodySub);
//
//
//	// set the color
//	textRender.setFillColor(sf::Color::Red);
//
//	// set the text style
//	//textRender.setStyle(sf::Text::Bold | sf::Text::Underlined);
//}
//
//
//void Text::render(){	
//	window.draw(textRender);
//}
//
//Text::~Text()
//{
//	fontSizeUnSubscribe();
//}
//
//void Text::setWrapText()
//{
//	std::string wrapText;
//	wrapText.reserve(domText.body.get().size());
//	/*int char_width = textRender.getCharacterSize();
//	int char_per_line = width / char_width;*/
//	int line_size = 0;
//
//	int start = 0;
//	for (int i = 0; i < domText.body.get().size(); i++) {
//		int advance = textRender.getFont()->getGlyph((sf::Uint32)domText.body.get()[i], toPixels(domText.fontSize), false).advance;
//		if (advance + line_size > width || i+1 == domText.body.get().size()) {
//			int end = i;
//			wrapText.append(domText.body, start, end-start);
//			wrapText.append("\n");
//			start = end;
//			line_size = 0;
//		}
//		line_size += advance;
//		
//	}
//	/*for (int i = 0; i < domText.body.get().size(); ) {
//		int end = std::min(i + char_per_line, (int)domText.body.get().size());
//		wrapText.append(domText.body, i, end-i);
//		wrapText.append("\n");
//		i = end;
//	}*/
//	textRender.setString(wrapText);
//}
//
//int Text::toPixels(Dimension unit)
//{
//	int pixels = 0;
//
//	switch (unit.index())
//	{
//	case 0:
//		pixels = std::get<0>(unit);
//		break;
//	case 1:
//		Percetage percentage = std::get<1>(unit);
//		Pixels parent_width = parent.value().get().width;
//		pixels = percentage/100.0f * parent_width;
//		break;
//	}
//	return pixels;
//}
