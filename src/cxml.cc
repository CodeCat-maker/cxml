#include "cxml.hpp"
using std::cout;
using std::__1::stack;
//设置解析状态
int CXML_PARSER_STATUS = CXML_PARSER_SUCCESS;
//节点栈
stack<CXMLNode *> st;

//单口节点
const string open_labels[] = {"br", "hr", "img", "input",
                              "meta", "area", "base", "col",
                              "command", "embed", "keygen", "param",
                              "source", "track", "wbr", "link"};
//跳过空格
bool is_blank(char c)
{
    return (c == ' ' || c == '\n' || c == '\r' || c == '\t');
}
//跳过空格
void strip(string &str)
{
    const char *ptr = str.c_str();
    while (*ptr != '\0' && is_blank(*ptr))
        ptr++;

    str = ptr;
}
//判断是否是单口标签
bool is_open(string name)
{
    for (int i = 0; i < 16; i++)
    {
        if (name.compare(open_labels[i]) == 0)
        {
            return true;
        }
    }
    return false;
}

string find_rigth_element_name(const string cxml)
{
    string name;
    string str = cxml;
    size_t pos = str.find("</");
    int i = 0;
    while (str[i + (int)pos] != '>')
    {
        i++;
    }
    name = str.substr(pos).substr(2, i - 2);
    return name;
}

void debug_stack()
{
    cout << st.size() << "\n";
}
//解析节点文字
CXMLNode *parse_node_element_text(const string cxml, CXMLNode *root)
{
    CXMLNode_text *node_text = new CXMLNode_text();
    if (is_open(root->name))
    {
        node_text->content = "";
        root->text = node_text;
        return root;
    }

    string text = "";
    string str = cxml;
    str = str.substr(str.find(">") + 1);
    str = str.substr(0, str.find("<"));

    strip(str);
    node_text->content = str;
    node_text->lens = str.length();
    root->text = node_text;

    return root;
}
//解析节点属性
CXMLNode *parse_node_element_attr(const string cxml, CXMLNode *root)
{
    string str = cxml;
    string attr_name, attr_value;
    CXMLNode_attr *attrs = new CXMLNode_attr();
    string _str = str.substr(str.find(root->name) - 1, str.find('>') + 1);
    size_t equal_pos = 0;
    while (_str.find('=') != equal_pos && _str.find('=') < maxLength)
    {
        equal_pos = _str.find('=');
        _str = _str.substr(_str.find(' '));
        attr_name = _str.substr(0, _str.find('='));
        strip(attr_name);
        _str = _str.substr(_str.find('\"') + 1);
        attr_value = _str.substr(0, _str.find('\"'));
        strip(attr_value);
        attrs->attributes.insert(std::pair<string, string>(attr_name, attr_value));
        attrs->nums++;

        //cout << "属性名:" << attr_name << " "<< "属性值:" << attr_value << std::endl;
        if (_str.find('=') > _str.length())
            break;
    }
    root->attr = attrs;
    return root;
}
//解析节点名称
CXMLNode *parse_node_element_name(const string cxml, CXMLNode *root)
{
    size_t blank_pos = cxml.find(" ");
    size_t l_pos = cxml.find("<");
    size_t r_pos = cxml.find(">");

    if (l_pos > maxLength || r_pos > maxLength)
    {
        CXML_PARSER_STATUS = CXML_SYNTAX_ERROR;
        return nullptr;
    }
    string name;

    if (blank_pos < r_pos && blank_pos < maxLength)
    {
        name = cxml.substr(l_pos + 1, blank_pos - 1);
    }
    else
    {
        name = cxml.substr(l_pos + 1, r_pos - 1);
    }

    if (name.find("/") < name.length() || name.empty())
        name = find_rigth_element_name(cxml);
    root->name = name;
    return root;
}

//解析节点
CXMLNode *parse_node(const string cxml, CXMLNode *root)
{
    //std::puts("==========");
    string str = cxml;
    strip(str);
    if (str.find("<") > maxLength)
        return nullptr;
    try
    {
        root = parse_node_element_name(str, root);
        root = parse_node_element_attr(str, root);
        root = parse_node_element_text(str, root);
    }
    catch (const std::exception &e)
    {
        CXML_PARSER_STATUS = CXML_SYNTAX_ERROR;
        cout << "解析异常" << std::endl;
        std::cerr << e.what() << '\n';
        return nullptr;
    }

    //如果解析名称为单口标签
    //cout << root->name << " " << st.top()->name << std::endl;

    if (root->name.find(st.top()->name) < root->name.length() && st.top()->name != "")
    {
        strip(str);
        str.erase(0, str.find("</" + root->name + ">") + root->name.length() + 3);
        //cout << "出栈" << root->name << std::endl;
        CXMLNode *brother = new CXMLNode();
        //st.top()->parent->children.push_back(brother);
        st.pop();
        //cout << "栈顶元素:" << st.top()->name << std::endl;
        parse_node(str, brother);
    }
    else
    {
        strip(str);
        str = str.substr(str.find(">") + 1);
        //cout << str << std::endl;
        CXMLNode *child = new CXMLNode();
        root->parent = st.top();

        //cout << "栈顶元素:" << st.top()->name << std::endl;
        st.top()->children.push_back(root);
        //解析单口标签
        if (is_open(root->name) == true)
        {
            str.substr(str.find(">") + 1);
        }
        else
        {
            st.push(root);
            //cout << "入栈:" << root->name << std::endl;
        }

        //root->children.push_back(child);
        parse_node(str, child);
    }
    return root;
}
//从字符串解析xml
CXMLNode *parse_from_string(const string cxml)
{
    //如果为空直接返回异常
    if (cxml.length() == 0)
    {
        CXML_PARSER_STATUS = CXML_CONTENT_EMPTY;
        return nullptr;
    }
    string str = cxml;
    //删除DOCTYPE这一行
    if (str.find("!DOCTYPE") < maxLength)
    {
        str = str.substr(str.find(">") + 1);
    }
    //删除xml这一行
    if (str.find("?xml") < maxLength)
    {
        str = str.substr(str.find(">") + 1);
    }
    //开始解析
    CXMLNode *root = new CXMLNode();
    CXMLNode *head = new CXMLNode();
    root->parent = head;
    st.push(head);
    head->children.push_back(parse_node(str, root));
    return head;
}

map<CXMLNode *, bool> used;
//dfs搜索
CXMLNode *search(const string target, CXMLNode *root)
{
    //cout << (root->name == target) << std::endl;
    if (root->name == target)
    {
        return root;
    }
    for (auto m : root->children)
    {
        if (used.count(m) == 0)
        {
            used.insert({m, true});
            CXMLNode *result = search(target, m);
            if (result != nullptr)
                return result;
            used.erase(m);
        }
    }
    return nullptr;
}