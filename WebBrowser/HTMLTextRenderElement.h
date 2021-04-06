#pragma once
#include "HTMLRenderElement.h"

class HTMLTextElemement : public HTMLRenderElement {
	SubManager<Property_Name> subProp;
	SubManager<Style_Name> subStyle;
	std::string body;

public:

	HTMLTextElemement(Parent& parent) : HTMLRenderElement(parent) {}

	const std::string& getBody() const {
		return body;
	}

	void setBodyProperty(Property<std::string>& prop) {
		auto sub = [&](const std::string& old, const std::string& current) {
			setBody(current);
		};
		sub(prop, prop);
		prop.subscribe(sub);
		auto unsub = prop.subscribe(sub);
		subProp.addSub(Property_Name::body, unsub);
	}

	void setFontWeight(Property<FontWeight>& prop) {
		auto sub = [&](const FontWeight& old, const FontWeight& current) {
			setFontWeight(current);
		};
		sub(prop, prop);
		prop.subscribe(sub);
		auto unsub = prop.subscribe(sub);
		subStyle.addSub(Style_Name::fontWeight, unsub);
	}

	void addStyles(Style_Name name, Property_Variant& prop) {
		switch (name) {
		case Style_Name::fontWeight:
			setFontWeight(std::get<Property<FontWeight>>(prop));
			break;
		}
	}

	void addProperties(Property_Name name, Property_Variant& prop) {
		switch (name) {
		case Property_Name::body:
			setBodyProperty(std::get<Property<std::string>>(prop));
			break;
		}
	}

	virtual void setBody(const std::string& body) {
		this->body = body;
	}

	virtual void setFontWeight(FontWeight weight) {}
};

