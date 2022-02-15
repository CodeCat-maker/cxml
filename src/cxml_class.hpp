#ifndef __CXMLCLASS__H_
#define __CXMLCLASS__H_
#include <cstring>
#include <iostream>
using std::__1::string;

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
//私有继承基类的属性
class CXMLNode_attr : private Node
{
private:
    string value; //子节点值
public:
    CXMLNode_attr();
};
class CXMLNode : public Node
{
public:
    string content;            //节点包含的内容
    int content_length;        //内容长度
    Node *parent;              //节点的父节点指针
    Node *children;            //节点的子节点指针
    CXMLNode_attr *attributes; //包含子节点的指针
public:
    CXMLNode();
};

#endif