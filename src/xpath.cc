#include "xpath.hpp"
using std::cout;
using std::endl;
int XPATH_PARSE_STATUE = XPATH_PARSE_SUCCESS;

CXMLNode_result *xpath_option(const string exp, CXMLNode *root)
{
    const char *ptr = exp.c_str();
    int len = 0;
    while (*ptr != '\0' && *ptr != '/')
    {
        ptr++;
        len++;
    }

    string name = exp.substr(0, len);
    cout << name;
    return nullptr;
}

const CXMLNode_result *xpath(const string exp, CXMLNode *root)
{
    const char *ptr = exp.c_str();
    int len = 0;
    CXMLNode_result *ret = new CXMLNode_result();
    while (*ptr != '\0')
    {
        if (*ptr == '/' && *ptr + 1 != '/')
        {
            ptr++;
            ret = xpath_option(ptr, root);
            return ret;
        }
        ptr++;
    }

    return nullptr;
}