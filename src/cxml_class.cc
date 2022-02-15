#include "cxml_class.hpp"

CXMLNode_attr::CXMLNode_attr()
{
    name = "";
    value = "";
    perv = next = nullptr;
}

CXMLNode::CXMLNode()
{
    name = content = "";
    content_length = 0;
    parent = children = nullptr;
    perv = next = nullptr;
    attributes = nullptr;
}