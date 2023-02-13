#include "Script/Print3DControler/xml_operation.h"
//#include <QFile>
//#include <QDebug>
//#include "Script/StateMachine/config_and_state.h"
//#include <QApplication>

XMLOperation::XMLOperation(QString path):path(path)
{
//    InitXML();
}

XMLOperation::~XMLOperation()
{
//    int count = materialInfo->count();
//    if (count != 0)
//    {
//        materialInfo->clear();
//    }
//    delete materialInfo;
}

void XMLOperation::CreateXML()
{
    QFile file(path);
    bool bl = file.open(QIODevice::ReadOnly);
    if(bl)
    {
        //文件存在的话就返回，不存在的话创建一个标准格式的xml文件
        return;
    }
    else
    {
        file.open(QIODevice::WriteOnly);
        QDomDocument doc;
        QDomProcessingInstruction ins = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"utf-8\"");
        doc.appendChild(ins);

        QDomElement root = doc.createElement("Realmaker配置文件");
        doc.appendChild(root);

        QTextStream stream(&file);
        doc.save(stream, 4);
        file.close();
    }
}

bool XMLOperation::WriteXMLData(QString elementname,QString textValue="",QDomElement *rootElement)
{
    if(rootElement == NULL)
    {
        InitXML();
    }
    if(initXMLFlag)
    {
        QDomElement root;
        if(rootElement == NULL)
        {
            root = m_doc.documentElement();
        }
        else
        {
            root = *rootElement;
        }

        //根节点下没有子节点
        if(!root.hasChildNodes())
        {
            QDomElement dateElem = m_doc.createElement(elementname);   // 创建节点
            QDomText text = m_doc.createTextNode(textValue);
            dateElem.appendChild(text);
            root.appendChild(dateElem);//添加到根节点下方
        }
        //根节点下存在子节点
        else
        {
            for(QDomNode n = root.firstChild(); !n.isNull(); n = n.nextSibling())
            {
                QDomElement e = n.toElement();
                //子节点是一个父节点
                if(e.hasChildNodes() && e.tagName() == "材料对应亮度")
                {
                    if(WriteXMLData(elementname,textValue,&e))
                    {
                        return true;
                    }
                    else
                    {
                        continue;
                    }
                }
                else
                {
                    //该节点是目标节点
                    if(e.tagName() == elementname)
                    {
                        QDomNode textNode = e.lastChild();
                        e.removeChild(textNode);
                        QDomText text = m_doc.createTextNode(textValue);
                        e.appendChild(text);
                        return WriteXML();
                    }
                    else
                    {
                        continue;
                    }
                }

            }
            //根节点下不为空但无该节点
            if(rootElement == NULL)
            {
                QDomElement dateElem = m_doc.createElement(elementname);   // 创建节点
                QDomText text = m_doc.createTextNode(textValue);
                dateElem.appendChild(text);
                root.appendChild(dateElem);//添加到根节点下方
                // 将内存数据写入文件
                return WriteXML();
            }
            return false;
        }

    }
}

bool XMLOperation::WriteXML()
{
    QFile file(path);
    bool bl = file.open(QIODevice::WriteOnly);
    if(!bl)
    {
        initXMLFlag = false;
        return false;
    }
    QTextStream stream(&file);
    m_doc.save(stream, 4);
    file.close();
    initXMLFlag = false;
    return true;
}

void XMLOperation::InitXML()
{
    m_doc = QDomDocument();
    QFile file(path);
    if(file.open(QIODevice::ReadOnly))
    {
       bool bl = m_doc.setContent(&file);//初始化doc
       if(bl)
       {
           initXMLFlag = true;
           file.close();
           return;
       }
       file.close();
       initXMLFlag = false;
    }
    else
    {
       initXMLFlag = false;
    }
}

QString XMLOperation::ReadXMLText(QString targetName)
{
    InitXML();
    if(initXMLFlag)
    {
       QDomElement root = m_doc.documentElement();
       return StartRead(targetName,root);
    }/*else {
        InitXML();
        QDomElement root = m_doc.documentElement();
        return StartRead(targetName,root);
    }*/
}

QString XMLOperation::StartRead(QString targetName,QDomElement root)
{
    QString text;
    QString resultOfNest = "";     //用来返回嵌套的值
    //遍历当前根的子节点
    for(QDomNode n = root.firstChild(); !n.isNull(); n = n.nextSibling())
    {
        if(n.firstChildElement().tagName() != "")
        {
            //子节点还不是最小一级，则嵌套
            resultOfNest = StartRead(targetName,n.toElement());
        }
        else
        {
            //子节点已经是最小一级，则取值
            QDomElement e = n.toElement();
            if(e.tagName() == targetName)
            {
                QDomText t = n.lastChild().toText();
                if (!t.isNull())
                {
                    text = t.data();
                    return text;
                }
                else
                {
                    return "";
                }
            }
        }
    }
    return resultOfNest;
}

QVector<QVector<QString> *> XMLOperation::GetAllMaterial(){
    InitXML();
    if(initXMLFlag){
        QDomElement root = m_doc.documentElement();
        for (QDomNode n = root.firstChild();!n.isNull();n=n.nextSibling()) {
            QDomElement parent = n.toElement();
            if(parent.tagName() == "材料对应亮度"){
                materialInfo *materialMessage;
                for (QDomNode child = parent.firstChild();!child.isNull();child = child.nextSibling()) {
//                    signalChild *getSignalMaterial = new signalChild();
//                    QVector<QString> *SignalMaterialBright = new QVector<QString>();
                     materialMessage = new QVector<QString>();
                    //获取材料名称
                    QDomElement nameElement = child.toElement();
//                    getSignalMaterial->append(nameElement.tagName());
                    materialMessage->append(nameElement.tagName());

                    //获取材料亮度值
                    QDomText t = nameElement.lastChild().toText();
//                    getSignalMaterial->append(t.data());
                    materialMessage->append(t.data());

                    allMaterial.append(materialMessage);
                    materialMessage = 0;
//                    allMaterial.append(getSignalMaterial);
                }
                return allMaterial;
            }
        }
    }
}

void XMLOperation::clear()
{
    int count = allMaterial.count();
    if(count > 0)
    {
        for (int i=0;i<count;i++)
        {
            delete allMaterial.at(i);
        }
        allMaterial.clear();
    }
}
