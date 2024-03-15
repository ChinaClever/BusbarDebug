/*
 *
 *  Created on: 2021年1月1日
 *      Author: Lzy
 */
#include "sn_devid.h"

Sn_DevId::Sn_DevId(QObject *parent) : BaseThread(parent)
{
    mTypeDef = Sn_DevType::bulid();
}

Sn_DevId *Sn_DevId::bulid(QObject *parent)
{
    static Sn_DevId* sington = nullptr;
    if(sington == nullptr)
        sington = new Sn_DevId(parent);
    return sington;
}

void Sn_DevId::initReadType(sRtuItem &it)
{
    it.addr = mItem->addr;
    it.fn = 0x03;
    it.reg = 0xA001;
    it.num = 2;
}

bool Sn_DevId::analysDevType(uchar *buf, int len)
{
    bool ret = false;
    QString str = tr("读取设备类型成功");
    if(len != 4) {
        str = tr("读取设备类型失败：返回长度为%1").arg(len);
        return updatePro(str, ret);
    }

    uint id = 0;
    for(int i=0; i<len; ++i) {
        id *= 256;  id += buf[i];
    }

    ret = mTypeDef->analysDevType(id);
    if(!ret) str = tr("不支持此设备类型 ID：%1").arg(id, 0, 16);

    return updatePro(str, ret);
}

bool Sn_DevId::readDevId()
{
    sRtuItem it;
    initReadType(it);

    int len = 0;
    static uchar recv[256] = {0};
    for(int i=0; i<5; ++i) {
        len = mModbus->readSn(it, recv);
        if(len==4) break; else if(!delay(3+i)) break;
        if(i>3) mModbus->changeBaudRate();
    }

    return analysDevType(recv, len);
}

bool Sn_DevId::readDevType()
{
    QString str = tr("开始读取设备ID！");
    bool ret = updatePro(str);
    if(ret) ret = readDevId();

    return ret;
}


void Sn_DevId::initReadCalVal(sRtuItem &it , int size)
{
    it.addr = mItem->addr;
    it.fn = 0x03;
    it.reg = 0xA020;
    it.num = 2*size;
}

bool Sn_DevId::readCalibrationValue(int size , QString & valStr)
{
    QString str = tr("开始读取设备校准值！");
    bool ret = updatePro(str);
    if(ret) ret = readCalVal(size , valStr);

    return ret;
}


bool Sn_DevId::readCalVal(int size , QString & valStr)
{
    sRtuItem it;
    initReadCalVal(it , size);

    int len = 0;
    static uchar recv[255] = {0};
    for(int i=0; i<5; ++i) {
        len = mModbus->read(it, recv);
        if(len==size*4) break; else if(!delay(3+i)) break;
    }

    return analysDevCalibrationValue(recv, len , size , valStr);
}

bool Sn_DevId::analysDevCalibrationValue(uchar *buf, int len ,int size , QString &valStr)
{
    bool ret = false;
    QString str;
    if(len != size*4) {
        str = tr("读取设备校准值失败：返回长度为%1").arg(len);
        return updatePro(str);
    }
    str = tr("读取设备校准值成功：返回长度为%1").arg(len);
    uint id = 0;
    ret = true;
    for(int i = 0; i < len; i += 4) {
        id = 0;
        id += buf[i]*256 + buf[i+1];
        id <<= 16;
        id += buf[i+2]*256 + buf[i+3];
        if(i != len - 4) valStr += QString::number(id)+"/";
        else valStr += QString::number(id);
    }

    return updatePro(str, ret);
}
