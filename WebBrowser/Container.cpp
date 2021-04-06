#include "Container.h"

void Container::render()
{
	for (auto& child : children) {
		child->render();
	}
}
