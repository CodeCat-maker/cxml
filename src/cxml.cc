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

bool parser_element_name(const string ptr, string &name)
{
    const char *p = ptr.c_str();
    int len = 0;
    p = skip(p);

    while (*p != ' ' && *p != '>')
    {
        if (*p == '\0' || *p == '<' || *p == '\"' || *p == '=') //非法字符
        {
            CXML_PARSER_STATUS = CXML_SYNTAX_ERROR;
            return false;
        }
        len++;
        p++;
    }
    name = ptr.substr(0, len);
    return true;
}

bool parse_node_attr(const string cxml, CXMLNode *root)
{
    string str = cxml;
    string attr_name, attr_value;
    CXMLNode_attr *attrs = new CXMLNode_attr();
    string _str = str.substr(str.find(root->name) - 1, str.find('>') + 1);
    size_t equal_pos = 0;

    while (_str.find('=') != equal_pos)
    {
        equal_pos = _str.find('=');
        _str = _str.substr(_str.find(' '));
        attr_name = _str.substr(0, _str.find('='));
        _str = _str.substr(_str.find('\"') + 1);
        attr_value = _str.substr(0, _str.find('\"'));
        attrs->attributes.insert(std::pair<string, string>(attr_name, attr_value));
        attrs->nums++;
        cout << attr_name << " " << attr_value << std::endl;
        if (_str.find('=') > _str.length())
            break;
    }
    root->parent = root;
    return true;
}

bool parse_node(const string cxml, CXMLNode *root)
{
    const char *ptr = cxml.c_str();
    ptr = skip(ptr);
    if (*ptr != '<')
    {
        CXML_PARSER_STATUS = CXML_SYNTAX_ERROR;
        return false;
    }
    root->content = ptr;
    ptr++;
    string eleName;
    if (parser_element_name(ptr, eleName) == false)
    {
        CXML_PARSER_STATUS = CXML_SYNTAX_ERROR;
        return false;
    };
    root->name = eleName;
    size_t bpos = string(cxml).find(eleName);
    size_t epos = string(cxml).rfind(eleName);

    if (bpos == epos)
    {
        CXML_PARSER_STATUS = CXML_SYNTAX_ERROR;
        return false;
    }
    root->content = string(cxml).substr(bpos + root->name.length() + 1, epos - root->name.length() - 4);
    if (parse_node_attr(cxml, root) == false)
    {
        CXML_PARSER_STATUS = CXML_SYNTAX_ERROR;
        return false;
    }
    return true;
}