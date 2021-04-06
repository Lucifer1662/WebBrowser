#pragma once
#include <memory>
#include "HTMLElement.h"
#include "HTMLRenderElement.h"
#include "SFMLHTMLTextRender.h"
#include "SFMLHTMLImageRender.h"
#include "FontWeight.h"

struct HTMLToRenderGraphCreator
{
	sf::RenderWindow& window;

	HTMLToRenderGraphCreator(sf::RenderWindow& window) :window(window) {}

	using HTMLPropertyStack = std::unordered_map<Property_Name, Property_Variant*>;


	std::vector<std::unique_ptr<HTMLRenderElement>>
		create(HTML_Element& element, StyleRef style,
			HTMLRenderElement::Parent& parent) {

		auto displayIt = element.getStyle().find(Style_Name::display);
		if (displayIt != element.getStyle().end()) {
			switch (std::get<Property<Display>>(displayIt->second).get()) {
			case Display::inline_block:
				style.erase(Style_Name::width);
				style.erase(Style_Name::height);
				break;
			}
		}

		for (auto& val : element.getStyle()) {
			style.insert_or_assign(val.first, val.second);
		}

		std::vector<std::unique_ptr<HTMLRenderElement>> newChildren;

		switch (element.getTag()) {

		case Tag_Name::text:
		{
			newChildren.push_back(std::make_unique<SFMLHTMLTextRender>(window, parent));
			break;
		}
		case Tag_Name::img:
		{
			newChildren.push_back(std::make_unique<SFMLHTMLImageRender>(window, parent));
			break;
		}
		default:
			std::vector<std::unique_ptr<HTMLRenderElement>> children;
			for (auto& child : element.getChildren()) {
				auto newChildren = create(child, style, parent);
				for (auto& newChild : newChildren) {
					children.push_back(std::move(newChild));
				}
			}
			return children;
		}

		std::unique_ptr<HTMLRenderElement>& text = newChildren.front();


		for (auto& prop : element.getProperties())
			text->addProperties(prop.first, prop.second);

		for (auto& s : style)
			text->addStyles(s.first, s.second);

		for (auto& child : element.getChildren()) {
			auto s = HTMLRenderElement::Parent(text.get());
			auto newChildren = create(child, style, s);
			text->addChildren(newChildren);
		}

		return newChildren;		
	}

	std::unique_ptr<HTMLRenderElement>
		create(HTML_Element& element, StyleRef style) {
		auto parent = std::make_unique<HTMLRenderElement>();
		auto render = HTMLRenderElement::Parent(parent.get());
		for (auto& val : element.getStyle()) {
			style.insert_or_assign(val.first, val.second);
		}

		for (auto& prop : element.getProperties())
			render.value()->addProperties(prop.first, prop.second);

		for (auto& s :  style)
			render.value()->addStyles(s.first, s.second);

		auto children = create(element, style, render);
		parent->addChildren(children);
		return parent;
	}


};

