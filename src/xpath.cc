#include "xpath.hpp"
using std::cout;
using std::endl;
using std::__1::pair;
using std::__1::queue;
typedef queue<pair<string, string> > qpss;
qpss queue_option; //储存操作类型和名称
int XPATH_PARSE_STATUE = XPATH_PARSE_SUCCESS;
//child 子
//genera 后
const string options[] = {
    "get_parent_node",                // /.. 选择父元素 ✅
    "get_this_node",                  // /. 选择当前元素 ✅
    "get_all_nodes",                  // /* 匹配任意元素
    "get_node_from_genera_by_name",   // //name 选择当前元素后代元素中的name元素 ✅
    "get_node_from_child_by_name",    // /name 选择当前元素子代元素中的name元素 ✅
    "get_node_by_array_and_name",     // /name[n] 选择当前元素下第n个name元素 ✅
    "get_node_by_attr_and_name",      // /name[@attr] 属性筛选，选择有attr属性的元素 ✅
    "get_node_by_attrValue_and_name", // /name[@attr=value] 属性筛选，选择attr属性的值为value的name元素(属性值不加分号) ✅
    "get_text_from_this",             // /text() 返回当前元素中的文本 ✅
    "get_texts_from_genera",          // //text() 返回当前元素以及它所有后代元素中的文本 ✅
    "get_attr_from_this",             // /@attr 返回当前元素attr属性的值 ✅
    "get_all_attr"                    // @* 匹配任意属性};
};
// string转int
constexpr unsigned int
str2int(const char *str, int h = 0)
{
    return !str[h] ? 5381 : (str2int(str, h + 1) * 33) ^ str[h];
}

pair<string, string> parse_option(const string op)
{
    pair<string, string> ret;
    //取后代
    if (op.find("//") < maxLength)
    {

        if (op.find("text()") < maxLength)
        {
            ret = {options[9], op.substr(2)};
        }
        else
        {
            ret = {options[3], op.substr(2)};
        }
    }
    else
    {
        //取子代
        if (op.find("..") < maxLength)
        {
            ret = {options[0], ""};
        }
        else if (op.find(".") < maxLength)
        {
            ret = {options[1], ""};
        }
        else if (op.find("[") < maxLength)
        {
            if (op.find("@") < maxLength)
            {
                if (op.find("=") < maxLength)
                {
                    ret = {options[7], op.substr(1)};
                }
                else
                {
                    ret = {options[6], op.substr(1)};
                }
            }
            else
            {
                ret = {options[5], op.substr(1)};
            }
        }
        else if (op.find("@") < maxLength)
        {
            ret = {options[10], op.substr(1)};
        }
        else if (op.find("text()") < maxLength)
        {
            ret = {options[8], op.substr(1)};
        }
        else
        {
            ret = {options[4], op.substr(1)};
        }
    }
    //cout << ret.first << " " << ret.second << endl;
    return ret;
}
//返回父节点
CXMLNode *xpath_get_parent_node(CXMLNode *root)
{
    return root->parent;
}
//返回当前节点
CXMLNode *xpath_get_this_node(CXMLNode *root)
{
    return root;
}
//获取当前节点文本
string xpath_get_text_from_this(CXMLNode *root)
{
    return root->text->content;
}
//获取当前节点后节点的所有文本
string xpath_get_texts_from_genera(CXMLNode *root)
{
    pair<CXMLNode *, bool> d;
    queue<CXMLNode *> q;
    q.push(root);
    string ret;
    while (!q.empty())
    {
        auto p = q.front();
        q.pop();
        for (auto m : p->children)
        {
            auto t = m->text;
            ret += t->content + " ";
            q.push(m);
        }
    }
    return ret;
}
//根据属性 值 名称 来选择节点
CXMLNode *xpath_get_node_by_attrValue_and_name(const string name, CXMLNode *root)
{
    string attr_name = name.substr(name.find("@") + 1, name.find("=") - name.find("@") - 1);
    string value_name = name.substr(name.find("=") + 1, name.find("]") - name.find("=") - 1);
    string node_name = name.substr(0, name.find("["));
    //cout << attr_name << " " << node_name << " " << value_name << endl;
    for (auto child : root->children)
    {
        CXMLNode_attr *att = child->attr;
        if (child->name == node_name)
        {
            for (auto item : att->attributes)
            {
                if (item.first == attr_name && item.second == value_name)
                {

                    return child;
                }
            }
        }
    }
    return nullptr;
}
//根据属性 in过程 选择节点
CXMLNode *xpath_get_node_by_attr_and_name(const string name, CXMLNode *root)
{
    string attr_name = name.substr(name.find("@") + 1, name.find("]") - name.find("@") - 1);
    string node_name = name.substr(0, name.find("["));
    for (auto child : root->children)
    {
        CXMLNode_attr *att = child->attr;
        if (child->name == node_name)
        {
            for (auto item : att->attributes)
            {
                if (item.first == attr_name)
                    return child;
            }
        }
    }
    return nullptr;
}
// 通过数组选择与名字相同的节点
CXMLNode *xpath_get_node_by_array_and_name(const string name, CXMLNode *root)
{
    int j = *(name.substr(name.find("[") + 1, name.find("]") - name.find("[")).c_str()) - '0';
    string tmp_name = name.substr(0, name.find("["));
    cout << tmp_name << " " << j << " " << endl;
    int i = 0;
    for (auto child : root->children)
    {
        //cout << child->name << endl;
        if (child->name == tmp_name)
        {

            i++;
            if (i == j)
                return child;
        }
    }
    return nullptr;
}
//选择当前元素子代元素中的name元素
CXMLNode *xpath_get_node_from_child_by_name(const string name, CXMLNode *root)
{
    if (root->children.size() == 0)
        return nullptr;
    for (auto child : root->children)
    {
        if (child->name == name)
            return child;
    }
    return nullptr;
}

map<CXMLNode *, bool> used;
//选择当前元素后代元素中的name元素
CXMLNode *xpath_get_node_from_genera_by_name(const string name, CXMLNode *root)
{
    if (root->name == name)
    {
        return root;
    }
    for (auto m : root->children)
    {
        if (used.count(m) == 0)
        {
            used.insert({m, true});
            CXMLNode *result = xpath_get_node_from_genera_by_name(name, m);
            if (result != nullptr)
                return result;
            used.erase(m);
        }
    }
    return nullptr;
}

string xpath_get_attr_from_this(const string name, CXMLNode *root)
{
    string attr_name = name.substr(name.find("@") + 1);
    string ret;
    CXMLNode_attr *a = root->attr;
    for (auto m : a->attributes)
    {
        if (m.first == attr_name)
            return m.second;
    }

    return ret;
}
//将操作入队 双指针算法入队
bool get_xpath_option(const string exp)
{
    int l(0), r(0);
    int len = 0;

    while (len <= exp.length())
    {

        if ((exp[len] == '/'))
        {
            if (exp[len + 1] == '/')
                r = l + 2;
            else
                r = l + 1;
            while (r <= exp.length())
            {
                if (exp[r] == '/')
                    break;
                r++;
            }
            string tmp_option = exp.substr(l, r - l);
            //cout << tmp_option << " ";
            queue_option.push(parse_option(tmp_option));
        }
        len = r;
        l = r;
    }
    //string name = exp.substr(0, len);
    return true;
}

//处理xpath操作
bool do_xpath_option(CXMLNode *root, CXMLNode_result *result)
{
    CXMLNode *node = root;
    string ret_text;
    while (queue_option.empty() == false)
    {
        pair<string, string> op = queue_option.front();
        queue_option.pop();
        string option = op.first;
        string name = op.second;
        //cout << option << " " << name << endl;
        switch (str2int(option.c_str()))
        {
        case str2int("get_node_from_genera_by_name"):
            node = xpath_get_node_from_genera_by_name(name, node);
            result->element = node;
            break;
        case str2int("get_node_from_child_by_name"):
            node = xpath_get_node_from_child_by_name(name, node);
            result->element = node;
            break;
        case str2int("get_node_by_array_and_name"):
            node = xpath_get_node_by_array_and_name(name, node);
            result->element = node;
            break;
        case str2int("get_node_by_attr_and_name"):
            node = xpath_get_node_by_attr_and_name(name, node);
            result->element = node;
            break;
        case str2int("get_node_by_attrValue_and_name"):
            node = xpath_get_node_by_attrValue_and_name(name, node);
            result->element = node;
            break;
        case str2int("get_text_from_this"):
            ret_text = xpath_get_text_from_this(node);
            result->text = ret_text;
            return true;
        case str2int("get_texts_from_genera"):
            ret_text = xpath_get_texts_from_genera(node);
            result->text = ret_text;
            return true;
        case str2int("get_this_node"):
            node = xpath_get_this_node(node);
            result->element = node;
            break;
        case str2int("get_parent_node"):
            node = xpath_get_parent_node(node);
            result->element = node;
            break;
        case str2int("get_attr_from_this"):
            ret_text = xpath_get_attr_from_this(name, node);
            result->text = ret_text;
            return true;
        default:
            return false;
        }
    }
    return true;
}

//返回xpath解析内容
const CXMLNode_result *xpath(const string exp, CXMLNode *root)
{
    const char *ptr = exp.c_str();
    CXMLNode_result *ret = new CXMLNode_result();
    try
    {
        get_xpath_option(exp);
        do_xpath_option(root, ret);
    }
    catch (const std::exception &e)
    {
        XPATH_PARSE_STATUE = XPATH_SYNTAX_ERROR;
        return nullptr;
    }
    return ret;
}