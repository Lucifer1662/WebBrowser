#pragma once
#include "Property.h"
#include "Dimension.h"
#include <vector>
#include <string>
#include <set>
#include <optional>
#include "Style.h"

enum class Property_Name {
	width,
	height,
	body,
	source
};

enum class Tag_Name {
	p,
	b,
	h1,
	h2,
	h3,
	h4,
	h5,
	h6,
	div,
	img,
	i,
	body,
	html,
	head,
	text,
	a,
	title
};

class HTML_Element
{
	using Parent = std::optional<std::reference_wrapper<HTML_Element>>;
	Parent parent;
	std::vector<HTML_Element> children;
	std::unordered_map<Property_Name, Property_Variant> props;
	Style style;
	StyleRef styleRef;

	Tag_Name tag_name;
public:
	HTML_Element(Tag_Name name) : tag_name(name) {}

	void addChild(const HTML_Element& child) {
		children.push_back(child);
	}

	template<typename H>
	void assignProperty(Property_Name name, const H& val) {
		auto it = props.find(name);
		if (it != props.end()) {
			std::get<Property<H>>(it->second).set(val);
		}
		else {
			props.emplace(name, val);
		}
	}

	template<typename H>
	void assignStyle (Style_Name name, const H& val) {
		auto it = style.find(name);
		if (it != style.end()) {
			std::get<Property<H>>(it->second).set(val);
		}
		else {
			style.emplace(name, val);
		}
	}

	template<typename H>
	void assignStyleRef(Style_Name name, const H& val) {
		auto it = styleRef.find(name);
		if (it != styleRef.end()) {
			std::get<Property<H>>(it->second).set(val);
		}
		else {
			styleRef.emplace(name, val);
		}
	}

	template<typename T>
	std::optional<Property<T>> get(Property_Name name) const {
		auto it = props.find(name);
		if (it != props.end()) {
			return std::get<T>(*it);
		}
		return std::nullopt;
	}

	Tag_Name getTag() const { return tag_name; }

	auto& getChildren() {
		return children;
	}

	auto& getProperties() {
		return props;
	}

	Style& getStyle() {
		return style;
	}


	StyleRef& getStyleRef() {
		return styleRef;
	}
};

