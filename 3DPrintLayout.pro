#-------------------------------------------------
#
# Project created by QtCreator 2021-05-27T13:45:38
#
#-------------------------------------------------

QT       += core gui xml quick
QT       += sql
QT       += serialport
QT       += network
QT       += multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Realmaker
TEMPLATE = app

target.path = /home/pi/python_project/core
INSTALLS += target

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_MESSAGELOGCONTEXT
DEFINES += QT_DEPRECATED_WARNINGS


# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
    Script/HeaterControl/heatercontrol.cpp \
    Script/Log/logcontrol.cpp \
    Script/MaterialSync/materialsync.cpp \
    Script/Network/TCP/getfile.cpp \
    Script/Network/qmlnetmodel.cpp \
    Script/Print3DControler/projectphoto.cpp \
    Script/Print3DControler/qmlcontrol.cpp \
    Script/SQL/sql.cpp \
    Script/ScreenProtect/screenprotect.cpp \
    Script/SettingPageItem/settingpageitem.cpp \
    Script/SlicePackList/showslicepack.cpp \
    Script/StateMachine/qmlstate.cpp \
    Script/Upate/update.cpp \
    Script/lightEngine/lcdlight.cpp \
    Script/lightEngine/light_engine.cpp \
    Script/lightEngine/cyusbserialcontroller.cpp \
    Script/lightEngine/cyusbseriallib.cpp \
    Script/Log/log.cpp \
    Script/Network/HttpUpdate/http_update.cpp \
    Script/Network/NetworkActiveCheck/network_active_check.cpp \
    Script/Network/SerialPort/serialport.cpp \
    Script/Network/TCP/tcpserver.cpp \
    Script/Network/TCP/tcpsocket.cpp \
    Script/Network/TCP/message_deal.cpp \
    Script/Network/UDP/udpnetwork.cpp \
    Script/Pressure/pluse_calibrate.cpp \
    Script/Print3DControler/unzip.cpp \
    Script/Print3DControler/xml_operation.cpp \
    Script/Print3DControler/print_control.cpp \
    Script/Print3DControler/img_mask.cpp \
    Script/Print3DControler/temperature_read.cpp \
    Script/ShowVideo/video_player.cpp \
    Script/ShowVideo/video_frames.cpp \
    Script/ShowVideo/temp_get_frame.cpp \
    Script/StateMachine/config_and_state.cpp \
    Script/main.cpp \
    Script/widget.cpp \
    Script/CheckUpdate/checkupdate.cpp \
    Script/AdjustPlane/adjustplane.cpp \
    Script/AdjustPlane/scrollbar.cpp \
    Script/AdjustPage/adjustpage.cpp \
    Script/DeviceMessage/devicemessage.cpp \
    Script/WifiSearch/wifisearch.cpp \
    Script/WifiSearch/wifitable.cpp \
    Script/WifiSearch/wificonnect.cpp \
    Script/programcore.cpp

HEADERS += \
    Script/HeaterControl/heatercontrol.h \
    Script/Log/logcontrol.h \
    Script/MaterialSync/materialsync.h \
    Script/Network/TCP/getfile.h \
    Script/Network/qmlnetmodel.h \
    Script/Print3DControler/projectphoto.h \
    Script/Print3DControler/qmlcontrol.h \
    Script/SQL/sql.h \
    Script/ScreenProtect/screenprotect.h \
    Script/SettingPageItem/settingpageitem.h \
    Script/SlicePackList/showslicepack.h \
    Script/StateMachine/qmlstate.h \
    Script/Upate/update.h \
    Script/lightEngine/lcdlight.h \
    Script/lightEngine/light_engine.h \
    Script/lightEngine/CyUSBSerial.h \
    Script/lightEngine/cyusbserialcontroller.h \
    Script/lightEngine/cyusbseriallib.h \
    Script/Log/log.h \
    Script/Network/HttpUpdate/http_update.h \
    Script/Network/NetworkActiveCheck/network_active_check.h \
    Script/Network/SerialPort/serialport.h \
    Script/Network/TCP/tcpserver.h \
    Script/Network/TCP/tcpsocket.h \
    Script/Network/TCP/message_deal.h \
    Script/Network/UDP/udpnetwork.h \
    Script/Pressure/pluse_calibrate.h \
    Script/Print3DControler/unzip.h \
    Script/Print3DControler/xml_operation.h \
    Script/Print3DControler/print_control.h \
    Script/Print3DControler/img_mask.h \
    Script/Print3DControler/temperature_read.h \
    Script/ShowVideo/video_player.h \
    Script/ShowVideo/video_frames.h \
    Script/ShowVideo/temp_get_frame.h \
    Script/StateMachine/config_and_state.h \
    Script/widget.h \
    Script/CheckUpdate/checkupdate.h \
    Script/AdjustPlane/adjustplane.h \
    Script/AdjustPlane/scrollbar.h \
    Script/AdjustPage/adjustpage.h \
    Script/DeviceMessage/devicemessage.h \
    Script/WifiSearch/wifisearch.h \
    Script/WifiSearch/wifitable.h \
    Script/WifiSearch/wificonnect.h \
    Script/programcore.h

FORMS += \
        widget.ui \
    Script/CheckUpdate/checkupdate.ui \
    Script/AdjustPlane/adjustplane.ui \
    Script/AdjustPage/AdjustPage.ui \
    Script/DeviceMessage/devicemessage.ui \
    Script/WifiSearch/wifisearch.ui \
    Script/WifiSearch/wificonnect.ui

# Default rules for deployment.
#qnx: target.path = /tmp/$${TARGET}/bin
#else: unix:!android: target.path = /opt/$${TARGET}/bin
#!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc \

QMAKE_LFLAGS += -Wl,--rpath=.


INCLUDEPATH += $$PWD/../../../../usr/local/include
DEPENDPATH += $$PWD/../../../../usr/local/include

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../usr/local/lib/release/ -lcyusbserial
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../usr/local/lib/debug/ -lcyusbserial
#else:unix: LIBS += -L$$PWD/../../../../usr/local/lib/ -lcyusbserial

INCLUDEPATH += $$PWD/../../CyUSBSerial_SDK_Linux/common/header
DEPENDPATH += $$PWD/../../CyUSBSerial_SDK_Linux/common/header

unix:!macx: LIBS += -L$$PWD/../../../../opt/qt5pi/sysroot/usr/local/lib/ -lcyusbserial

INCLUDEPATH += $$PWD/../../../../opt/qt5pi/sysroot/usr/local
DEPENDPATH += $$PWD/../../../../opt/qt5pi/sysroot/usr/local

LIBS += -lwiringPi
LIBS += -lwiringPiDev
