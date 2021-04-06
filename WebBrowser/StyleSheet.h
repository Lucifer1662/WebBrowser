#pragma once
#include <unordered_map>
#include "Style.h"
#include "HTMLElement.h"
#include <optional>

class StyleSheet
{
private:
	std::unordered_map<Tag_Name, Style> styleTagNames;
	std::unordered_map<std::string, Style> styleClass;

public: 
	std::optional<std::reference_wrapper<Style>> getStyleForTag(Tag_Name tag) {
		auto it = styleTagNames.find(tag);
		if (it == styleTagNames.end())
			return std::nullopt;
		return it->second;
	}


};

