#pragma once
#include <variant>
#include <functional>
#include <memory>
#include <optional>

using Pixels = int;
using Percetage = float;
using ScreenHeight = float;

using Dimension = std::variant<Pixels, Percetage>;


struct DimensionConcrete {
	Pixels pixels;
	std::optional<Percetage> percentage;

	void update(Pixels parentPixels) {
		if (percentage) {
			pixels = parentPixels * percentage.value() * 100.0f;
		}
	}

	void set(Dimension dimension, Pixels parentPixels) {
		if (dimension.index() == 0) {
			pixels = std::get<Pixels>(dimension);
			percentage = std::nullopt;
		}
		else {
			percentage = std::get<Percetage>(dimension);
			pixels = parentPixels * percentage.value() * 100.0f;
		}
	}
};


