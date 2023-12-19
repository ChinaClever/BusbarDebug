/*
 *
 *  Created on: 2021年1月1日
 *      Author: Lzy
 */
#include "dev_object.h"

Dev_Object::Dev_Object(QObject *parent) : BaseThread(parent)
{
}


void Dev_Object::initUnit(const QString& prefix, sUnitCfg &unit, int f)
{
    unit.min = read(prefix+"_min", 0).toFloat();
    unit.max = read(prefix+"_max", 10).toFloat();

    switch (f) {
    case 2:
        unit.en = read(prefix+"_en", 0).toInt();
        unit.id = read(prefix+"_id", 0).toInt();
    case 1:
        unit.crMin = read(prefix+"_crmin", 0).toFloat();
        unit.crMax = read(prefix+"_crmax", 10).toFloat();
        break;
    }
}


void Dev_Object::writeUnit(const QString& prefix, sUnitCfg &unit, int f)
{
    write(prefix+"_min", QString::number(unit.min));
    write(prefix+"_max", QString::number(unit.max));

    switch (f) {
    case 2:
        write(prefix+"_en", unit.en);
        write(prefix+"_id", unit.id);
    case 1:
        write(prefix+"_crmin", QString::number(unit.crMin));
        write(prefix+"_crmax", QString::number(unit.crMax));
        break;
    }
}


void Dev_Object::write(const QString &key, const QVariant& v)
{
    Cfg::bulid()->write(key, v, cfgGroup());
}

QVariant Dev_Object::read(const QString &key, const QVariant &v)
{
    return Cfg::bulid()->read(key, v, cfgGroup());
}


uchar *Dev_Object::toInt(uchar *ptr, int line, uint *value)
{
    for(int i=0; i<line; ++i) {
        value[i] =  (*ptr) * 256 + *(ptr+1);  ptr += 2; // 读取电能高8位
        value[i] <<= 16; // 左移8位
        value[i] +=  (*ptr) * 256 + *(ptr+1);  ptr += 2; // 读取电能底8位
    }

    return ptr;
}

uchar *Dev_Object::toOutputEle(uchar *ptr, int line, uint *value)
{
    for(int i=0; i<line; ++i) {
        value[i] = (*ptr++) << 16;
        value[i] += (*ptr++) << 8;
        value[i] += *ptr++;
    }

    return ptr;
}

uchar *Dev_Object::toShort(uchar *ptr, int line, ushort *value)
{
    for(int i=0; i<line; ++i) {
        value[i] =  (*ptr) * 256 + *(ptr+1);  ptr += 2;
    }

    return ptr;
}

uchar *Dev_Object::toChar(uchar *ptr, int line, uchar *value)
{
    for(int i=0; i<line; ++i) {
        value[i] = *(ptr++); // 读取电压
    }

    return ptr;
}

uchar *Dev_Object::toThreshold(uchar *ptr, int line, sDataUnit &unit)
{
    for(int i=0; i<line; ++i) {
        ptr = toShort(ptr, 1, &unit.min[i]);
        ptr = toShort(ptr, 1, &unit.max[i]);
    }

    return ptr;
}

uchar *Dev_Object::toEnvTh(uchar *ptr, sDataUnit &unit)
{
    unit.max[0] = *ptr++;
    unit.min[0] = *ptr++;
    return ptr;
}

ushort Dev_Object::getShort(uchar *ptr)
{
    return  (*ptr) * 256 + *(ptr+1);
}

uchar *Dev_Object::getShort(uchar *ptr, int line, uchar *value)
{
    for(int i=0; i<line; ++i) {
        value[i] = getShort(ptr); ptr += 2;
    }

    return  ptr;
}


ushort Dev_Object::calccrc(ushort crc, uchar crcbuf)
{
    uchar x, kkk=0;
    crc = crc^crcbuf;
    for(x=0;x<8;x++)
    {
        kkk = crc&1;
        crc >>= 1;
        crc &= 0x7FFF;
        if(kkk == 1)
            crc = crc^0xa001;
        crc=crc&0xffff;
    }
    return crc;
}

/**
  * 功　能：CRC校验
  * 入口参数：buf -> 缓冲区  len -> 长度
  * 返回值：CRC
  */
ushort Dev_Object::rtu_crc(uchar *buf, int len)
{
    ushort crc = 0xffff;
    for(int i=0; i<len; i++)
        crc = calccrc(crc, buf[i]);
    return crc;
}

/**
  * 功　能：发送数据打包
  * 入口参数：pkt -> 发送结构体
  * 出口参数：ptr -> 缓冲区
  * 返回值：打包后的长度
  */
int Dev_Object::rtu_sent_buff(uchar addr, uchar *buf , uint len)
{
    static Rtu_Sent msg;
    static QMutex mutex; // 互拆锁
    QMutexLocker locker(&mutex);

    msg.addr = addr;
    msg.len  = len;
    return rtu_sent_packet(&msg, buf);
}


/**
  * 功　能：发送数据打包
  * 入口参数：pkt -> 发送结构体
  * 出口参数：ptr -> 缓冲区
  * 返回值：打包后的长度
  */
int Dev_Object::rtu_sent_packet(Rtu_Sent *pkt, uchar *ptr)
{
    uchar *buf = ptr;
    *(ptr++) = pkt->addr;  /*地址码*/
    *(ptr++) = pkt->fn; /*功能码*/

    /*填入寄存器首地址*/
    *(ptr++) = ((pkt->reg) >> 8); /*高8位*/
    *(ptr++) = (0xff)&(pkt->reg); /*低8位*/

    if( pkt->fn == 0x10 ){
        *(ptr++) = ((pkt->len) >> 24);
        *(ptr++) = ((pkt->len) >> 16);
    }

    /*填入数据长度*/
    *(ptr++) = ((pkt->len) >> 8); /*长度高8位*/
    *(ptr++) = (0xff)&(pkt->len); /*低8位*/

     /*填入CRC*/
    if( pkt->fn == 0x10 ){
        pkt->crc = rtu_crc(buf, 8);
    }else{
        pkt->crc = rtu_crc(buf, 6);
    }
    *(ptr++) = (0xff)&(pkt->crc); /*低8位*/
    *(ptr++) = ((pkt->crc) >> 8); /*高8位*/

    return 8;
}

