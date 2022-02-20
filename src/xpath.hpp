#ifndef __XPATH__H_
#define __XPATH__H_

#define XPATH_PARSE_SUCCESS 0
#define XPATH_SYNTAX_ERROR -3

#define XPATH_TYPE_TEXT 0
#define XPATH_TYPE_ATTR 1
#define XPATH_TYPE_ELEM 2

#include "cxml.hpp"
#include <queue>
bool get_xpath_option(const string exp);
bool do_xpath_option(CXMLNode *root, CXMLNode_result &result);
const CXMLNode_result *xpath(const string exp, CXMLNode *root);

#endif