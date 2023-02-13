#ifndef GETFILE_H
#define GETFILE_H
#include <QString>

class GetFile
{
public:
    GetFile();
    void save(QByteArray &);

    QString fileName = "";
    QString fileCreateDate = "";
    int packSize = 0;
    int packCount = 0;
    int lastPackSize = 0;

};

#endif // GETFILE_H
