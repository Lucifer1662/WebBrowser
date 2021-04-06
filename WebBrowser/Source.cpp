#define SFML_STATIC
#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include "Text.h"
#include "Page.h"
#include "HTMLParser.h"
#include <variant>
#include "HTMLToRenderGraph.h"
#include "Connection.h"

const int WIDTH = 800;
const int HEIGHT = 800;


std::string testHtml = R"(


<HTML>

<HEAD>

<TITLE>Your Title Here</TITLE>

</HEAD>

<BODY BGCOLOR="FFFFFF">


<HR>

<a href="http://somegreatsite.com">Link Name</a>

is a link to another nifty site

<H1>This is a Header</H1>

<H2>This is a Medium Header</H2>

Send me mail at <a href="mailto:support@yourcompany.com">

support@yourcompany.com</a>

<IMG src="test.png">

<P> This is a new paragraph! </P>

<P> <B>This is a new paragraph in bold!</B> </P>

<BR> <B><I>This is a new sentence without a paragraph break, in bold italics.</I></B>

<HR>

</BODY>

</HTML>
)";


void printNodeTree(Node& node, int depth = 0) {
	for (int i = 0; i < depth; i++) {
		std::cout << '\t';
	}
	std::cout << node.value << std::endl;
	for (auto& child : node.children) {
		printNodeTree(child,depth+1);
	}
}

int main()
{
	auto [header, htmlString] = connection();

	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Web Browser");
	Page page(window);

	auto document = parseDocument(std::string_view(htmlString));
	printNodeTree(document);
	auto html = toHTML(document);	
	
	auto creator = HTMLToRenderGraphCreator(window);
	Property_Variant defaultFontSize = (Dimension)16;
	Property_Variant defaultWidth = (Dimension)(Pixels)window.getSize().x;

	auto style = StyleRef();
	style.insert_or_assign(Style_Name::fontSize, defaultFontSize);
	style.insert_or_assign(Style_Name::width, defaultWidth);

	auto graph = creator.create(html, style);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
	
		window.clear(sf::Color(255,255,255,255));
		//draw
		graph->render();
		//end draw

		window.display();
	}

	return 0;
}