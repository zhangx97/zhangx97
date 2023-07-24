#ifndef XMLOPERATION_H
#define XMLOPERATION_H

#include <QString>
#include <QXmlStreamReader>
#include <QDomDocument>
#include <QTextStream>
#include <QFile>
#include <QDebug>

//typedef QVector<QString> signalChild;
//实际上xml类型的文件只有一个modelpack,为了追求速度用流式传输，所以其实不用一个类
class XMLOperation
{
public:
    XMLOperation(QString);
    ~XMLOperation();

    void CreateXML();//创建一个xml文档
    bool WriteXMLData(QString elementname,QString textValue,QDomElement *rootElement = NULL);
//    bool WriteXML();
    void InitXML();
    void clear();

    QString ReadXMLText(QString targetName);

    typedef QVector<QString> materialInfo;
    QVector<QVector<QString> *>allMaterial;
    QVector<QVector<QString> *> GetAllMaterial();

    QString GetTargetLight(QString targetMaterial);

//    void GetAllMaterial();

private:
    QDomDocument m_doc;//存在于内存中的，可修改的xml文件
    QString path;
    bool initXMLFlag = false;//xml文件是否可以正确打开
    QString StartRead(QString,QDomElement);
    bool WriteXML();

};
#endif // XMLOPERATION_H
