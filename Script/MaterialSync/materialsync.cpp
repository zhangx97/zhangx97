#include "materialsync.h"

MaterialSync::MaterialSync(QObject *parent):QAbstractListModel (parent)
{
    qDebug()<<"material init";
    roleName[NameRole] = "materialName";
    roleName[LightRole] = "materialLight";

    materialXml = new XMLOperation("../config/realmakerSetting.config");
    Getmaterial();
    MaterialSQL = new SQL();
    RMKID = ConfigAndState::printID;
    materialNameList<<"WD010-GDLX"<<"M200-Z-DWQ"<<"M200-A-LSY"<<"M200-Z-XFMX"<<"M200-Z-ZZMX"<<"SG4002-ZZDB"<<"WD010-ZJLX"<<"GIN01-RZYY";
}

MaterialSync::~MaterialSync()
{
    int count = materialList.count();
    if(count>0)
    {
        for (int i=0;i<count;i++)
        {
            delete  materialList.at(i);
        }
    }
    materialList.clear();
    delete materialXml;
    delete MaterialSQL;
}

int MaterialSync::rowCount(const QModelIndex &parent) const{
    return materialList.count();
}

QVariant MaterialSync::data(const QModelIndex &index, int role) const{
    int row = index.row();
    if(row < 0 || row > materialList.count())
    {
        return QVariant();
    }
    QVector<QString> *material = materialList[row];
    return material->at(role-Qt::UserRole);
}

QHash<int,QByteArray> MaterialSync::roleNames() const{
    return roleName;
}

void MaterialSync::Getmaterial(){
    qDebug()<<"打开材料亮度页面";
    QVector<QVector<QString> *> tempMaterial;
    materialXml->GetAllMaterial();
    tempMaterial=materialXml->allMaterial;
    for (int i=0;i< tempMaterial.count();i++)
    {
        materialList.append(tempMaterial[i]);
    }
}

void MaterialSync::startSync()
{
    qDebug()<<"开始材料同步";
    for (int i = 0;i<materialNameList.count();i++) {
        MaterialSQL->Open();
        QString queryText = QString("select * from ModuleMaterialBrightness where material = '%1' and RMKID = '%2' order by modifydate desc")
                .arg(materialNameList[i]).arg(RMKID);
        result = MaterialSQL->
                Query(
                    queryText
                    );
        int materialName = result.record().indexOf("material");
        int lightIndex = result.record().indexOf("brightness");
        if(result.first())
        {
            QString material = result.value(materialName).toString();
            int brightness = result.value(lightIndex).toInt();
            materialXml->WriteXMLData(material,QString::number(brightness));
        }
        MaterialSQL->Close();
    }
    updateMaterial();
}

void MaterialSync::updateMaterial()
{
        beginResetModel();
        materialXml->clear();
        int count = materialList.count();
        qDebug()<<"材料同步页面，材料列表数量："<<count;
        if(count > 0)
        {
            materialList.clear();
        }
        qDebug()<<"重新载入材料亮度";
        Getmaterial();
        endResetModel();
}
