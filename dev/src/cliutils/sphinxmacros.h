#ifndef SPHINXMACROS_H
#define SPHINXMACROS_H

#define LINE1(totalsize)                                                                            \
{                                                                                                   \
    out << "+";for(int i=0;i<totalsize;i++) out << "-";out << "+";out << endl;                      \
}

#define LINE2(col1,col2)                                                                            \
{                                                                                                   \
    out << "+";                                                                                     \
    for(int i=0;i<col1;i++)                                                                         \
    out << "-";                                                                                     \
    out << "+";                                                                                     \
    for(int i=0;i<col2;i++)                                                                         \
    out << "-";                                                                                     \
    out << "+";                                                                                     \
    out << endl;                                                                                    \
}

#define LINETITLE2(col1,col2)                                                                       \
{                                                                                                   \
    out << "+";                                                                                     \
    for(int i=0;i<col1;i++)                                                                         \
    out << "=";                                                                                     \
    out << "+";                                                                                     \
    for(int i=0;i<col2;i++)                                                                         \
    out << "=";                                                                                     \
    out << "+";                                                                                     \
    out << endl;                                                                                    \
}

#define LINE3(col1,col2,col3)                                                                       \
{                                                                                                   \
    out << "+";                                                                                     \
    for(int i=0;i<col1;i++)                                                                         \
    out << "-";                                                                                     \
    out << "+";                                                                                     \
    for(int i=0;i<col2;i++)                                                                         \
    out << "-";                                                                                     \
    out << "+";                                                                                     \
    for(int i=0;i<col3;i++)                                                                         \
    out << "-";                                                                                     \
    out << "+";                                                                                     \
    out << endl;                                                                                    \
}

#define LINETITLE3(col1,col2,col3)                                                                  \
{                                                                                                   \
    out << "+";                                                                                     \
    for(int i=0;i<col1;i++)                                                                         \
    out << "=";                                                                                     \
    out << "+";                                                                                     \
    for(int i=0;i<col2;i++)                                                                         \
    out << "=";                                                                                     \
    out << "+";                                                                                     \
    for(int i=0;i<col3;i++)                                                                         \
    out << "=";                                                                                     \
    out << "+";                                                                                     \
    out << endl;                                                                                    \
}

#define LINECONTENT1(content,size)                                                                  \
{                                                                                                   \
    out << "|";                                                                                     \
    out << content;                                                                                 \
    for(int i=content.length();i<size;i++)                                                          \
    out << " ";                                                                                     \
    out << "|";                                                                                     \
    out << endl;                                                                                    \
}

#define LINECONTENT2(content1,size1,content2,size2)                                                 \
{                                                                                                   \
    out << "|";                                                                                     \
    out << content1;                                                                                \
    for(int i=content1.length();i<size1;i++)                                                        \
    out << " ";                                                                                     \
    out << "|";                                                                                     \
    out << content2;                                                                                \
    for(int i=content2.length();i<size2;i++)                                                        \
    out << " ";                                                                                     \
    out << "|";                                                                                     \
    out << endl;                                                                                    \
}

#define LINECONTENT3(content1,size1,content2,size2,content3,size3)                                  \
{                                                                                                   \
    out << "|";                                                                                     \
    out << content1;                                                                                \
    for(int i=content1.length();i<size1;i++)                                                        \
    out << " ";                                                                                     \
    out << "|";                                                                                     \
    out << content2;                                                                                \
    for(int i=content2.length();i<size2;i++)                                                        \
    out << " ";                                                                                     \
    out << "|";                                                                                     \
    out << content3;                                                                                \
    for(int i=content3.length();i<size3;i++)                                                        \
    out << " ";                                                                                     \
    out << "|";                                                                                     \
    out << endl;                                                                                    \
}

#endif // SPHINXMACROS_H
