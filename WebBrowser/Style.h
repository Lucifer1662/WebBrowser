#pragma once
#include <unordered_map>
#include "Property.h"
#include <optional>

enum class Style_Name {
	fontSize,
	fontWeight,
	fontStyle,
	width,
	height,
	display,
	margin_top,
	margin_bottom,
	margin_left,
	margin_right
};

class Style : public std::unordered_map<Style_Name, Property_Variant>
{
public:
	template<typename T> 
	std::optional<T> get(Style_Name name) {
		auto it = find(name);
		if (it != end()) {
			return std::get<Property<T>>(it->second).get();
		}
		else {
			return std::nullopt;
		}
	}

	template<typename T>
	T get_or_default(Style_Name name, T defaul) {
		auto it = find(name);
		if (it != end()) {
			return std::get<Property<T>>(it->second).get();
		}
		else {
			return defaul;
		}
	}
};


class StyleRef : public std::unordered_map<Style_Name, 
	std::reference_wrapper<Property_Variant>>
{
public:
	template<typename T>
	std::optional<T> get(Style_Name name) {
		auto it = find(name);
		if (it != end()) {
			//return std::get<Property<T>>(it->second.get()).get();
		}
		else {
			return std::nullopt;
		}
	}

	template<typename T>
	T get_or_default(Style_Name name, T defaul) {
		auto it = find(name);
		if (it != end()) {
			return std::get<Property<T>>(it->second.get()).get();
		}
		else {
			return defaul;
		}
	}
};
