#include "sql.h"

SQL::SQL()
{
    db = new QSqlDatabase();
    *db = QSqlDatabase::addDatabase("QODBC");
    QString dsn = QString("Driver={FreeTDS};Server=rm-bp1280w63ltml17t6o.sqlserver.rds.aliyuncs.com,3433;Database=RMKDB;uid=realmaker;pwd=shxd0825GO");
    db->setDatabaseName(dsn);
}

SQL::~SQL()
{
    if(db->isOpen())
    {
        db->close();
    }
    delete db;
}

QSqlQuery SQL::Query(QString query)
{
    QSqlQuery qsQuery = QSqlQuery(*db);
    QString strSqlText(query);
    qsQuery.prepare(strSqlText);
    bool ok = qsQuery.exec();
    if(ok)
    {
        return qsQuery;
    }
    else
    {
        qDebug()<<"no";
        return qsQuery;
    }

}

bool SQL::Open()
{
    return db->open();
}

bool SQL::Close()
{
    db->close();
    if(db->isOpen())
    {
        return false;
    }
    return true;
}
