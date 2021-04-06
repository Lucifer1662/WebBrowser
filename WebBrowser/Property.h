#pragma once
#include <vector>
#include <functional>
#include <variant>
#include <algorithm>
#include <string>
#include "Dimension.h"
#include "FontWeight.h"
#include "Display.h"

template<typename T>
class Property
{
public:
	using Subscription = std::function<void(const T&, const T&)>;
	using Unsubscribe = std::function<void(void)>;
private:
	T value;
	int i = 0;
	std::vector<std::pair<int,Subscription>> subscriptions;
public:

	Property(const T& t): value(t) {}
	/*Property<T>& operator=(const Property<T>& t) {
		value = t.value;
		subscriptions.insert(subscriptions.end(),
			t.subscriptions.begin(), t.subscriptions.end());
		return *this;
	}
	*/
	T& get() {
		return value;
	}
	void set(const T& t) {
		for (auto& func : subscriptions)
			func.second(value, t);
		value = t;
	}

	T operator = (const T& t) {
		set(t);
		return t;
	}
	Property<T> operator = (const Property<T>&) = delete;

	operator T() const { return value; }

	auto subscribe(Subscription subscription) {
		i++;
		auto sub = std::make_pair(i, subscription);
		subscriptions.push_back(sub);
		int w = i;
		return [&, w]() {

			for (auto j = subscriptions.begin(); j != subscriptions.end(); j++) {
				if (j->first == w) {
					*j = subscriptions.back();
					subscriptions.pop_back();
					break;
				}
			}
			
		};
	}
};


using Property_Variant = std::variant<
	Property<Dimension>, 
	Property<std::string>,
	Property<bool>,
	Property<FontWeight>,
	Property<Display>
>;

