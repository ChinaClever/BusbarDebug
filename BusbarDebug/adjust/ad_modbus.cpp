/*
 *
 *  Created on: 2020年10月1日
 *      Author: Lzy
 */
#include "ad_modbus.h"

extern QString user_land_name();

Ad_Modbus::Ad_Modbus(QObject *parent) : QThread(parent)
{
    mSerial = nullptr;
    mItem = Cfg::bulid()->item;
}


Ad_Modbus *Ad_Modbus::bulid(QObject *parent)
{
    static Ad_Modbus* sington = nullptr;
    if(sington == nullptr)
        sington = new Ad_Modbus(parent);
    return sington;
}


void Ad_Modbus::initSerial()
{
    if(!mSerial) {
        mSerial = mItem->control;
    }
}


bool Ad_Modbus::writeSerial(quint8 *cmd, int len)
{
    initSerial();
    bool ret = mSerial->isOpened();
    if(ret) {
        mSerial->write(cmd, len);
    } else {
        qDebug() << "AdjustCoreThread writeSerial err !" << ret;
    }

    return ret;
}


/**
  * 功　能：XOR和校验
  * 入口参数：buf -> 缓冲区  len -> 长度
  * 返回值：XOR
  */
uchar Ad_Modbus::getXorNumber(uchar *buf, int len)
{
    uchar xorsum = 0x00;
    for(int i=0; i<len; i++)
        xorsum ^= buf[i];
    return xorsum;
}
