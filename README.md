# CXML ![](https://img.shields.io/badge/license-MIT-blue)![](https://img.shields.io/badge/language-c%2B%2B-blue)![](https://img.shields.io/badge/version-1.0-blue)

c++ 解析 xml 文档

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

int main()
{
    using std::cout;
    using std::endl;
    CXMLNode *root = parse_from_string("\
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
        </bookstore>");
    //开始搜索
    CXMLNode *result = search("book", root);

    cout << result->children.size() << endl;
    using std::puts;
    puts("解析到的属性:");
    if (result->attr != nullptr)
    {
        for (auto m : result->attr->attributes)
        {
            //属性
            cout << "key:" << m.first << " value:" << m.second << endl;
        }
    }
    //
    puts("解析子节点 及节点值");
    for (auto m : result->children)
    {
        //节点名称
        cout << m->name << ":" << m->text->content << endl;
    }
    return 0;
}
```

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

# 设计

## 1.主逻辑

### 1.读入

​ 1.从文档读入

​ 2.从字符串读入

### 2.解析

​ 1.解析当前标签名

​ 2.解析标签属性

​ 3.解析标签值

​ 4.构建 xml 树

故文档结构就是这个样子了:

```
cxml.cc
cxml.hpp
cxml_class.cc
cxml_class.hpp
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

### 4.用到的数据结构

栈、双链表，图

### 5.用到的 STL 容器

vector 动态数组

map 哈希表

string 字符串

#### 6.用到的算法

dfs

## 2.后期完善工作

1.支持 xpath 语法

2.支持 html 解析

# 欢迎 Fork 和 Pr
