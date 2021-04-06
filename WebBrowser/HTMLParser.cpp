#include "HTMLParser.h"

#include "DOMElement.h"
#include "DOMDocument.h"
#include "DOMText.h"
#include <string>
#include <memory>
#include <algorithm>
#include <iostream>
#include "Node.h"
#include "HTMLElement.h"

std::vector<std::string> voidNames = {
	"area", "base", "br", "col", "embed", "hr", "img",
	"input", "link", "meta", "param", "source", "track", "wbr"
};


std::optional<int> findNextT(const std::string_view& html) {
	for (int i = 0; i < html.size(); i++) {
		if (html[i] == '<')
			return i;
	}
	return std::nullopt;
}

std::optional<int> nextNode(std::string_view tag) {
	//auto it = std::find(tag.begin(), tag.end(), '>');
	auto it = std::find_if(tag.begin()+1, tag.end(),
		[](auto v) { return v == '>' || v == '<'; });
	if (it == tag.end())
		return tag.size();

	int i = it - tag.begin();
	if (*it == '>')
		i += 1;
	return i;
}

std::optional<int> endTagPos(std::string_view tag) {
	//auto it = std::find(tag.begin(), tag.end(), '>');
	auto it = std::find_if(tag.begin() + 1, tag.end(),
		[](auto v) { return v == '>'; });
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



bool isSelfClosing(std::string_view tag, std::string_view tagName) {
	std::string tagNameLower = std::string(tagName);
	std::transform(tagName.begin(), tagName.end(), tagNameLower.begin(),
		[](auto v) {return std::tolower(v); });

	if (std::any_of(voidNames.begin(), voidNames.end(), 
		[&](auto v) {return v == tagNameLower; })) {
		return true;
	}

	return tag[tag.size()-2] == '/';
}

std::string_view getTagName(std::string_view tag) {
	auto begin = std::find_if(tag.begin(), tag.end(),
		[](const char& v) {return std::isalpha(v); });
	auto end = std::find_if(begin, tag.end(),
		[](const char& v) {return !std::isalpha(v); });
	return tag.substr(begin - tag.begin(), end - begin);
}

//std::unique_ptr<DOMElement> getTag(const std::string_view& tag,
//	const std::string_view& children) {
//
//	auto tagName = std::string(getTagName(tag));
//	std::transform(tagName.begin(), tagName.end(), tagName.begin(),
//		[](auto v) {return std::tolower(v); });
//
//
//	if (tagName == "p") {
//		auto text = std::make_unique<DOMText>();
//		text->body = std::string(children);
//		return text;
//	}
//	if (tagName == "html")
//		return std::make_unique<DOMDocument>();
//
//	return std::make_unique<DOMText>();
//}

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

/*
<html>
	<a>
		<img>
		<p></p>
	</a>
	<p>
		Hello world
	</p>
</html>

html, a, img, p
html, a -> {img, p}
html, a->{img, p}, p
html, a->{img, p}, p, "Hello world"
html, a->{img, p}, p->{"Hello world"}
html -> {a->{img,p}, p->{"Hello world"}}


html->a

*/


std::string remove_extra_whitespaces(const std::string& input)
{
	std::string output;
	output.reserve(input.size());
	output.clear();  // unless you want to add at the end of existing sring...
	unique_copy(input.begin(), input.end(), std::back_insert_iterator<std::string>(output),
		[](char a, char b) { return isspace(a) && isspace(b); });
	return output;
}

std::string toLower(const std::string_view& s) {
	std::string sLower = std::string(s);
	std::transform(s.begin(), s.end(), sLower.begin(),
		[](auto v) {return std::tolower(v); });
	return sLower;
}

Node parseDocument(std::string_view html) {
	auto document = Node("", "document");
	std::vector<Node>& queue = document.children;

	auto next = nextNode(html);
	if (next) {
		int endTag = next.value();
		html = html.substr(endTag);
	}

	while (html.size() > 0) {
		auto next = nextNode(html);
		
		if (next) {
			int endTag = next.value();
			auto tag = html.substr(0, endTag);

			if (html.front() == '<') {
				auto temp = getTagName(tag);
				auto tagName = toLower(temp);

				if (html.size() > 1 && html[1] == '/') {
					for (int i = queue.size() - 1; i >= 0; i--) {
						if (tagName == queue[i].name) {
							queue[i].children.reserve(queue.size() - i - 1);
							std::copy(queue.begin() + i + 1, queue.end(),
							std::back_inserter(queue[i].children));
							queue.erase(queue.begin() + i + 1, queue.end());
							/*while (queue.size() > i + 1) {
								
								queue[i].children.push_back(queue[i + 1]);

								queue[i + 1] = queue.back();
								queue.pop_back();
							}*/
							break;
						}
					}
				}
				else {
					queue.push_back(Node(std::string(tag), tagName));
				}
			}
			else {
				std::string t;
				t.reserve(tag.size());
				std::copy_if(tag.begin(), tag.end(), std::back_inserter(t),
					[](auto v) {return v != '\n'; });
				t = remove_extra_whitespaces(t);
				if(t != "" && t !=" ")
					queue.push_back(Node(t, ""));

			}
			html = html.substr(endTag);
		}
	}
	
	
	return document;
}

bool containsL(const std::string& s, const std::string& v) {
	auto endit = std::search(s.begin(), s.end(), v.begin(), v.end(),
		[](const char& l, const char& r) { return std::tolower(l) == std::tolower(r); });
	return endit != s.end();
}

Dimension stringToDimension(const std::string& s) {
	return 0;
}

std::vector<std::pair<std::string, std::string>> 
getAttributeStrings(const std::string& s) {
	return {};
}

Tag_Name toTag(std::string& s) {
	static std::unordered_map<std::string, Tag_Name> tagNames = 
	{ 
		{std::string("b"), Tag_Name::b}, 
		{std::string("i"), Tag_Name::i},
		{std::string("div"), Tag_Name::div},
		{std::string("h1"), Tag_Name::h1},
		{std::string("h2"), Tag_Name::h2},
		{std::string("h3"), Tag_Name::h3},
		{std::string("h4"), Tag_Name::h4},
		{std::string("h5"), Tag_Name::h5},
		{std::string("h6"), Tag_Name::h6},
		{std::string("img"), Tag_Name::img},
		{std::string("p"), Tag_Name::p},
		{std::string("head"), Tag_Name::head},
		{std::string("html"), Tag_Name::html},
		{std::string(""), Tag_Name::text},
		{std::string("title"), Tag_Name::title},
	};

	return tagNames[s];
}

void parseStyle(const std::string_view& styleString, Style& style) {
	
}


int nextNotWhiteSpace(const std::string_view& s, int i = 0) {
	for (; i < s.size() && std::isspace(s[i]); i++) {
	
	}
	return i;
}

int nextWhiteSpaceOrEqual(const std::string_view& s, int i = 0) {
	for (; i < s.size() && !(std::isspace(s[i]) || s[i] == '='); i++) {

	}
	return i;
}

bool isNextEquals(const std::string_view& s, int i = 0) {
	int a = nextNotWhiteSpace(s, i);
	if (a == s.size()) {
		return false;
	}
	return s[a] == '=';		
}

Property_Name toProperty(const std::string_view& s) {
	static std::unordered_map<std::string, Property_Name> tagNames =
	{
		{std::string("body"), Property_Name::body},
		{std::string("height"), Property_Name::height},
		{std::string("width"), Property_Name::width},
		{std::string("src"), Property_Name::source}
	};
	return tagNames[toLower(s)];
}

void assignPropery(HTML_Element& element,
	const std::string_view& PropName,
	const std::string_view& value) {
	auto propName = toProperty(PropName);
	switch (propName)
	{
	case Property_Name::source:
		element.assignProperty(propName, std::string(value));
		break;
	default:
		break;
	}
}

bool isQuote(char c) {
	return c == '\'' || c == '"';
}

int findNextQuote(const std::string_view& s, int i) {
	for (; i < s.size() && !isQuote(s[i]); i++) {

	}
	return i;
}

int findNextQuoteSingle(const std::string_view& s, int i) {
	for (; i < s.size() && s[i] != '\''; i++) {

	}
	return i;
}

int findNextQuoteDouble(const std::string_view& s, int i) {
	for (; i < s.size() && s[i] != '"'; i++) {

	}
	return i;
}

void parseProps(const std::string_view& tagString, HTML_Element& element) {
	auto spaceIndex = tagString.find(" ");


	while (spaceIndex != std::string::npos) {
		//get attrib name
		int attribStart = nextNotWhiteSpace(tagString, spaceIndex+1);
		int attribEnd = nextWhiteSpaceOrEqual(tagString, attribStart+1);
		auto attribName = tagString.substr(attribStart, attribEnd - attribStart);
		//is next equals 
		if (!isNextEquals(tagString, attribEnd)) {
			assignPropery(element, attribName, "");
			spaceIndex = tagString.find(" ", attribEnd+1);
		}
		else {
			auto quoteStart = findNextQuote(tagString, attribEnd + 1);
			auto endQuote = tagString[quoteStart] == '"' ?
				findNextQuoteDouble(tagString, quoteStart + 1) :
				findNextQuoteSingle(tagString, quoteStart + 1);
			auto value = tagString.substr(quoteStart+1, endQuote - quoteStart-1);
			assignPropery(element, attribName, value);
			spaceIndex = tagString.find(" ", endQuote + 1);

		}
		
	}
}

HTML_Element toHTML(Node& current) {
	HTML_Element elem = toTag(current.name);

	if (elem.getTag() == Tag_Name::text) {
		elem.assignProperty(Property_Name::body, current.value);
	}
	if (elem.getTag() == Tag_Name::b) {
		elem.assignStyle(Style_Name::fontWeight, FontWeight::bold);
	}
	if(elem.getTag() == Tag_Name::p){
		/*
		 display: block;
	  margin-top: 1em;
	  margin-bottom: 1em;
	  margin-left: 0;
	  margin-right: 0;
		*/
		//elem.assign(display, block);
		//elem.assignStyle(Style_Name::fontWeight, containsL(current.value, std::string("true")));
		/*elem.assignProperty(Property_Name::margin_top, (Dimension)0);
		elem.assignProperty(Property_Name::margin_bottom, (Dimension)0);
		elem.assignProperty(Property_Name::margin_left, (Dimension)0);
		elem.assignProperty(Property_Name::margin_right, (Dimension)0);*/
	}
	if (elem.getTag() == Tag_Name::div) {
		/*
		 display: block;
		*/

		//elem.assign(display, block);
	}

	if (elem.getTag() == Tag_Name::html) {
		/*
		 display: block;
		*/
	}

	if (elem.getTag() == Tag_Name::head) {
		/*
		 display: block;
		*/
	}

	if (elem.getTag() == Tag_Name::img) {
		/*
		 display: block;
		*/
		elem.assignProperty(Property_Name::source, std::string("test.jpg"));
		elem.assignStyle(Style_Name::display, Display::inline_block);
	}
	
	//Read style attrib,
	//parseStyle("", elem.getStyle());
	if(elem.getTag() != Tag_Name::text)
		parseProps(current.value, elem);
	

	for (auto& child : current.children) {
		elem.addChild(toHTML(child));
	}
	return elem;
}





