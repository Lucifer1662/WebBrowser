#pragma once
#include "HTMLRenderElement.h"
class HTMLImageRender : public HTMLRenderElement
{
	SubManager<Property_Name> subProp;
public:
	HTMLImageRender(Parent& parent) : HTMLRenderElement(parent) {}


	void setSourceProperty(Property<std::string>& prop) {
		auto sub = [&](const std::string& old, const std::string& current) {
			setSource(current);
		};
		sub(prop, prop);
		prop.subscribe(sub);
		auto unsub = prop.subscribe(sub);
		subProp.addSub(Property_Name::source, unsub);
	}


	void addProperties(Property_Name name, Property_Variant& prop) {
		switch (name) {
		case Property_Name::source:
			setSourceProperty(std::get<Property<std::string>>(prop));
			break;
		}
	}


	virtual void setSource(const std::string& source) {}
};

