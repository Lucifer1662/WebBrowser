//#pragma once
//#include "Property.h"
//#include "Dimension.h"
//#include <vector>
//#include <string>
//#include <set>
//#include <optional>
//
//enum Property_Name {
//	bold,
//	italic, 
//	fontSize,
//	x,
//	y,
//	width,
//	height
//};
//
//class Properties
//{
//	std::unordered_map<Property_Name, Property_Variant> props;
//
//
//	void apply(Property_Name name, Property_Variant prop) {
//		props.insert_or_assign(name, prop);
//	}
//
//	template<typename T>
//	std::optional<Property<T>> get(Property_Name name) {
//		auto it = props.find(name);
//		if (it != props.end()) {
//			return std::get<T>(*it);
//		}
//		return std::nullopt;
//	}
//
//};
//
