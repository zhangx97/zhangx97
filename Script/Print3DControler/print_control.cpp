#include "Script/Print3DControler/print_control.h"
#include <QDir>

PrintControl::PrintControl(QObject *parent)
    :QObject (parent)
{

}

PrintControl::~PrintControl()
{
    delete UnzipForPrint;
    delete reader;
    delete PrintCmd;
    delete projection;
}

bool PrintControl::ReadXML(QString &filePath)
{
    QFile xmlFile(filePath);
//    qDebug()<<QApplication::applicationDirPath();

    if (!xmlFile.open(QFile::ReadOnly|QFile::Text))
    {
        qDebug()<<"打印配置文件开启失败";
        return false;
    }
    reader = new QXmlStreamReader();
    reader->setDevice(&xmlFile);
    while(!reader->atEnd())
    {
        if(reader->isStartElement())
        {
            if (reader->name() == "ApplicationConfig")
            {
                qInfo()<<"状态机-modelpack初始化";
                ReadChildElement();
            }
            else
            {
                qDebug()<<"未发现属性配置子节点";
            }
        }
        else
        {
            reader->readNext();
        }
    }
    xmlFile.close();
    if (reader->hasError())
    {
        return false;
    }
    else if (xmlFile.error() != QFile::NoError)
    {
        return false;
    }
    qDebug()<<"modelpack初始化结束";
    return true;
}

void PrintControl::ReadChildElement()
{
    Q_ASSERT(reader->isStartElement() && reader->name() == "ApplicationConfig");
    reader->readNext();
    while (!reader->atEnd())
    {
        if (reader->isEndElement())
        {
            reader->readNext();
            qDebug()<<"跳出";
            break;
        }

        if (reader->isStartElement())
        {
            if(reader->name() == "count")
            {
                ConfigAndState::modelCount = reader->readElementText().toInt();
                reader->readNext();     //上一个方法设计失误，以后可以改
                reader->readNext();
            }
            else if(reader->name() == "width")
            {
                ConfigAndState::modelPicWidth = reader->readElementText().toInt();
                reader->readNext();
                reader->readNext();
            }
            else if(reader->name() == "height")
            {
                ConfigAndState::modelPicHeight = reader->readElementText().toInt();
                reader->readNext();
                reader->readNext();
            }
            else if(reader->name() == "thick")
            {
                ConfigAndState::layerThickness = reader->readElementText().toDouble();
                reader->readNext();
                reader->readNext();
            }
            else if (reader->name() == "max_z")
            {
                ConfigAndState::maxZ = reader->readElementText().toDouble();
                reader->readNext();
                reader->readNext();
            }
            else if (reader->name() == "bottom_count")
            {
                ConfigAndState::bottomCount = reader->readElementText().toInt();
                reader->readNext();
                reader->readNext();
            }
            else if (reader->name() == "bottom_expose_time")
            {
                QString bottomCuringTimes = reader->readElementText();
                ConfigAndState::bottomCuringTimes = bottomCuringTimes.split(",",QString::SkipEmptyParts);
                qDebug()<<ConfigAndState::bottomCuringTimes;
                reader->readNext();
                reader->readNext();
            }
            else if (reader->name() == "layer_expose_time")
            {
                ConfigAndState::resinCuringTime = reader->readElementText().toDouble();
                reader->readNext();
                reader->readNext();
            }
            else if (reader->name() == "model_name")
            {
                ConfigAndState::modelName = reader->readElementText();
                reader->readNext();
                reader->readNext();
            }
            else if (reader->name() == "print_time")
            {
                ConfigAndState::printStartTime = reader->readElementText();
                reader->readNext();
                reader->readNext();
            }
            else if (reader->name() == "material")
            {
                ConfigAndState::material = reader->readElementText();
                reader->readNext();
                reader->readNext();
            }
            else if (reader->name() == "volume")
            {
                ConfigAndState::modelVolume = reader->readElementText().toInt();
                reader->readNext();
                reader->readNext();
            }
            else if (reader->name() == "supportpara")
            {
                QString supportParament = reader->readElementText();
                ConfigAndState::supportParament = supportParament.split(",",QString::SkipEmptyParts);
                reader->readNext();
                reader->readNext();
            }
            else if (reader->name() == "special_expose_time")
            {
                QString specialExposeLayer = reader->readElementText();
                if(specialExposeLayer == "null")
                {
                    qDebug()<<"无特殊层，生成0数组";
                    ConfigAndState::specialExposeLayer<<"0"<<"0"<<"0";
                }
                else
                {
                    ConfigAndState::specialExposeLayer = specialExposeLayer.split(",",QString::SkipEmptyParts);
                }
                reader->readNext();
                reader->readNext();
            }
        }
        else
        {
            reader->readNext();
        }
    }

}

void PrintControl::DealStartPrint(QString filePath)
{
    qDebug()<<"打印线程的ID:"<<QThread::currentThreadId()<<"\n";
    //先解压,解压只能在linux上测试
    qDebug()<<"开始解压";

    UnzipForPrint = new Unzip();
    if(filePath == "")
    {
        UnzipForPrint->UnzipImgFile("../images.zip","../images");
    }
    else if(filePath != "")
    {
//        qDebug()<<"待打印文件名不为空";
        UnzipForPrint->UnzipImgFile(filePath,"../images",true);
    }

    emit UnzipFinish();

    //再读取参数
    QString modelPackPath = "../images/model.pack";
    bool ifReadFinish = ReadXML(modelPackPath);

    PrintCmd = new SerialPort;
    bool changeLEDBelt;
    bool arduinoStartReady;
    //解除待机

    //等待加热，加热这块需要等传感器连上再写

    //向arduino发布指令
    changeLEDBelt = PrintCmd->SendPort(ConfigAndState::startCommand[1].toUtf8());
    arduinoStartReady = PrintCmd->SendPort(ConfigAndState::startCommand[0].toUtf8());
    if(!changeLEDBelt)
    {
        qWarning()<<"串口通信遇到意外";
    }
    if(!arduinoStartReady)
    {
        qWarning("串口通信遇到意外");
        return;
    }

    projection = new CyUSBSerialLib(ConfigAndState::lightSensorType);
    bool states = projection->DeviceInit();
    bool lightReady = projection->AutoSetValue(ConfigAndState::targetLight,ConfigAndState::lightValueOffset);

    if(!lightReady)
    {
        qWarning("光机亮度未能达到预期");
        PrintCmd->SendPort(ConfigAndState::stopCommand[0].toUtf8());
        PrintCmd->SendPort(ConfigAndState::stopCommand[1].toUtf8());
        return;
    }

    //开始控制打印图层
    PrintingCount = ConfigAndState::GetFinishCount();
    modelCount = ConfigAndState::modelCount;
    bottomCount = ConfigAndState::bottomCount;

    ProjectionLayer();
}


void PrintControl::ProjectionLayer()
{
    qDebug()<<"打印开始";
    ConfigAndState::SetPrintState("start");

    while(PrintingCount<bottomCount + modelCount)
    {
        qDebug()<<"帧播放信号发送";

        emit MachineIsStartPrintNowSignal();

        if(ConfigAndState::GetPrintState() == "stop")
        {
            ControlStop();
//            emit StopShowFrameSignal();
            return;
        }
        else if(ConfigAndState::GetPrintState() == "pause")
        {
            ControlPause();
//            emit PauseShowFrameSignal();
            return;
        }
        BuildNewLayer();
        PrintingCount++;
        ConfigAndState::SetFinishCount(PrintingCount);
    }
    ControlStop();
    ConfigAndState::SetPrintState("NULL");
    qDebug()<<"打印结束";
}

void PrintControl::BuildNewLayer()
{
    double waitResinCuringTime;
    qDebug()<<QString("第 %1 层").arg(PrintingCount);
    if(PrintingCount<bottomCount)
    {
        QTime startTime = QTime::currentTime();

        waitResinCuringTime = ConfigAndState::bottomCuringTimes[PrintingCount].toDouble();
        float ascendDistance = ConfigAndState::layerThickness*PrintingCount+ConfigAndState::crackDistance;
        QString moveUpCMD = QString("G1 Z%1 F%2\n").arg(QString::number(ascendDistance,10,3)).arg(ConfigAndState::ascendSpeed);
        PrintCmd->SendPort(moveUpCMD.toUtf8());

        float descendDistance = ConfigAndState::layerThickness*(PrintingCount + 1);
        QString moveDownCMD = QString("G1 Z%1 F%2\n").arg(QString::number(descendDistance,10,3)).arg(ConfigAndState::descendSpeed);
        PrintCmd->SendPort(moveDownCMD.toUtf8());

        QTime endTime = QTime::currentTime();
        int delta = startTime.msecsTo(endTime);
        ConfigAndState::motorMoveTime = delta;
        qDebug()<<QString("平台移动一次花费的时间：%1").arg(delta);
    }
    else
    {
        //遇到特殊层的话，设置特定的打印时间
        if(PrintingCount>ConfigAndState::specialExposeLayer[0].toInt() && PrintingCount<ConfigAndState::specialExposeLayer[1].toInt())
        {
            waitResinCuringTime = ConfigAndState::specialExposeLayer[2].toDouble();
        }
        else
        {
            waitResinCuringTime = ConfigAndState::resinCuringTime;
        }
        QTime startTime = QTime::currentTime();
        float ascendDistance = ConfigAndState::layerThickness*PrintingCount+ConfigAndState::crackDistance;
        QString moveUpCMD = QString("G1 Z%1 F%2\n").arg(QString::number(ascendDistance,10,3)).arg(ConfigAndState::ascendSpeed);
        PrintCmd->SendPort(moveUpCMD.toUtf8());

        float descendDistance = ConfigAndState::layerThickness*(PrintingCount + 1);
        QString moveDownCMD = QString("G1 Z%1 F%2\n").arg(QString::number(descendDistance,10,3)).arg(ConfigAndState::descendSpeed);
        PrintCmd->SendPort(moveDownCMD.toUtf8());

        QTime endTime = QTime::currentTime();
        int delta = startTime.msecsTo(endTime);
        ConfigAndState::motorMoveTime = delta;
        qDebug()<<QString("平台移动一次花费的时间：%1 (毫秒)").arg(delta);
    }

    projectionImgPath = QString("../images/%1.png").arg(PrintingCount,4,10,QLatin1Char('0'));
    emit photoChange(projectionImgPath);
    QThread::msleep(waitResinCuringTime*1000);
    emit photoChange("");
}

void PrintControl::ControlStop()
{
    bool printSuccessStop = false;
    projectionImgPath = "";
    emit photoChange("");
    PrintingCount = 0;
    ConfigAndState::SetFinishCount(PrintingCount);
    ConfigAndState::SetPrintState("stop");
    ConfigAndState::PrintInitState = 0;
    printSuccessStop=PrintCmd->SendPort(ConfigAndState::stopCommand[0].toUtf8());
    printSuccessStop=PrintCmd->SendPort(ConfigAndState::stopCommand[0].toUtf8());

    qDebug()<<"停止打印被触发";
    if(!printSuccessStop)
    {
        emit StopPrint();
        ConfigAndState::SetPrintState("NULL");
    }
}

void PrintControl::ControlPause()
{
    projectionImgPath = "";
    emit photoChange("");
    ConfigAndState::SetPrintState("pause");
    ConfigAndState::SetFinishCount(PrintingCount);
}

void PrintControl::ControlContinue()
{
    bool state = projection->SetProjectorOnOff(true);
    if (!state)
    {
        qWarning("光机未能成功开启");
        return;
    }
    else
    {
        bool lightReady = projection->AutoSetValue(ConfigAndState::targetLight,ConfigAndState::lightValueOffset);
        if(!lightReady)
        {
            qWarning()<<"光机未能达到预期亮度";
            return;
        }
    }

    bool changeLEDBelt = PrintCmd->SendPort(ConfigAndState::startCommand[1].toUtf8());
    if(!changeLEDBelt)
    {
        qWarning()<<"串口通信遇到意外";
    }
    ProjectionLayer();
}

QString PrintControl::photoPath()
{
    return projectionImgPath;
}

void PrintControl::SetPhotoPath(const QString & photoPath)
{
    projectionImgPath = photoPath;
    emit photoChange(photoPath);
}

