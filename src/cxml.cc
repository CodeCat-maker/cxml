#include "cxml.hpp"
using std::cout;
//设置解析状态
int CXML_PARSER_STATUS = CXML_PARSER_SUCCESS;

//单口节点
const char *open_labels[] = {"br", "hr", "img", "input",
                             "meta", "area", "base", "col",
                             "command", "embed", "keygen", "param",
                             "source", "track", "wbr", "link"};

bool is_open_label(const char *label)
{
    for (int i = 0; i < 16; i++)
    {
        if (std::strcmp(label, open_labels[i]))
        {
            return true;
        }
    }
    return false;
}

bool blank(char c)
{
    return (c == ' ' || c == '\n' || c == '\r' || c == '\t');
}

const char *skip(const char *ptr)
{
    while (*ptr != '\0' && blank(*ptr))
        ptr++;
    return ptr;
}
//从字符串解析
CXMLNode *parse_from_string(string cxml)
{

    const char *ptr = cxml.c_str();
    if (cxml.length() == 0)
    {
        CXML_PARSER_STATUS = CXML_CONTENT_EMPTY;
        return nullptr;
    }
    while (*ptr != '<')
        ptr++;
    char *dt = new char[100];
    memset(dt, '\0', 100);
    strncpy(dt, ptr + 2, 7);

    if (!strcmp(dt, "doctype"))
    {
        while (*ptr != '>')
            ptr++;
        ptr += 1;
    }

    CXMLNode *root = new CXMLNode();
    parse_node(ptr, root);

    CXMLNode *head = new CXMLNode();
    head->children = root;

    return head;
}

const char *parser_element_name(const char *ptr, char **name)
{
    int len = 0;
    ptr = skip(ptr);
    const char *p = ptr;

    while (*p != ' ' && *p != '>')
    {
        if (*p == '\0' || *p == '<' || *p == '\"' || *p == '=') //非法字符
        {
            CXML_PARSER_STATUS = CXML_SYNTAX_ERROR;
            return p;
        }
        len++;
        p++;
    }

    *name = new char[len + 1];
    memset(*name, '\0', len + 1);
    strncpy(*name, ptr, len);
    return p;
}

void parse_node(const char *cxml, CXMLNode *root)
{
    const char *ptr = cxml;
    ptr = skip(ptr);
    if (*ptr != '<')
    {
        CXML_PARSER_STATUS = CXML_SYNTAX_ERROR;
        return;
    }
    root->content = ptr;
    ptr++;
    char *eleName;
    ptr = parser_element_name(ptr, &eleName);
    root->name = eleName;
    if (XML_PARSE_STATUS != XML_PARSE_SUCCESS)
        return;
    ptr = skip(ptr);
    //代表是单标签
    if (*ptr == '/')
    {
    }
}