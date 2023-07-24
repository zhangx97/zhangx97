#include "Script/Network/TCP/message_deal.h"
#include <QList>
#include <QByteArray>
#include <stdlib.h>
#include <QApplication>

MessageDeal::MessageDeal()
{

}

MessageDeal::~MessageDeal()
{
    delete TcpSocket;
}

void MessageDeal::DealOrder(QString message,qintptr descriptor)
{
    QString order = message;
    QList<QString> paramList;
    paramList<<"SVN"<<"LockState"<<"Heater"<<"TargetLight"<<"ModelType"<<"Debug"<<"LockState";

    XMLOperation * operationXML = new XMLOperation("../config/realmakerSetting.config");

    if (order != "state" && !order.contains("PackName"))
    {
    }

    if (order == "state")
    {
        ReturnStateToPC(descriptor);
        return;
    }

    if (order == "help")
    {
        ReturnHelpMessage(descriptor);
        return;
    }

    if (order == ConfigAndState::GetPrintState())
    {
        return;
    }

    QString printerControlMessage;
    if (order != "")
    {
        printerControlMessage = order.split(" ")[0];
    }
    else
    {
        printerControlMessage = "";
        return ;
    }

    qDebug()<<"printerControlMessage : " << printerControlMessage;
    if(printerControlMessage == "continue")      //继续打印
    {
//        QByteArray tempChange = printerControlMessage.toUtf8()+"!";
//        emit OrderResultSignal(tempChange,descriptor);
        qDebug()<<"上位机要求继续打印";
//        ConfigAndState::SetPrintState(printerControlMessage);
//        emit ContinuePrintSignal();
    }
    else if(printerControlMessage == "stop")        //停止打印
    {
        qDebug()<<"上位机要求停止打印";
//        ConfigAndState::SetPrintState(printerControlMessage);

//        QByteArray tempChange = printerControlMessage.toUtf8()+"!";
//        emit OrderResultSignal(tempChange,descriptor);
        //emit StopPrintSignal();
    }
    else if(printerControlMessage == "pause")       //暂停打印
    {
//        QByteArray tempChange = printerControlMessage.toUtf8()+"!";
//        emit OrderResultSignal(tempChange,descriptor);

        qDebug()<<"上位机要求暂停打印";
//        ConfigAndState::SetPrintState(printerControlMessage);
        //emit PausePrintSignal();
    }
    else if(printerControlMessage == "start")       //开始打印
    {

        qDebug()<<"上位机要求开始打印";
        qDebug()<<"fileName : " + fileName;
        QFile fileInfo("../slicepack/images.zip");
        QString slicepackfileName = "../slicepack/" + fileName;
        qDebug()<<"slicepackfileName : " + slicepackfileName;
        QString unzipSlicepackfileName = "../unzipSlicepack/" + fileName.split(".")[0];
        qDebug()<<"unzipSlicepackfileName : " + unzipSlicepackfileName;
//        bool fileComplete = ConfigAndState::CheckPackExist("../slicepack/",fileName,fileSize);
//        if (fileComplete)
//        {
//            qDebug()<<"存在重名文件";
//            system(QString("sudo rm %1 -rf").arg(slicepackfileName).toUtf8());
//        }
        system(QString("sudo rm %1 -rf").arg(slicepackfileName).toUtf8());
        system(QString("sudo rm %1 -rf").arg(unzipSlicepackfileName).toUtf8());

        fileInfo.rename(slicepackfileName);

        QString unzipFileNoSuffix = "../unzipSlicepack/"+fileName.split(".")[0];
        qDebug()<<"创建文件夹的位置："<<unzipFileNoSuffix;
        QDir makeDir(unzipFileNoSuffix);
        makeDir.mkdir(unzipFileNoSuffix);
        QString dirpath = makeDir.absolutePath();
        qDebug()<<"解压文件夹："<<dirpath;
        system(QString("sudo unzip -P realmaker123 %1 -d %2").arg(slicepackfileName).arg(dirpath).toUtf8());
        QString pngpath = dirpath + "/" + "0*.png";
        qDebug()<<"删除切层图："<<pngpath;
        system(QString("sudo rm %1").arg(pngpath).toUtf8());
        /*---------------------------------判断slicepack文件是否超过最大数量-----------------------------------*/
        ConfigAndState::deleteExtraFile("ZZDB*.zip");
        ConfigAndState::deleteExtraFile("XFMX*.zip");
//        QStringList CurrentAllNameFilters;
//        CurrentAllNameFilters << "*.zip";
//        QDir dir("../slicepack/");
//        QStringList CurrentAllFiles = dir.entryList(CurrentAllNameFilters,QDir::Files|QDir::Readable,QDir::Time);
//        int deletenum = CurrentAllFiles.count() - ConfigAndState::slicepackMaxNum;
//        if (deletenum > 0)
//        {
//            for (int j = 0; j < deletenum; j++)
//            {
//                QString deleteSlicepackFile = QString("../slicepack/%1.zip").arg(CurrentAllFiles[CurrentAllFiles.count()-j-1].split(".")[0]);
//                QString deleteUnzipSlicepackDir = QString("../unzipSlicepack/%1").arg(CurrentAllFiles[CurrentAllFiles.count()-j-1].split(".")[0]);
//                system(QString("sudo rm %1 -rf").arg(deleteSlicepackFile).toUtf8());
//                qDebug() << "电脑端发送slicepack,删除slicepack目录下旧的超过最大文件限制的文件 :" << QString("sudo rm %1 -rf").arg(deleteSlicepackFile);
//                system(QString("sudo rm %1 -rf").arg(deleteUnzipSlicepackDir).toUtf8());
//                qDebug() << "电脑端发送slicepack,删除slicepack目录下旧的超过最大文件限制的文件夹 :" << QString("sudo rm %1 -rf").arg(deleteUnzipSlicepackDir);
//            }

//        }
        /*---------------------------------判断slicepack文件是否超过最大数量-----------------------------------*/
//        if(ConfigAndState::PrintInitState == 1)
//        {
//            fileComplete = ConfigAndState::CheckPackExist("../slicepack/","images.zip",fileSize);
//            //存在则重命名该文件
//            QFile fileInfo("../slicepack/images.zip");
//            fileInfo.rename(fileName);
//        }
//        else if(ConfigAndState::PrintInitState == 0)
//        {
//            fileComplete = ConfigAndState::CheckPackExist("../slicepack/","images.zip");
//            if(fileComplete)
//            {
//                filePath= "";
//            }
//            else
//            {
//                qWarning()<<"未找到上位机发送的打印文件";
//                return;
//            }
//        }
//        if(fileComplete)
//        {
//            QByteArray tempChange = printerControlMessage.toUtf8()+"!";
//            emit OrderResultSignal(tempChange,descriptor);

//            QString printState = ConfigAndState::GetPrintState();
//            if(printState == "stop" || printState =="NULL")
//            {
//                emit StartPrintSignal(filePath);
//                ConfigAndState::SetPrintState(printerControlMessage);
//            }
//        }
//        else {
//            qWarning()<<"本地未找到对应的打印文件";
//        }
        return;
    }
    else if(printerControlMessage == "cancel")      //python那边也啥都没写，不知道怎么回事，先放着
    {
        QByteArray tempChange = printerControlMessage.toUtf8()+"!";
        emit OrderResultSignal(tempChange,descriptor);
    }
    else if(printerControlMessage == "inputfile")       //解压缩树莓派上的文件，为了兼容性而保留
    {
        QByteArray tempChange = printerControlMessage.toUtf8()+"!";
        emit OrderResultSignal(tempChange,descriptor);
    }
    else if(paramList.contains(printerControlMessage))//参数修改
    {
        ChangeParament(printerControlMessage,order.split(" ")[1]);
        QByteArray tempChange = printerControlMessage.toUtf8()+"!";
        emit OrderResultSignal(tempChange,descriptor);
    }
    else if(printerControlMessage == "reboot")
    {
        QByteArray tempChange = printerControlMessage.toUtf8()+"!";
        emit OrderResultSignal(tempChange,descriptor);
        RestartProgram();
    }
    else if(printerControlMessage == "clear")//清除报错
    {
        ConfigAndState::printException = "";
    }
    else if(printerControlMessage == "shutdown")
    {
        QByteArray tempChange = printerControlMessage.toUtf8()+"!";
        emit OrderResultSignal(tempChange,descriptor);
        ShutdownProgram();
    }
    else if(printerControlMessage == "version")//返回光机和设备版本
    {
        GetVersion(descriptor);
    }
    else if(printerControlMessage == "LightList")//更新材料亮度
    {
        XMLOperation materialRecord("../config/realmakerSetting.config");
        materialRecord.WriteXMLData(order.split(" ")[1],order.split(" ")[2]);
        QByteArray tempChange = printerControlMessage.toUtf8()+"!";
        emit OrderResultSignal(tempChange,descriptor);
    }
    else if(printerControlMessage == "PrintRecord")
    {

    }
    else if(printerControlMessage == "still")       //拍照的，又没摄像头，不知道有什么用，先停了
    {
        QByteArray tempChange = printerControlMessage.toUtf8()+"!";
        emit OrderResultSignal(tempChange,descriptor);
    }
    else if(printerControlMessage == "addrecord")
    {
        QByteArray tempChange = printerControlMessage.toUtf8()+"!";
        emit OrderResultSignal(tempChange,descriptor);
    }
    else if (printerControlMessage == "delrecord")
    {
        QByteArray tempChange = printerControlMessage.toUtf8()+"!";
        emit OrderResultSignal(tempChange,descriptor);
    }
    else if(printerControlMessage == "revrecord")
    {
        QByteArray tempChange = printerControlMessage.toUtf8()+"!";
        emit OrderResultSignal(tempChange,descriptor);
    }
    else if (printerControlMessage == "projection")
    {
        lightengine = new CyUSBSerialLib(ConfigAndState::lightSensorType);
        lightengine->SetProjectorOnOff(true);

        QString imgName = order.split(" ")[1];
        QString imgPath = QString("../projection/%1").arg(imgName);
        emit ProjectionIMG(imgPath);

        QByteArray tempChange = printerControlMessage.toUtf8()+"!";
        emit OrderResultSignal(tempChange,descriptor);
    }
    else if (printerControlMessage == "upload")     //固件升级，完成,未测试
    {
        qDebug()<<"升级";
        QString fileName = order.split(" ")[1];
        bool isOk  = Upload(fileName);
        if(isOk)
        {
            QByteArray tempChange = QString("UploadOk!").toUtf8();
            emit OrderResultSignal(tempChange,descriptor);
        }
        else
        {
//            QByteArray tempChange = printerControlMessage.toUtf8()+"Fail!";
            QByteArray tempChange = QString("UploadFail!").toUtf8();
            emit OrderResultSignal(tempChange,descriptor);
        }
    }
    else if (printerControlMessage == "LED")//LED on off
    {
        QString LEDState = order.split(" ")[1];
        qDebug()<<LEDState;
        lightengine = new CyUSBSerialLib(ConfigAndState::lightSensorType);
        if(LEDState == "ON")
        {
            lightengine->SetProjectorOnOff(true);
            lightengine->AutoSetValue(ConfigAndState::targetLight,ConfigAndState::lightValueOffset);
        }
        else if(LEDState == "OFF")
        {
            lightengine->SetLedOnOff(false);
            lightengine->SetProjectorOnOff(false);
        }
        else if(LEDState == "up")
        {
            lightengine->SetLedOnOff(true);
        }
        else if(LEDState == "down")
        {
            lightengine->SetLedOnOff(false);
        }
        delete lightengine;
    }
    else if(printerControlMessage == "display")     //完成，未测试,也是图片投影
    {
        QString imgName = order.split(" ")[1];
        qDebug() << "imgName : " << imgName;
        QString imgPath = QString("../projection/%1").arg(imgName);
        qDebug() << "imgPath : " << imgPath;
        emit ProjectionIMG(imgPath);

        QByteArray tempChange = printerControlMessage.toUtf8()+"!";
        emit OrderResultSignal(tempChange,descriptor);
    }
    else if (printerControlMessage == "adjust")//兼容用的调平，也可以在屏幕上直接调
    {

        QByteArray tempChange = printerControlMessage.toUtf8()+"!";
        qDebug() << "adjust order : " << order;
        QByteArray cmd = order.mid(7).toUtf8();
        qDebug() << "adjust cmd : " << cmd;
        AdjustPlatform(cmd);
        emit OrderResultSignal(tempChange,descriptor);
    }
    else if(printerControlMessage == "pluseCalibrate")//压力校准
    {
        QByteArray tempChange = printerControlMessage.toUtf8()+"!";
        emit OrderResultSignal(tempChange,descriptor);
        PluseCalibrate();
    }
    else if(printerControlMessage.contains("PackName"))//检查文件名是否存在
    {
        QString fileMessage = printerControlMessage.split(":")[1];
        QStringList nameAndSize=fileMessage.split("-");
        QString fileName= nameAndSize[0].split(".")[0]+QString(".zip");
        int fileSize = nameAndSize[1].toInt();
        if (ConfigAndState::CheckPackExist("../slicepack/",fileName,fileSize))
        {
            emit FileTransSignal("reject",descriptor);
        }
        else
        {
            emit FileTransSignal("accept",descriptor);
        }
    }
    else if(printerControlMessage.contains("initPrint"))//打印前准备工作
    {
        QString fileMessage = printerControlMessage.split(":")[1];
        if (InitPrint(fileMessage))
        {
            emit FileTransSignal("reject",descriptor);
        }
        else
        {
            emit FileTransSignal("accept",descriptor);
        }
    }
    else if(printerControlMessage == "getBottomTime")//打印前准备工作
    {
        QString modeType = order.split(" ")[1];
        QString modeBottomTime = "";
        if (modeType == "MX")
        {
            modeBottomTime = operationXML->ReadXMLText("MX底片固化时间");
        }
        else if (modeType == "YG")
        {
            modeBottomTime = operationXML->ReadXMLText("YG底片固化时间");
        }
        else if (modeType == "DB")
        {
            modeBottomTime = operationXML->ReadXMLText("DB底片固化时间");
        }
        else if (modeType == "ZJ")
        {
            modeBottomTime = operationXML->ReadXMLText("ZJ底片固化时间");
        }
        else if (modeType == "YY")
        {
            modeBottomTime = operationXML->ReadXMLText("YY底片固化时间");
        }
        QByteArray tempChange = (printerControlMessage + " " + modeBottomTime).toUtf8() + "!";
        qDebug() << "getBottomTime order : " << tempChange;
        emit OrderResultSignal(tempChange,descriptor);
    }
    else if(printerControlMessage == "scaling")//打印前准备工作
    {

        QString PCMagnification = operationXML->ReadXMLText("应用模型放大倍数");
        QByteArray tempChange = (printerControlMessage + " " + PCMagnification).toUtf8()+"!";
        qDebug() << "scaling order : " << tempChange;
        emit OrderResultSignal(tempChange,descriptor);
    }
    else if(printerControlMessage == "StandardScaling")//打印前准备工作
    {
        QString StandardMagnification = operationXML->ReadXMLText("标准模型放大倍数");
        QByteArray tempChange = (printerControlMessage + " " + StandardMagnification).toUtf8()+"!";
        qDebug() << "scaling order : " << tempChange;
        emit OrderResultSignal(tempChange,descriptor);
    }
    else
    {
//        QByteArray tempChange = printerControlMessage.toUtf8()+"!";
        QByteArray tempChange = "Unknown command!";
        qDebug()<<"收到未知命令："+printerControlMessage;
        emit OrderResultSignal(tempChange,descriptor);
    }
    delete operationXML;
}

void MessageDeal::UTF8ToGB2312(QByteArray &code)
{
    QTextCodec* utf8Codec= QTextCodec::codecForName("utf-8");
    QTextCodec* gb2312Codec = QTextCodec::codecForName("gb2312");

    QString strUnicode= utf8Codec ->toUnicode(code);
    QByteArray ByteGb2312= gb2312Codec ->fromUnicode(strUnicode);

    code= ByteGb2312;
}

//向排版软件返回当前系统状态
void MessageDeal::ReturnStateToPC(qintptr descriptor)
{
    QMap<QString,QString> systemState;
    QList<QString> listOfSendMessage;
    listOfSendMessage<<"CurrentCount"<<"Heater"<<"PrintState"<<"ModelName"<<"LightTarget"
                    <<"ResinCuringTime"<<"LightCurrent"<<"LayerThickness"<<"PrintTime"
                   <<"Exception"<<"ModelCount"<<"BottomCount"<<"LockState"<<"BottomCuringTime"
                  <<"LightEngineSN"<<"MovementTime"<<"CurTemperature"<<"SVN"<<"ModelType"
                 <<"CurCurrent"<<"Material";

    systemState.insert("ResinCuringTime",QString::number(ConfigAndState::resinCuringTime,10,1));
            systemState.insert("ModelCount",QString::number(ConfigAndState::modelCount));
    systemState.insert("LayerThickness",QString::number(ConfigAndState::layerThickness,10,5));
    systemState.insert("ModelName",QString(ConfigAndState::modelName));
    systemState.insert("BottomCount",QString::number(ConfigAndState::bottomCount));
    systemState.insert("PrintTime",QString(ConfigAndState::printStartTime));     //说是打印时间，其实是是打印开始时间，而且根本没有生效，也不知道是干嘛用的
    systemState.insert("Material",QString(ConfigAndState::material));
    systemState.insert("BottomCuringTime",ConfigAndState::bottomCuringTimes.join(","));
    systemState.insert("PrintState",QString(ConfigAndState::GetPrintState()));
    systemState.insert("Exception",QString(ConfigAndState::printException));
    systemState.insert("CurrentCount",QString::number(ConfigAndState::GetFinishCount()));
    systemState.insert("LightTarget",QString::number(ConfigAndState::targetLight));
    systemState.insert("Heater",QString(ConfigAndState::heater));     //打印的温度
    systemState.insert("LightCurrent",QString::number(ConfigAndState::curBrightness));  //当前的亮度
    systemState.insert("CurCurrent",QString::number(ConfigAndState::curCurrent));     //当前电流
    systemState.insert("SVN",QString(ConfigAndState::printID));     //设备序列号
    systemState.insert("LockState",QString(ConfigAndState::isLocker?"True":"False"));     //大概是和电动门有关，目前来说是没有用了
    systemState.insert("LightEngineSN",QString(ConfigAndState::lightengineSN));
    systemState.insert("MovementTime",QString::number(ConfigAndState::movementTime,10,5));     //移动速度，路程除以转速
    systemState.insert("CurTemperature",QString::number(ConfigAndState::curLEDTemp));
    systemState.insert("ModelType",QString::number(ConfigAndState::modelType));

    QMap<QString,QString>::const_iterator i;
    messageNeedToSend =QString("state!");

    for(int i=0;i<listOfSendMessage.length();i++ )
    {
        if(systemState.contains(listOfSendMessage[i]))
        {
            messageNeedToSend += listOfSendMessage[i]+": "+systemState.find(listOfSendMessage[i]).value()+"\n";
        }else
        {
            qWarning()<<QString("未能找到值：%1").arg(listOfSendMessage[i]);
        }
    }

    QByteArray tempChange = messageNeedToSend.toUtf8();
    emit OrderResultSignal(tempChange,descriptor);
}
//回传帮助信息
void MessageDeal::ReturnHelpMessage(qintptr descriptor)
{
    QMap<QString,QString> helpMessage;
    QList<QString> listOfHelp;
    QString helpMessageToSend = "help!";
    listOfHelp<<"start"<<"continue"<<"stop"<<"cancel"<<"state"<<"inputfile";
    helpMessage.insert("start","print start");
    helpMessage.insert("continue","continue print");
    helpMessage.insert("stop","stop print");
    helpMessage.insert("cancel","cancel last print");
    helpMessage.insert("state","state information");
    helpMessage.insert("inputfile","unzip upload file");

    for(int i = 0;i<listOfHelp.length();i++)
    {
        if(helpMessage.contains(listOfHelp[i]))
        {
            helpMessageToSend = listOfHelp[i]+": "+helpMessage.find(listOfHelp[i]).value()+"\n";
        }else
        {
            qWarning()<<QString("未能找到值：%1").arg(listOfHelp[i]);
        }
    }
    QByteArray tempChange = helpMessageToSend.toUtf8();
    emit OrderResultSignal(tempChange,descriptor);
}

void MessageDeal::RestartProgram()
{
    system("sudo reboot");
}

void MessageDeal::ShutdownProgram()
{
    system("sudo halt");
}

bool MessageDeal::Upload(QString fileName)
{
    int docName = fileName.lastIndexOf("/");
    QString title = fileName.right(fileName.length()-docName-1);
    QString titleCut = title.mid(0,6);
    if(titleCut=="RMK_TF")
    {
        qDebug()<<"升级被取消，该机型没有感应门";
        return false;
    }
    QStringList titlelist = title.split(".");
    if(titlelist[1] == "hex")
    {
        QString upCmd = "sudo /usr/share/arduino/hardware/tools/avrdude -C/usr/share/arduino/hardware/tools/avrdude.conf -v -patmega2560 -cwiring";
        QString fileCmd = QString("-b115200 -D -Uflash:w:%1:i").arg(fileName);
        if(titleCut == "RealMa")
        {
            QString serialName;
            XMLOperation SerialName("./config/realmakerSetting.config");
            serialName = SerialName.ReadXMLText("串口名");

            QString devCmd = QString("-P%1").arg(serialName);
            bool updateOk = system(upCmd.append(" ").append(devCmd).append(" ").append(fileCmd).toUtf8());
            return updateOk;
        }
    }
    else if(titlelist[1] == "zip")
    {
        QProcess *pPrecess = new QProcess(this);
        pPrecess->startDetached("../update/update");

        QThread::sleep(1);

        QApplication* app;
        app->exit(0);

        return true;
    }
}

void MessageDeal::GetVersion(qintptr descriptor)
{
    XMLOperation Version("../config/realmakerSetting.config");
    QString helpMessageToSend = "version!rmkVersion:"+Version.ReadXMLText("主程序版本")+"nvmVersion: "+Version.ReadXMLText("光机版本");
    QByteArray tempChange = helpMessageToSend.toUtf8();
    emit OrderResultSignal(tempChange,descriptor);
}

void MessageDeal::AdjustPlatform(QByteArray cmd)
{
    SerialPort AdjustCmd;
    AdjustCmd.SendPort(cmd);
}

void MessageDeal::ChangeParament(QString parament,QString value)
{
    XMLOperation ParamentChange("../config/realmakerSetting.config");
    if(value == "SVN")
    {
        ParamentChange.WriteXMLData("序列号",value);
    }
    else if(value == "Heater")
    {
        ParamentChange.WriteXMLData("温度",value);
    }
    else if(value == "TargetLight")
    {
        ParamentChange.WriteXMLData("目标亮度",value);
    }
    else if(value == "ModelType")
    {
        ParamentChange.WriteXMLData("机型",value);
    }
    else
    {
        qDebug()<<"不支持将参数:"<<parament<<" 修改为:"<<value;
    }

}

void MessageDeal::PluseCalibrate()
{
    SerialPort PluseCalibrate;
    ConfigAndState::pluseCount = 1300000;
    bool m18 = PluseCalibrate.SendPort("M18\n");
    bool g28;
    if(m18)
    {
        qDebug()<<"已经回到最高点，开始校正压力";
        PluseCalibrate.SendPort("G15\n");
        g28 = PluseCalibrate.SendPort("G28\n");
    }
}

bool MessageDeal::InitPrint(QString packName)
{
    QString path = "../slicepack/";
    QStringList nameAndSize=packName.split("-");
    fileName = nameAndSize[0].split(".")[0]+QString(".zip");
    filePath = path+fileName;
    fileSize = nameAndSize[1].toInt();
    QFileInfo fileInfo(filePath);
    if(fileInfo.isFile())
    {
        QString size = QString::number(fileInfo.size());
        if(size == fileSize)
        {
            ConfigAndState::PrintInitState = 2;
            return  true;
        }
    }
    ConfigAndState::PrintInitState = 1;
    return false;
}
