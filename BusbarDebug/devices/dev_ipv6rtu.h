#ifndef DEV_IPV6RTU_H
#define DEV_IPV6RTU_H

#include "dev_iprtu.h"

class Dev_Ipv6Rtu : public Dev_Object
{
    Q_OBJECT
protected:
    explicit Dev_Ipv6Rtu(QObject *parent = nullptr);
public:
    static Dev_Ipv6Rtu *bulid(QObject *parent = nullptr);
    bool readPduData();

protected:
    void initRtuItem(sRtuItem &it);
    bool recvPacket(uchar *buf, int len);
    int recvDataV6(uchar *ptr);
    int recvDataV6Huawei(uchar *ptr);
};

#endif // DEV_IPV6RTU_H
