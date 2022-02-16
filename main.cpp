#include "src/parser.hpp"

int main()
{

    CXMLNode *root = parse_from_string("\
        <bookstore company=\"codecat\" boss=\"man\">\n\
            <book category=\"CHILDREN\">\n\
                <title>Harry Potter</title>\n\
                <author>J K.Rowlingk</author>\n\
                <year>2005</year>\n\
                <price>29.99 </price>\n\
            </book>\n\
        </bookstore>");
    // std::cout << root->children->content;
    return 0;
}
// <book category=\"WEB\">\n\
//                 <title>Learning XML</title>\n\
//                 <author>Erik T.Ray</author>\n\
//                 <year>2003 </year>\n\
//                 <price>39.95 </price>\n\
//             </book>\n\