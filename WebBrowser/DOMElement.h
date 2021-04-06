#pragma once
#include <memory>
#include <optional>
#include "Property.h"
#include <vector>

class DOMElement
{
   
public:
    using ParentDOMElement = std::optional<std::reference_wrapper<DOMElement>>;
    ParentDOMElement parent;
    std::vector<std::unique_ptr<DOMElement>> children;

    DOMElement() {
        children = std::vector<std::unique_ptr<DOMElement>>();
    }
    DOMElement(ParentDOMElement parent):parent(parent) { }
    /*virtual Property_Variant getProperty(std::string name);*/
};

