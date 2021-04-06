#pragma once
#include <vector>
#include <string>

class Node
{
public:
	Node(const std::string& value, const std::string& name) 
		:value(value), name(name) {}
	std::string value;
	bool isText;
	std::string name;
	std::vector<Node> children;
};

