#ifndef __CXML__H_
#define __CXML__H_

#define CXML_PARSER_SUCCESS 0
#define CXML_CONTENT_EMPTY -1
#define CXML_SYNTAX_ERROR -2

#define CXML_SYN_HTML 0
#define CXML_SYN_STRICT 1

#include "cxml_class.hpp"
#include <algorithm>
bool is_open_label(char *label);
bool blank(char c);
const string skip(const string pointer);
bool parser_element_name(const string ptr, char **name);
bool parse_node(const string cxml, CXMLNode *root);
bool parse_node_attr(const string cxml, CXMLNode *root);
CXMLNode *parse_from_string(string cxml);

#endif