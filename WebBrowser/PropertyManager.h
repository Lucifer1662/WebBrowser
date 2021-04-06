#pragma once
#include <string>


template<char* NAME, typename T>
struct Property {
	static std::string name = std::string(NAME);
	private T value;

};


template<typename... Properties_T>
class PropertyManager
{
	std::tuple<Properties_T> properties;

	public int get(const std::string& name) {
		switch (name)
		{

		case "Hello":
		break;
		default:
			break;
		}
	}

	template<T, typename... Ts>
	private int get_(const std::string& name) {
		
	}
};

