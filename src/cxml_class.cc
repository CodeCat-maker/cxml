#include "cxml_class.hpp"

CXMLNode_attr::CXMLNode_attr()
{
    next = perv = nullptr;
    nums = 0;
}

CXMLNode::CXMLNode()
{
    name = content = "";
    parent = children = nullptr;
    perv = next = nullptr;
    attributes = nullptr;
}

CXMLNode_text::CXMLNode_text()
{
    perv = next = nullptr;
    lens = 0;
}