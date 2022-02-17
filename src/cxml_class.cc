#include "cxml_class.hpp"

CXMLNode_attr::CXMLNode_attr()
{
    next = perv = nullptr;
    nums = 0;
}

CXMLNode::CXMLNode()
{
    name = content = "";
    parent = nullptr;
    perv = next = nullptr;
    attr = nullptr;
}

CXMLNode_text::CXMLNode_text()
{
    perv = next = nullptr;
    lens = 0;
}
string CXMLNode::get_name() const
{
    string ret = "";
    ret = this->name;
    return ret;
}
string CXMLNode::get_text() const
{
    string ret = "";
    CXMLNode_text *child = this->text;
    ret = child->content;
    return ret;
}
map<string, string> CXMLNode::get_attrs() const
{
    map<string, string> ret;
    CXMLNode_attr *child = this->attr;
    ret = child->attributes;
    return ret;
}
CXMLNode *CXMLNode::get_parent() const
{
    CXMLNode *ret;
    ret = this->parent;
    return ret;
}
CXMLNode *CXMLNode::get_child() const
{
    CXMLNode *ret;
    ret = this->children.back();
    return ret;
}
vector<CXMLNode *> CXMLNode::get_children() const
{
    vector<CXMLNode *> ret;
    ret = this->children;
    return ret;
}
CXMLNode_result::CXMLNode_result()
{
    element = nullptr;
}