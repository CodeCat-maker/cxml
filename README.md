# CXML ![](https://img.shields.io/badge/license-MIT-blue)![](https://img.shields.io/badge/language-c%2B%2B-blue)![](https://img.shields.io/badge/version-2.0-blue)

c++ 解析 xml 文档 支持 xpath 语法

- [CXML](#cxml-----https---imgshieldsio-badge-license-mit-blue-----https---imgshieldsio-badge-language-c-2b-2b-blue-----https---imgshieldsio-badge-version-20-blue-)
  - [前言](#--)
- [如何使用](#----)
  - [下载安装](#----)
  - [使用 DEMO](#---demo)
  - [支持的 xpath 语法](#----xpath---)
  - [Cmakelists](#cmakelists)
  - [编译](#--)
  - [结果](#--)
- [设计](#--)
  - [1.主逻辑](#1---)
    - [1.读入](#1--)
    - [2.解析](#2--)
    - [2.如何解析](#2----)
    - [3. 如何建立 XML 树](#3------xml--)
    - [4.如何解析 xpath 语法](#4-----xpath---)
      - [思路：](#---)
        - [常量名](#---)
        - [双指针算法入队](#-------)
        - [switch 处理队列操作](#switch-------)
        - [bfs 算法](#bfs---)
        - [dfs 搜索](#dfs---)
    - [5.用到的数据结构](#5-------)
    - [6.用到的 STL 容器](#6----stl---)
      - [6.用到的算法](#6-----)
  - [2.后期完善工作](#2------)
- [欢迎 Fork 和 Pr](#---fork---pr)

<small><i><a href='http://ecotrust-canada.github.io/markdown-toc/'>Table of contents generated with markdown-toc</a></i></small>

## 前言

因为我叫 codecat，所以我的 XML 就叫 CXML

这次设计这个代码主要是学完 c++基础的，想结合数据结构和 c++基础的东西来做一个比较实际的项目

# 如何使用

## 下载安装

```
git clone https://github.com/CodeCat-maker/cxml.git
```

## 使用 DEMO

```c++
#include "src/parser.hpp"
#include "src/xpath.hpp"
extern int CXML_PARSER_STATUS; //解析状态
extern int XPATH_PARSE_STATUE;
int main()
{
    using std::cout;
    using std::endl;
    clock_t start, end;
    start = clock();
    CXMLNode *root = parse_from_string("\
        <bookstore company=\"codecat\" boss=\"man\">\n\
            <book category=\"CHILDREN\">\n\
                <title>Harry Potter</title>\n\
                <author>J K.Rowlingk</author>\n\
                <year>2005</year><br>\n\
                <price>29.99 </price>\n\
            </book>\n\
            <book category=\"WEB\">\n\
                <title>Learning XML</title>\n\
                <author>Erik T.Ray</author>\n\
                <year>2003 </year>\n\
                <price>39.95 </price>\n\
            </book>\n\
        </bookstore>");
    //cout << root->children.size() << endl;
    if (CXML_PARSER_STATUS == CXML_SYNTAX_ERROR)
    {
        std::puts(">xml解析异常");
        return 0;
    }
    else
    {
        std::puts(">xml解析成功");
    }

    const CXMLNode_result *result1 = xpath("/bookstore/book[@category=CHILDREN]/@category//text()", root);
    const CXMLNode_result *result2 = xpath("/bookstore/book/title/../price/text()", root);
    if (XPATH_PARSE_STATUE == XPATH_SYNTAX_ERROR)
    {
        std::puts(">xpath解析异常");
        return 0;
    }
    else
    {
        std::puts(">xpath解析成功");
    }
    cout << "测试样例1:" << result1->text << endl;
    cout << "测试样例2:" << result2->text << endl;

    end = clock();
    cout << "\n\n函数运行花费:" << (double)(end - start) / CLOCKS_PER_SEC << "秒";
    return 0;
}
```

## 支持的 xpath 语法

- /name 选择当前元素子元素中的 name 元素
- //name 选择当前元素后代元素中的 name 元素
- /. 选择当前元素
- /.. 选择父元素
- /name[@attr=value] 属性筛选，选择 attr 属性的值为 value 的 name 元素(属性值不加分号)
- /name[@attr] 属性筛选，选择有 attr 属性的元素
- /name[n] 选择当前元素下第 n 个 name 元素
- /text() 返回当前元素中的文本
- /@attr 返回当前元素 attr 属性的值
- //text() 返回当前元素以及它所有后代元素中的文本
- //@attr 返回当前元素以及它所有后代元素中 attr 属性的值

## Cmakelists

```cmake
cmake_minimum_required(VERSION 3.13)

project(cxml)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(src)

add_executable(cxml main.cpp)

target_link_libraries(cxml CxmlFunction)
```

## 编译

```
cd build
cmake .
make ..
```

## 结果

```
>xml解析成功
>xpath解析成功
测试样例1:CHILDREN
测试样例2:   Harry Potter J K.Rowlingk 2005  29.99  Learning XML Erik T.Ray 2003  39.95

函数运行花费:0.000312秒
```

# 设计

## 1.主逻辑

### 1.读入

1.从文档读入

2.从字符串读入

### 2.解析

1.解析当前标签名

2.解析标签属性

3.解析标签值

4.构建 xml 树

故文档结构就是这个样子了:

```
cxml.hpp
cxml_class.cc
cxml_class.hpp
xpath.hpp
xpath.cc
cxml.cc
parser.hpp
```

### 2.如何解析

```c++
CXMLNode *parse_from_string(const string cxml);

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
```

到这里，其实 XML 中建立 XML 树这部分的思路就是这样了。剩下的就是如何解析字符串和如何建立树的问题了

解析字符串很简单 就不详细说明了

### 3. 如何建立 XML 树

首先我们来看一个例子：

```xml
        <bookstore company=\"codecat\" boss=\"man\">\n\
            <book category=\"CHILDREN\">\n\
                <title>Harry Potter</title>\n\
                <author>J K.Rowlingk</author>\n\
                <year>2005</year>\n\
                <price>29.99 </price>\n\
            </book>\n\
            <book category=\"WEB\">\n\
                <title>Learning XML</title>\n\
                <author>Erik T.Ray</author>\n\
                <year>2003 </year>\n\
                <price>39.95 </price>\n\
            </book>\n\
        </bookstore>"
```

那么接下来就是如何建立这棵树了

![](https://i.bmp.ovh/imgs/2022/02/f99c226b653a1ac8.png)

我在这里建树的思路是：

建立一个 stack 堆，将标签<name>都丢进去，一层一层，如果遇到</name>的，就将 stack 栈顶 pop 出来，代表当前节点已经废弃，并且入栈的时候按照的是父子关系，因为如果有不是子的就已经 pop 出去了，所以我们很容易建立一个关系

递归处理

```c++
CXMLNode *parse_node(const string cxml, CXMLNode *root)
{
    std::puts("==========");
    string str = cxml;
    strip(str);
    if (str.find("<") > maxLength)
        return nullptr;
    root = parse_node_element_name(str, root);
    root = parse_node_element_attr(str, root);
    root = parse_node_element_text(str, root);
    //如果解析名称为单口标签
    //cout << root->name << " " << st.top()->name << std::endl;
    if (is_open(root->name) == true)
    {
        //递归下一个
    }
    if (root->name.find(st.top()->name) < root->name.length() && st.top()->name != "")
    {
        strip(str);
        str.erase(0, str.find("</" + root->name + ">") + root->name.length() + 3);
        cout << "出栈" << root->name << std::endl;
        CXMLNode *brother = new CXMLNode();
        //st.top()->parent->children.push_back(brother);
        st.pop();
        cout << "栈顶元素:" << st.top()->name << std::endl;
        parse_node(str, brother);
    }
    else
    {
        strip(str);
        str = str.substr(str.find(">") + 1);
        //cout << str << std::endl;
        CXMLNode *child = new CXMLNode();
        root->parent = st.top();
        cout << "栈顶元素:" << st.top()->name << std::endl;
        st.top()->children.push_back(root);
        st.push(root);
        cout << "入栈:" << root->name << std::endl;

        //root->children.push_back(child);
        parse_node(str, child);
    }
    return root;
}
```

### 4.如何解析 xpath 语法

#### 思路：

从左到右用双指针算法将操作名称和操作对象名放入队列中，利用队列先进先出的特性，依次进行解析

##### 常量名

```c++
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
```

##### 双指针算法入队

```c++
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
```

##### switch 处理队列操作

```c++
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
```

##### bfs 算法

搜索全部文本名称

```c++
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
```

##### dfs 搜索

搜索符合条件的元素

```c++
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
```

### 5.用到的数据结构

栈、双链表，树，队列，元组

### 6.用到的 STL 容器

vector 动态数组

map 哈希表

string 字符串

pair 元组

stack 栈

queue 队列

#### 6.用到的算法

dfs bfs 双指针

## 2.后期完善工作

1.支持 html 解析

# 欢迎 Fork 和 Pr
