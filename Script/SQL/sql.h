#ifndef SQL_H
#define SQL_H
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>
#include <QDebug>

class SQL
{
public:
    SQL();
    ~SQL();
    QSqlQuery Query(QString query);
    bool Open();
    bool Close();
    QSqlDatabase *db;
};

#endif // SQL_H
