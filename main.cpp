#include "src/parser.hpp"

int main()
{

    CXMLNode *root = parse_from_string("\
        <bookstore>\n\
            <book category=\"CHILDREN\">\n\
                <title>Harry Potter</title>\n\
                <author>J K.Rowling</author>\n\
                <year>2005 < / year >\n\
                <price>29.99 </price>\n\
            </book>\n\
            <book category=\"WEB\">\n\
                <title>Learning XML</title>\n\
                <author>Erik T.Ray</author>\n\
                <year>2003 </year>\n\
                <price>39.95 </price>\n\
            </book>\n\
        </bookstore>");
    return 0;
}