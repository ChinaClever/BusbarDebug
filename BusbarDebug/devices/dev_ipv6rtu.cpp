#include "dev_ipv6rtu.h"

#define IP_LINE_NUM 3
#define IPV6_RTU_ONE_LEN 49  // 标准数据长度
#define IPV6_HUAWEI_RTU_ONE_LEN 42  // 华为数据长度

Dev_Ipv6Rtu::Dev_Ipv6Rtu(QObject *parent) : Dev_Object(parent)
{

}

Dev_Ipv6Rtu *Dev_Ipv6Rtu::bulid(QObject *parent)
{
    static Dev_Ipv6Rtu* sington = nullptr;
    if(sington == nullptr)
        sington = new Dev_Ipv6Rtu(parent);
    return sington;
}

void Dev_Ipv6Rtu::initRtuItem(sRtuItem &it)
{
    it.addr = mItem->addr;
    it.fn = 0x03;
    it.reg = 0;
    it.num = IPV6_RTU_ONE_LEN;// V6标准

    if(mCfg->ip_version == 7) {
        it.num = IPV6_HUAWEI_RTU_ONE_LEN;  // V6华为
    }
}

/**
  * 功　能：读取电参数 数据
  * 入口参数：ptr -> 缓冲区
  * 出口参数：pkt -> 结构体
  * 返回值：12 正确
  */
int Dev_Ipv6Rtu::recvDataV6(uchar *ptr)
{
    uchar *ret = ptr;
    int line = IP_LINE_NUM;

    sObjData *obj = &(mDev->data);
    ptr =  toShort(ptr, line, obj->vol.value);
    ptr =  toShort(ptr, line, obj->cur.value);
    ptr =  toShort(ptr, line, obj->pow);
    ptr =  toShort(ptr, line, obj->hz);
    ptr =  toInt(ptr, line, obj->ele);

    obj->tem.size = obj->hum.size = 1;
    ptr =  toShort(ptr, 1, obj->tem.value);
    ptr =  toShort(ptr, 1, obj->hum.value);
    obj->tem.max[0] = 40; obj->hum.max[0] = 99;

    ptr = toThreshold(ptr, line, obj->vol);
    ptr = toThreshold(ptr, line, obj->cur);
    ptr += 8 + (2*2*line + 4); // 报警标志位

    obj->size = getShort(ptr); ptr +=2;
    obj->version = getShort(ptr); ptr +=2;
    obj->br = getShort(ptr); ptr +=2;
    obj->reserve = getShort(ptr); ptr +=2;

    mDt->lines = obj->size;
    if(obj->size == 2)  obj->size = 3;
    obj->vol.size = obj->cur.size = obj->size;

    return ptr-ret;
}


/**
  * 功　能：读取电参数 数据
  * 入口参数：ptr -> 缓冲区
  * 出口参数：pkt -> 结构体
  * 返回值：12 正确
  */
int Dev_Ipv6Rtu::recvDataV6Huawei(uchar *ptr)
{
    uchar *ret = ptr;
    int line = IP_LINE_NUM;

    sObjData *obj = &(mDev->data);
    ptr =  toShort(ptr, line, obj->vol.value);
    ptr =  toShort(ptr, line, obj->cur.value);
    ptr =  toShort(ptr, line, obj->pow);
    ptr =  toShort(ptr, line, obj->hz);
    ptr =  toInt(ptr, line, obj->ele);

    ptr = toThreshold(ptr, line, obj->vol);
    ptr = toThreshold(ptr, line, obj->cur);

    obj->tem.max[0] = 40; obj->hum.max[0] = 99;


    ptr +=  (2*line + 2); // 报警

    mDt->lines = getShort(ptr); ptr +=2;
    obj->version = getShort(ptr); ptr +=2;
    obj->vol.size = obj->cur.size = mDt->lines;

    ptr +=  (2 + 2); // 波特率+预留

    return ptr-ret;
}

bool Dev_Ipv6Rtu::recvPacket(uchar *buf, int len)
{
    if(mCfg->ip_version == 6) recvDataV6(buf);
    if(mCfg->ip_version == 7) recvDataV6Huawei(buf);

    return len!=0;
}

bool Dev_Ipv6Rtu::readPduData()
{
    sRtuItem it;
    uchar recv[MODBUS_RTU_SIZE] = {0};

    initRtuItem(it);
    int len = mModbus->read(it, recv);
    return recvPacket(recv, len);
}
