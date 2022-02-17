#include "src/parser.hpp"
#include "src/xpath.hpp"
extern int CXML_PARSER_STATUS; //解析状态

int main()
{
    using std::cout;
    using std::endl;

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
    if (CXML_PARSER_STATUS == CXML_SYNTAX_ERROR)
    {
        std::puts(">解析异常");
        return 0;
    }
    std::puts(">解析成功");
    const CXMLNode_result *result = xpath("/book", root);

    // CXMLNode *result = search("book", root);
    // cout << result->children.size() << endl;
    // using std::puts;
    // puts("解析到的属性:");
    // if (result->attr != nullptr)
    // {
    //     for (auto m : result->attr->attributes)
    //     {
    //         //属性
    //         cout << "key:" << m.first << " value:" << m.second << endl;
    //     }
    // }
    // //
    // puts("解析子节点 及节点值");
    // for (auto m : result->children)
    // {
    //     //节点名称
    //     cout << m->name << ":" << m->text->content << endl;
    // }
    return 0;
}
