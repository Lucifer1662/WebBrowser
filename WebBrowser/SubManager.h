#pragma once
#include <functional>
#include <unordered_map>

template<typename T>
class SubManager
{
	std::unordered_map<T, std::function<void()>> unsubs;

public:
	void addSub(const T& t, std::function<void()>&& unsub) {
		auto it = unsubs.find(t);
		if (it != unsubs.end()) {
			it->second();
		}
		unsubs.insert_or_assign(t, unsub);
	}

	~SubManager() {
		for (auto& name_unsub : unsubs) {
			name_unsub.second();
		}
	}
};

