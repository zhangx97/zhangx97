#include "Script/widget.h"

#include "Script/StateMachine/config_and_state.h"
#include "Script/Log/log.h"
//#include "Script/Print3DControler/projection_widget.h"
#include "Script/Print3DControler/xml_operation.h"
#include "programcore.h"
#include "Script/WifiSearch/wifisearch.h"
#include "Script/SettingPageItem/settingpageitem.h"
#include "Script/DeviceMessage/devicemessage.h"
#include "Script/Network/qmlnetmodel.h"
#include "Script/MaterialSync/materialsync.h"
//#include "Script/SlicePackList/showslicepack.h"
#include "Script/Print3DControler/qmlcontrol.h"
#include "Script/StateMachine/qmlstate.h"
#include "Script/AdjustPlane/adjustplane.h"
#include "Script/Upate/update.h"
#include "Script/ScreenProtect/screenprotect.h"
#include "Script/Log/logcontrol.h"
#include "Script/Print3DControler/projectphoto.h"

#include "Script/OpenDoor/opendoor.h"
#include "Script/SlicePackList/newshowslicepack.h"
#include "Script/SlicePackList/readmodelpack.h"
#include "Script/Udisk/udisktransfer.h"

#include <QApplication>
#include <QObject>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QString>
#include <libgen.h>
#include <unistd.h>

using namespace LOG;

int main(int argc, char *argv[])
{
//    QApplication a(argc, argv);

    chdir(dirname(argv[0]));
    initLogging();
    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    app.setOverrideCursor(QCursor(Qt::BlankCursor));

    qmlRegisterType<WifiSearch>("wifi.search",1,0,"WifiModel");
    qmlRegisterType<SettingPageItem>("setting.item",1,0,"SettingItemModel");
    qmlRegisterType<DeviceMessage>("device.item",1,0,"DeviceItemModel");
    qmlRegisterType<qmlNetModel>("net.State",1,0,"NetStateModel");
    qmlRegisterType<MaterialSync>("material.value",1,0,"MaterialModel");
//    qmlRegisterType<ShowSlicePack>("slicePack.item",1,0,"SlicePackModel");
    qmlRegisterType<QMLControl>("qmlControl.item",1,0,"QMLControl");
    qmlRegisterType<QMLState>("qmlState.item",1,0,"QMLState");
    qmlRegisterType<AdjustPlane>("adjustPlaneVoltage",1,0,"AdjustPlane");
    qmlRegisterType<Update>("softUpdate",1,0,"UpdateSoft");
    qmlRegisterType<ScreenProtect>("screenProtect",1,0,"ScreenSaver");
    qmlRegisterType<LogControl>("logControl",1,0,"ShowLog");
    qmlRegisterType<ProjectPhoto>("photoProject",1,0,"PhotoProj");

    qmlRegisterType<OpenDoor>("qmlOpenDoor",1,0,"QmlOpenDoor");
    qmlRegisterType<ShowSlicePack>("qmlShowSlicePack",1,0,"QmlShowSlicePack");
    qmlRegisterType<ReadModelPack>("qmlReadModelPack",1,0,"QmlReadModelPack");
    qmlRegisterType<UdiskTransfer>("qmlUdiskTransfer",1,0,"QmlUdiskTransfer");

    InitializeConfig();

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}

