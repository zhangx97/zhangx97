#ifndef UNZIP_H
#define UNZIP_H

#include <QString>
#include <QObject>
#include <QProcess>

class Unzip:public QObject
{
    Q_OBJECT
public:
    Unzip();
    ~Unzip();

    void UnzipImgFile(QString filePath,QString targetPath,bool needPassword = false);
    static bool UzipSuccess;
    void ReadOutput();
    QProcess *zipProcess;

private:
};
#endif // UNZIP_H
