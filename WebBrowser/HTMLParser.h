#pragma once
#include "DOMElement.h"
#include "DOMDocument.h"
#include "DOMText.h"
#include <string>
#include <memory>
#include <algorithm>
#include "Node.h"
#include "HTMLElement.h"

struct HTMLParser
{

};



Node parseDocument(std::string_view html);
HTML_Element toHTML(Node& current);



/*
#include "HTMLParser.h"

#include "DOMElement.h"
#include "DOMDocument.h"
#include "DOMText.h"
#include <string>
#include <memory>
#include <algorithm>
#include <iostream>
#include "Tag.h"




std::optional<int> findNextTag(const std::string_view& html) {
	for (int i = 0; i < html.size(); i++) {
		if (html[i] == '<')
			return i;
	}
	return std::nullopt;
}

std::optional<int> endTagPos(std::string_view tag) {
	auto it = std::find(tag.begin(), tag.end(), '>');
	if (it == tag.end())
		return std::nullopt;
	int i = it - tag.begin();
	return i;
}

bool isEndTag(std::string_view html, std::string_view::iterator pos) {
	return *(pos - 1) == '/';
}

std::optional<int> closingPos(std::string_view html, std::string_view tag) {

	int depth = 0;
	auto begin = html.begin();
	auto endTag = std::string("</") + std::string(tag) + ">";
	auto beginTag = std::string("<") + std::string(tag);
	while (begin != html.end()) {
		auto endit = std::search(begin, html.end(), endTag.begin(), endTag.end(),
			[](const char& l, const char& r) { return std::tolower(l) == std::tolower(r); });

		auto beginit = std::search(begin, html.end(), beginTag.begin(), beginTag.end(),
			[](const char& l, const char& r) { return std::tolower(l) == std::tolower(r); });

		auto it = beginit < endit ? beginit : endit;
		begin = it;
		bool isEnd = beginit > endit;
		if (isEnd) {
			if (depth == 0) {
				return it - html.begin();
			}
			else {
				depth--;
			}
		}
		else {
			depth++;
		}
	}

	return std::nullopt;
}



bool isInline(std::string_view tagName) {
	std::string tagNameLower = std::string(tagName);
	std::transform(tagName.begin(), tagName.end(), tagNameLower.begin(),
		[](auto v) {return std::tolower(v); });

	if(tagNameLower == "img")
		return true;

	if (tagNameLower == "br")
		return true;

	if (tagNameLower == "p")
		return false;

	if (tagNameLower == "h1")
		return false;

	if (tagNameLower == "h2")
		return false;

	if (tagNameLower == "hr")
		return true;

	if (tagNameLower == "html")
		return false;

	if (tagNameLower == "center")
		return false;

	if (tagNameLower == "body")
		return false;

	if(tagNameLower == "head")
		return false;

	if (tagNameLower == "a")
		return false;

	if (tagNameLower == "b")
		return false;

	if (tagNameLower == "i")
		return false;

	if (tagNameLower == "title")
		return false;

	return true;

}

std::string_view getTagName(std::string_view tag) {
	auto begin = std::find_if(tag.begin(), tag.end(),
		[](const char& v) {return std::isalpha(v); });
	auto end = std::find_if(begin, tag.end(),
		[](const char& v) {return !std::isalpha(v); });
	return tag.substr(begin - tag.begin(), end - begin);
}

std::unique_ptr<DOMElement> getTag(const std::string_view& tag,
	const std::string_view& children) {

	auto tagName = std::string(getTagName(tag));
	std::transform(tagName.begin(), tagName.end(), tagName.begin(),
		[](auto v) {return std::tolower(v); });


	if (tagName == "p") {
		auto text = std::make_unique<DOMText>();
		text->body = std::string(children);
		return text;
	}
	if (tagName == "html")
		return std::make_unique<DOMDocument>();

	return std::make_unique<DOMText>();
}

std::unique_ptr<DOMElement>&& getTagInline(const std::string_view& tag) {

	auto tagName = std::string(getTagName(tag));
	std::transform(tagName.begin(), tagName.end(), tagName.begin(),
		[](auto v) {return std::tolower(v); });


	if (tagName == "p") {
		auto text = std::make_unique<DOMText>();
		return std::move(text);
	}
	if (tagName == "html")
		return std::move(std::make_unique<DOMDocument>());

	return std::move(std::make_unique<DOMText>());
}




void parseDocument(std::unique_ptr<DOMElement>& parent,
	std::string_view html) {
	auto nextOpt = findNextTag(html);

	while (nextOpt) {
		int next = nextOpt.value();
		auto endTagOpt = endTagPos(html);

		if (endTagOpt) {
			int endTag = endTagOpt.value()+1;
			auto tag = html.substr(next, endTag - next);
			auto tagName = getTagName(tag);

			if (isInline(tagName)) {
				auto childParent = getTagInline(tag);
				parent->children.push_back(std::move(childParent));
				html = html.substr(endTag);
			}
			else {
				auto body = html.substr(endTag);
				auto closeingTagOpt = closingPos(body, tagName);
				if (closeingTagOpt) {
					int closingTag = closeingTagOpt.value();
					auto children = body.substr(0, closingTag);
					auto childParent = getTag(tag, children);
					parseDocument(childParent, children);
					parent->children.push_back(std::move(childParent));
					html = html.substr(closingTag + endTag + tagName.size() + 3);
				}
			}
		}
		nextOpt = findNextTag(html);
	}
}



void parseDocument(Tag& parent, std::string_view html) {
	auto nextOpt = findNextTag(html);

	while (nextOpt) {
		int next = nextOpt.value();
		auto endTagOpt = endTagPos(html);

		if (endTagOpt) {
			int endTag = endTagOpt.value() + 1;
			auto tag = html.substr(next, endTag - next);
			auto tagName = getTagName(tag);

			if (isInline(tagName)) {
				auto childParent = getTagInline(tag);
				parent.children.push_back(childParent);
				html = html.substr(endTag);
			}
			else {
				auto body = html.substr(endTag);
				auto closeingTagOpt = closingPos(body, tagName);
				if (closeingTagOpt) {
					int closingTag = closeingTagOpt.value();
					auto children = body.substr(0, closingTag);
					auto childParent = getTag(tag, children);
					parseDocument(childParent, children);
					parent->children.push_back(std::move(childParent));
					html = html.substr(closingTag + endTag + tagName.size() + 3);
				}
			}
		}
		nextOpt = findNextTag(html);
	}
}


*/