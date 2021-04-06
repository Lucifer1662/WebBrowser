#pragma once
#include <optional>
#include <memory>
#include <SFML/Graphics.hpp>

struct Entity
{
	using ParentEntity = std::optional<std::reference_wrapper<Entity>>;
	ParentEntity parent;
	std::vector<std::unique_ptr<Entity>> children;
	sf::Transform transform;
	int width;
	int height;

	virtual void render() {};
};

