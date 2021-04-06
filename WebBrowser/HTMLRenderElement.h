#pragma once
#include <optional>
#include <vector>
#include <memory>

#include "Property.h"
#include "Dimension.h"
#include "Display.h"
#include "HTMLElement.h"
#include "SubManager.h"


class HTMLRenderElement
{
public:
	using Parent = std::optional<HTMLRenderElement*>;
	DimensionConcrete width;
	Pixels y = 0;
	Pixels x = 0;
	DimensionConcrete height;
	DimensionConcrete fontSize;
	Display display;

	Pixels getY() { return y; }
	Pixels getX() { return x; }
	DimensionConcrete getWidth() { return width; }
	DimensionConcrete getHeight() { return height; }
	DimensionConcrete getFontSize() { return fontSize; }


private:
	Parent parent;
	std::vector<std::unique_ptr<HTMLRenderElement>> children;
	SubManager<Style_Name> subStyle;

public:
	HTMLRenderElement(Parent& parent) :parent(parent) {}
	HTMLRenderElement() :parent(std::nullopt) {}
	Parent& getParent() { return parent; };

	virtual void render() {
		for (auto& child : children)
			child->render();
	}

	void updateChildrenPositions() {
		int y = 0;
		for (auto& child : children) {
			child->setY(y);
			y += child->height.pixels;
		}
	}

	void setWidth(Property<Dimension>& prop) {
		auto widthSub = [&](const Dimension& old, const Dimension& current) {
			setWidth(current);
		};
		widthSub(prop, prop);

		auto unsub = prop.subscribe(widthSub);
		subStyle.addSub(Style_Name::width, unsub);
	}

	void setHeight(Property<Dimension>& prop) {
		auto sub = [&](const Dimension& old, const Dimension& current) {
			setHeight(current);
		};
		sub(prop, prop);

		auto unsub = prop.subscribe(sub);
		subStyle.addSub(Style_Name::height, unsub);
	}

	void setFontSize(Property<Dimension>& prop) {
		auto sub = [&](const Dimension& old, const Dimension& current) {
			setFontSize(current);
		};
		sub(prop, prop);

		auto unsub = prop.subscribe(sub);
		subStyle.addSub(Style_Name::fontSize, unsub);
	}

	void setDisplay(Property<Display>& prop) {
		auto sub = [&](const Display& old, const Display& current) {
			setDisplay(current);
		};
		sub(prop, prop);

		auto unsub = prop.subscribe(sub);
		subStyle.addSub(Style_Name::fontSize, unsub);
	}
	
	virtual void addStyles(Style_Name name, Property_Variant& prop) {
		switch (name)
		{
		case Style_Name::fontSize:
			setFontSize(std::get<Property<Dimension>>(prop));
			break;
		case Style_Name::width:
			setWidth(std::get<Property<Dimension>>(prop));
			break;
		case Style_Name::height:
			setHeight(std::get<Property<Dimension>>(prop));
			break;
		case Style_Name::display:
			setDisplay(std::get<Property<Display>>(prop));
			break;
		default:
			break;
		}
	}
	virtual void addProperties(Property_Name name, Property_Variant& prop) {}

	void addChild(std::unique_ptr<HTMLRenderElement>& child) {
		children.push_back(std::move(child));
		updateChildrenPositions();
	}

	template<typename T>
	void addChildren(T& v) {
		for (auto& newChild : v) {
			addChild(newChild);
		}
	}

	virtual void setX(Pixels x) {
		this->x = x;
	}
	virtual void setY(Pixels y) {
		this->y = y;
	}
	virtual void setWidth(Dimension dimension) {
		if (display == Display::inline_block)
			return;

		if (!parent && dimension.index() == 0)
			width.set(dimension, 0);
		else
			width.set(dimension, parent.value()->width.pixels);

		for (auto& child : children)
			child->setWidth(width.pixels);
	}
	virtual void setHeight(Dimension dimension) {
		if (display == Display::inline_block)
			return;

		int oldPixels = height.pixels;
		if (!parent && dimension.index() == 0)
			height.set(dimension, 0);
		else
			height.set(dimension, parent.value()->height.pixels);

		if (oldPixels == height.pixels)
			return;

		if (parent)
			parent.value()->updateChildrenPositions();

		updateChildrenPositions();

		for (auto& child : children)
			child->setHeight(height.pixels);
	}
	virtual void setFontSize(Dimension dimension) {
		if (!parent && dimension.index() == 0)
			fontSize.set(dimension, 0);
		else
			fontSize.set(dimension, parent.value()->fontSize.pixels);
	}

	virtual void setDisplay(Display display) {
		this->display = display;
	}
};


