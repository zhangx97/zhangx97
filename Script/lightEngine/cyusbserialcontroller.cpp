#include "Script/lightEngine/cyusbserialcontroller.h"
#include <QDebug>
#include <QThread>

enum CommandID{
    WRITE_LED_ENABLE = 0x52,    //ok
    READ_LED_ENABLE = 0x53,     //ok
    WRITE_LED_CURRENT = 0x54,   //ok
    READ_LED_CURRENT = 0x55,    //ok
    READ_TEMPERATRUE = 0xd6,    //ok
    WRITE_TEST_PATTERNS = 0x0b, //ok
    WRITE_INPUT_SOURCE = 0x05,  //ok
    WRITE_PROJCETOR_FLIP = 0x14,//ok
    READ_PROJCETOR_FLIP = 0x15, //ok
    READ_FB_VER = 0xd9,         //?
    WRITE_PWR = 0xa0,           //?
    READ_LIGHT = 0xac           //?
};

enum FlashCommandCode{
    WRITE_ENABLE = 0x06,
    WRITE_DISABLE = 0x04,
    READ_STATUS1 = 0x05,
    READ_STATUS2 = 0x35,
    WRITE_STATUS = 0x01,
    PAGE_PROGRAM = 0x02,
    READ_DATA = 0x03,
    SECTOR_ERASE = 0x20,
    BLOCK_ERASE_64KB = 0xd8
};


CyUSBSerialController::CyUSBSerialController():
    numDevices(0)
{

}


CY_RETURN_STATUS CyUSBSerialController::isCypressDevice(const quint8 &deviceNum) const
{
    CY_HANDLE handle;
    CY_RETURN_STATUS rStatus;
    quint8 interfaceNum = 0;

    rStatus = CyOpen(deviceNum, interfaceNum, &handle);
    qDebug()<<QString("Cy设备判断结果：%1").arg(rStatus);
    CyClose(handle);
    return rStatus;
}

qint32 CyUSBSerialController::findDevice()
{
    CY_DEVICE_INFO deviceInfo;
    quint8 numberInterfaces, interfaceNum;
    uchar deviceName[256];

    rStatus = CyLibraryInit();
    qDebug()<<QString("CyLibraryInit状态：%1").arg(rStatus);
    if(rStatus != CY_SUCCESS){
        errorString = QString("CY:Error in Doing library init: Error NO:<%1>").arg(rStatus);
        qDebug()<<errorString;
        return -1;
    }

    rStatus = CyGetListofDevices(&numDevices);
    qDebug()<<QString("usb设备列表中的设备数：%1").arg(numDevices);
    if(rStatus != CY_SUCCESS){
        return -1;
    }
    for (quint8 num = 0; num < numDevices; ++num) {
        interfaceNum = 0;
        rStatus = CyGetDeviceInfo(num, &deviceInfo);
        qDebug()<<QString("CyGetDeviceInfo返回值：%1").arg(rStatus);
        qDebug()<<QString("Cy设备判断结果：%1").arg(deviceInfo.productName[num]);
        if(rStatus == CY_SUCCESS){
            if(isCypressDevice(num) != CY_SUCCESS){
                continue;
            }
            numberInterfaces = deviceInfo.numInterfaces;
            while(numberInterfaces--){
//                if(deviceInfo.deviceClass[interfaceNum] == CY_CLASS_VENDOR){
//                    CyLibraryExit();
//                    return num;
//                }
                if(deviceInfo.deviceClass[numberInterfaces] == CY_CLASS_VENDOR){
                    CyLibraryExit();
                    return num;
                }
            }
        }
    }
    CyLibraryExit();
    return -1;
}

CY_RETURN_STATUS CyUSBSerialController::GpioRead(const quint8 &deviceNum, const quint8 &interfaceNum,const quint8 &gpioNum, quint8 *value)
{
    rStatus = CyOpen(deviceNum, interfaceNum, &cyHandle);
    if(rStatus != CY_SUCCESS){
        CyLibraryExit();
        return rStatus;
    }
    rStatus = CyGetGpioValue(cyHandle,gpioNum,value);
    return rStatus;
}

CY_RETURN_STATUS CyUSBSerialController::GpioWrite(const quint8 &deviceNum, const quint8 &interfaceNum, const quint8 &gpioNum, quint8 value)
{
    rStatus = CyOpen(deviceNum, interfaceNum, &cyHandle);
    if(rStatus != CY_SUCCESS){
        CyLibraryExit();
        return rStatus;
    }
    value = value > 0 ? 1 : 0;
    rStatus = CySetGpioValue(cyHandle, gpioNum, value);
    if(rStatus != CY_SUCCESS){
        CyLibraryExit();
    }

    CyClose(cyHandle);
    if(value == 0) CyLibraryExit();
    return rStatus;
}

CY_RETURN_STATUS CyUSBSerialController::I2CWrite(const quint8 &deviceNum, const quint8 &interfaceNum, const quint8 &address, const quint32 &len, quint8 *buf)
{
    CY_I2C_DATA_CONFIG cyI2CDataConfig;
    CY_DATA_BUFFER cyDataBuffer;

    rStatus = CyOpen(deviceNum, interfaceNum, &cyHandle);
    if(rStatus != CY_SUCCESS){
        CyClose(cyHandle);
        return rStatus;
    }

    cyI2CDataConfig.slaveAddress = address;
    cyI2CDataConfig.isStopBit = true;
    cyDataBuffer.buffer = buf;
    cyDataBuffer.length = len;

    rStatus = CyI2cWrite(cyHandle, &cyI2CDataConfig, &cyDataBuffer, 5000);

    CyClose(cyHandle);
    return rStatus;
}

CY_RETURN_STATUS CyUSBSerialController::I2CRead(const quint8 &deviceNum, const quint8 &interfaceNum, const quint8 &address, const quint32 &len, quint8 *buf)
{
    CY_I2C_DATA_CONFIG cyI2CDataConfig;
    CY_DATA_BUFFER cyDataBuffer;

    rStatus = CyOpen(deviceNum, interfaceNum, &cyHandle);
    if(rStatus != CY_SUCCESS){
        CyClose(cyHandle);
        return rStatus;
    }

    cyI2CDataConfig.slaveAddress = address;
    cyI2CDataConfig.isStopBit = true;
    cyI2CDataConfig.isNakBit = true;
    cyDataBuffer.buffer = buf;
    cyDataBuffer.length = len;

    rStatus = CyI2cRead(cyHandle, &cyI2CDataConfig, &cyDataBuffer, 5000);

    CyClose(cyHandle);
    return rStatus;
}

CY_RETURN_STATUS CyUSBSerialController::cySPIWaitForIdle(const CY_HANDLE &handle)
{
    quint8 rData[2], wData[2];
    qint32 timeout = 0xFFFF;
    CY_RETURN_STATUS rStatus;
    CY_DATA_BUFFER writeBuf, readBuf;

    writeBuf.length = 2;
    writeBuf.buffer = wData;

    readBuf.length = 2;
    readBuf.buffer = rData;

    do{
        wData[0] = READ_STATUS1;
        rStatus = CySpiReadWrite(handle, &readBuf, &writeBuf, 5000);
        if(rStatus != CY_SUCCESS){
            break;
        }
        timeout --;
        if(timeout == 0){
            rStatus = CY_ERROR_IO_TIMEOUT;
            return rStatus;
        }
    }while(rData[1] & 3);

    return rStatus;
}

CY_RETURN_STATUS CyUSBSerialController::setLedOnOff(const quint8 &deviceNum, quint16 &ledRed, quint16 &ledGreen, quint16 &ledBlue)
{
    quint8 *sendBuf;
    const int writeSize = 2;
    quint8 enableByte = 0;
    if(ledRed > 0) enableByte |= 0x01;
    if(ledGreen > 0) enableByte |= 0x07;
    if(ledBlue > 0) enableByte |= 0x04;
    sendBuf = new quint8[writeSize];

    sendBuf[0] = WRITE_LED_ENABLE;
    sendBuf[1] = enableByte;
    rStatus = I2CWrite(deviceNum, I2C_IF_NUM, cSlaveAddress7bit, writeSize, sendBuf);
    if(rStatus != CY_SUCCESS){

    }
    delete [] sendBuf;
    return rStatus;
}

CY_RETURN_STATUS CyUSBSerialController::getLedOnOff(const quint8 &deviceNum, quint16 &ledRed, quint16 &ledGreen, quint16 &ledBlue)
{
    const int writeSize = 1, readSize = 1;

    quint8 *sendBuf = new quint8[writeSize];
    quint8 *readBuf = new quint8[readSize];

    sendBuf[0] = READ_LED_ENABLE;
    rStatus = I2CWrite(deviceNum, I2C_IF_NUM, cSlaveAddress7bit, writeSize, sendBuf);
    if(rStatus != CY_SUCCESS){
        return rStatus;
    }

    rStatus = I2CRead(deviceNum, I2C_IF_NUM, cSlaveAddress7bit, readSize, readBuf);
    if(rStatus != CY_SUCCESS){
        return rStatus;
    }
    ledRed = readBuf[0] & 0x01;
    ledGreen = (readBuf[0] >> 1) & 0x01;
    ledBlue = (readBuf[0] >> 2) & 0x01;
    delete []sendBuf;
    delete []readBuf;
    return rStatus;
}

CY_RETURN_STATUS CyUSBSerialController::getLedCurrent(const quint8 &deviceNum, qint32 &ledRed, qint32 &ledGreen, qint32 &ledBlue)
{
    const int writeSize = 1;
    const int readSize = 6;
    quint8 *sendBuf = new quint8[writeSize];
    quint8 *readBuf = new quint8[readSize];

    sendBuf[0] = READ_LED_CURRENT;
    rStatus = I2CWrite(deviceNum, I2C_IF_NUM, cSlaveAddress7bit, writeSize, sendBuf);
    if(rStatus != CY_SUCCESS){
        delete [] sendBuf;
        delete [] readBuf;
        return rStatus;
    }

    rStatus = I2CRead(deviceNum, I2C_IF_NUM, cSlaveAddress7bit, readSize, readBuf);
    if(rStatus == CY_SUCCESS){
        ledRed = (readBuf[1] << 8) | readBuf[0];
        ledGreen = (readBuf[3] << 8) | readBuf[2];
        ledBlue = (readBuf[5] << 8) | readBuf[4];
    }

    delete [] sendBuf;
    delete [] readBuf;
    return rStatus;
}

CY_RETURN_STATUS CyUSBSerialController::setLedCurrent(const quint8 &deviceNum, quint16 &ledRed, quint16 &ledGreen, quint16 &ledBlue)
{
    const int writeSize = 7;
    quint8 *sendBuf = new quint8[writeSize];

    sendBuf[0] = WRITE_LED_CURRENT;
    sendBuf[1] = ledRed & 0xff;
    sendBuf[2] = (ledRed >> 8) & 0xff;
    sendBuf[3] = ledGreen & 0xff;
    sendBuf[4] = (ledGreen >> 8) & 0xff;
    sendBuf[5] = ledBlue & 0xff;
    sendBuf[6] = (ledBlue >> 8) & 0xff;

    rStatus = I2CWrite(deviceNum, I2C_IF_NUM, cSlaveAddress7bit, writeSize, sendBuf);
    if(rStatus != CY_SUCCESS){
        errorString = QString("Set led current error: Error No:<%1>").arg(rStatus);
    }

    delete [] sendBuf;
    return rStatus;
}

CY_RETURN_STATUS CyUSBSerialController::flashBlockRead(const quint8 &deviceNum, quint32 address, quint32 len, char *buf)
{
    const quint32 blockSize = 0x1000;   //一个块4kb
    quint8 wBuffer[0x1004], rBuffer[0x1004];    //数组的长度是Blocksize+4

    CY_DATA_BUFFER cyDatabufferWrite, cyDatabufferRead;
    quint32 writeSize, readSize;
    quint32 strAddr = address;

    rStatus = CyOpen(deviceNum, SPI_IF_NUM, &cyHandle);
    if(rStatus != CY_SUCCESS){
        return rStatus;
    }

    //读取缓冲区
    while (len > 0) {
        wBuffer[0] = READ_DATA;
        wBuffer[1] = (address >> 16) & 0xff;
        wBuffer[2] = (address >> 8) & 0xff;
        wBuffer[3] = address & 0xff;

        writeSize = 4;
        readSize = len > blockSize ? blockSize : len;

        cyDatabufferWrite.buffer = wBuffer;
        cyDatabufferWrite.length = 4 + readSize;    //4字节的指令+256字节的块大小

        cyDatabufferRead.buffer = rBuffer;
        cyDatabufferRead.length = 4 + readSize;

        rStatus = CySpiReadWrite(cyHandle, &cyDatabufferRead, &cyDatabufferWrite, 5000);    //进行spi读写
        if(rStatus != CY_SUCCESS){
            CyClose(cyHandle);
            return rStatus;
        }

        for (uint i=0; i < readSize; ++i) {
            buf[address - strAddr + i] = rBuffer[writeSize + i];
        }

        rStatus = cySPIWaitForIdle(cyHandle);   //等待闪存激活
        if(rStatus != CY_SUCCESS){
            CyClose(cyHandle);
            return rStatus;
        }

        address += blockSize;
        if(len > blockSize) len -= blockSize;
        else len = 0;
    }

    CyClose(cyHandle);
    return rStatus;
}

CY_RETURN_STATUS CyUSBSerialController::setProjectorOnOff(const quint8 &deviceNum, const quint8 &enable)
{
    return GpioWrite(deviceNum, GPIO_IF_NUM, GPIO_IF_NUM, enable);
}

CY_RETURN_STATUS CyUSBSerialController::resetCypressDevice(const quint8 &deviceNum)
{
    quint8 sig[6];
    rStatus = CyOpen(deviceNum, I2C_IF_NUM, &cyHandle);
    if(rStatus == CY_SUCCESS){
        rStatus = CyGetSignature(cyHandle, sig);
        if(rStatus == CY_SUCCESS){
            CyResetDevice(cyHandle);
        }
    }
    CyClose(cyHandle);
    return rStatus;
}

CY_RETURN_STATUS CyUSBSerialController::cyAPIInit(const quint8 &deviceNum,int lightSensorType)
{
    if(lightSensorType == 1)//大功率
    {
        aSlaveAddress7bit = 0x29;
        readLightValueCommand = 0xB4;
    }
    else if (lightSensorType == 0)//普通
    {
        aSlaveAddress7bit = 0x39;
        readLightValueCommand = 0xAC;
    }
    else if(lightSensorType == 2)//小型2k光机
    {
        aSlaveAddress7bit = 0x1A;
        readLightValueCommand = 0xB4;
    }

    quint16 ledR = 0, ledG = 1, ledB = 0;

    rStatus = CyLibraryInit();
    if(rStatus != CY_SUCCESS){
        return rStatus;
    }

    qDebug("开始初始化光机");
    setProjectorOnOff(deviceNum, 1);
    QThread::sleep(1);
    qDebug("尝试打开光机");
    rStatus = getLedOnOff(deviceNum, ledR, ledG, ledB);
    qDebug()<<QString("getLedOnOff的返回值:%1").arg(rStatus);
    if(rStatus != CY_SUCCESS){
        //if i2c communication fail. then do hot plug procedures
        rStatus = resetCypressDevice(deviceNum);
        if(rStatus != CY_SUCCESS){
            CyLibraryExit();
            setProjectorOnOff(deviceNum, 0);
            return rStatus;
        }

        CyLibraryExit();
        QThread::sleep(1);

        rStatus = CyLibraryInit();
        if(rStatus != CY_SUCCESS){
            return rStatus;
        }

        rStatus = getLedOnOff(deviceNum, ledR, ledG, ledB);
        if(rStatus != CY_SUCCESS){
            CyLibraryExit();
            setProjectorOnOff(deviceNum, 0);
            return rStatus;
        }
    }

    return CY_SUCCESS;
}

CY_RETURN_STATUS CyUSBSerialController::setProjectorFlip(const quint8 &deviceNum, const quint8 &projectFlip)
{
    const quint8 writeSize = 2;
    quint8 *sendBuf = new quint8[writeSize];

    sendBuf[0] = WRITE_PROJCETOR_FLIP;
    sendBuf[1] = projectFlip;

    rStatus = I2CWrite(deviceNum, I2C_IF_NUM, cSlaveAddress7bit, writeSize, sendBuf);
    delete [] sendBuf;
    return rStatus;
}

CY_RETURN_STATUS CyUSBSerialController::setTestPattern(const quint8 &deviceNum,  const quint8 &PatternSel)
{
    const qint8 writeSize = 7;
    quint8 *sendBuf = new quint8[writeSize];

    sendBuf[0] = WRITE_TEST_PATTERNS;
    sendBuf[1] = PatternSel;
    if((PatternSel & 0x7F) == 0x01){
        sendBuf[2] = 0x70;
        sendBuf[3] = 0x00;
        sendBuf[4] = 0xff;
        sendBuf[5] = 0x00;
        sendBuf[6] = 0x00;
    }else if ((PatternSel & 0x7F) == 0x07) {
        sendBuf[2] = 0x70;
        sendBuf[3] = 0x04;
        sendBuf[4] = 0x00;
        sendBuf[5] = 0x04;
        sendBuf[6] = 0x00;
    }else {
        sendBuf[2] = 0x00;
        sendBuf[3] = 0x00;
        sendBuf[4] = 0x00;
        sendBuf[5] = 0x00;
        sendBuf[6] = 0x00;
    }
    rStatus = I2CWrite(deviceNum, I2C_IF_NUM, cSlaveAddress7bit, writeSize, sendBuf);
    if(rStatus != CY_SUCCESS){

    }
    delete [] sendBuf;
    return rStatus;
}

CY_RETURN_STATUS CyUSBSerialController::getLightSensor(const quint8 &deviceNum, qint32 &lightValue)
{
    const qint32 writeSize = 2;
    const qint32 readSize = 2;
    quint8 *sendBuf = new quint8[writeSize];
    quint8 *readBuf = new quint8[readSize];

    sendBuf[0] = 0xA0;
    sendBuf[1] = 0x03;
    rStatus = I2CWrite(deviceNum, I2C_IF_NUM, aSlaveAddress7bit, writeSize, sendBuf);
    if(rStatus == CY_SUCCESS){
        QThread::sleep(1);
        sendBuf[0] = 0xAC;
        rStatus = I2CWrite(deviceNum, I2C_IF_NUM, aSlaveAddress7bit, 1, sendBuf);
        if(rStatus == CY_SUCCESS){
            rStatus = I2CRead(deviceNum, I2C_IF_NUM, aSlaveAddress7bit, readSize, readBuf);
            if(rStatus == CY_SUCCESS){
                lightValue = (readBuf[1] << 8) | readBuf[0];
                sendBuf[0] = 0xA0;
                sendBuf[1] = 0x00;
                rStatus = I2CWrite(deviceNum, I2C_IF_NUM, aSlaveAddress7bit, writeSize, sendBuf);
            }
        }
    }
    delete [] sendBuf;
    delete [] readBuf;
    return rStatus;
}


CY_RETURN_STATUS CyUSBSerialController::getLedTemperature(const quint8 &deviceNum, qint32 &tempValue)
{
    quint8 sendBuf[1] = {READ_TEMPERATRUE};
    quint8 readBuf[2];
    qint32 value = 0;

    qint32 val_r = 0, val_g = 0, val_b = 0;
    rStatus = getLedCurrent(deviceNum, val_r, val_g, val_b);
    if(rStatus != CY_SUCCESS){
        return rStatus;
    }

    rStatus = I2CWrite(deviceNum, I2C_IF_NUM, cSlaveAddress7bit,  1, sendBuf);
    if(rStatus != CY_SUCCESS){
        return rStatus;
    }

    rStatus = I2CRead(deviceNum, I2C_IF_NUM, cSlaveAddress7bit, 2, readBuf);
    if(rStatus != CY_SUCCESS){
        return rStatus;
    }

    tempValue = (readBuf[1] << 8) | readBuf[0];

    quint16 r = 0, g = 0, b = 0;
    rStatus = getLedOnOff(deviceNum, r, g, b);
    if(rStatus != CY_SUCCESS){
        return rStatus;
    }

    value += static_cast<qint32>((g ? (0.88*4.0*5.8593*val_g):0.0)/100.0);
    tempValue += value;
    return rStatus;
}
