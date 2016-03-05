#ifndef U_ERROR_H
#define U_ERROR_H
#include <QtWidgets>

namespace Error
{

class QueryError
{
    QString text;
public:
    QueryError(QString queryContent):text(queryContent) {}
    inline QString data() {return text;}
};

}

#endif // U_ERROR_H
