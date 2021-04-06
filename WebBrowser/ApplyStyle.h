#pragma once
#pragma once
#include <memory>
#include "HTMLElement.h"
#include "HTMLRenderElement.h"
#include "SFMLHTMLTextRender.h"
#include "SFMLHTMLImageRender.h"
#include "FontWeight.h"
#include "StyleSheet.h"

class HTMLStyleAssociation
{
	StyleSheet& styleSheet;
	HTMLStyleAssociation(StyleSheet& styleSheet);


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
		

		auto tagStyle = styleSheet.getStyleForTag(element.getTag());
		if (tagStyle) {
			for (auto& val : tagStyle.value().get()) {
				style.insert_or_assign(val.first, val.second);
			}
		}



		for (auto& val : element.getStyle()) {
			style.insert_or_assign(val.first, val.second);
		}

		for (auto& s : style) {
			element.assignStyleRef(s.first, s.second);
		}

		for (auto& child : element.getChildren()) {
			create(child, style, parent);
		}
	}

	std::unique_ptr<		HTMLRenderElement>
		create(HTML_Element& element, StyleRef style) {
		auto parent = std::make_unique<HTMLRenderElement>();
		auto render = HTMLRenderElement::Parent(parent.get());
		for (auto& val : element.getStyle()) {
			style.insert_or_assign(val.first, val.second);
		}

		for (auto& prop : element.getProperties())
			render.value()->addProperties(prop.first, prop.second);

		for (auto& s : style)
			render.value()->addStyles(s.first, s.second);

		auto children = create(element, style, render);
		parent->addChildren(children);
		return parent;
	}


};



