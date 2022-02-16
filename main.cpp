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
