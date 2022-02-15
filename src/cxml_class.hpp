#ifndef __CXMLCLASS__H_
#define __CXMLCLASS__H_
#include <cstring>
#include <iostream>
#include <map>
#include <vector>
using std::__1::map;
using std::__1::string;
using std::__1::vector;

class Node
{
public:
    string name; //节点名称
    Node *next;  //前节点指针
    Node *perv;  //后节点指针
    string getName()
    {
        return name;
    }
};
//每个节点的属性
class CXMLNode_attr : public Node
{
public:
    map<string, string> attributes;
    int nums;

public:
    CXMLNode_attr();
};
//节点
class CXMLNode : public Node
{
public:
    string content;            //节点包含内容
    Node *parent;              //节点的父节点指针
    Node *children;            //节点的子节点指针
    CXMLNode_attr *attributes; //包含子节点的指针
public:
    CXMLNode();
};

class CXMLNode_text : public Node
{
public:
    vector<string> content; //文本数组
    int lens;               //多少组文本
    CXMLNode_text();        //构造函数
};

#endif