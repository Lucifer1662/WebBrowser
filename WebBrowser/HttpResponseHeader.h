#pragma once
#include <string>
class HttpResponseHeader
{
	
};

class HttpResponseHeaderFactory {
public:
	auto create(const std::string& header) {
		return HttpResponseHeader();
	}
};

