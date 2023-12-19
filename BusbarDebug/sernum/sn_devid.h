#ifndef DEV_TYPEID_H
#define DEV_TYPEID_H
#include "sn_devtype.h"

class Sn_DevId: public BaseThread
{
    Q_OBJECT
    explicit Sn_DevId(QObject *parent = nullptr);
public:
    static Sn_DevId *bulid(QObject *parent = nullptr);

    bool readDevType();
    bool readCalibrationValue(int size, QString &valStr);
protected:
    bool readDevId();
    void initReadType(sRtuItem &it);
    bool readPduData(){return false;}
    bool analysDevType(uchar *buf, int len);

    bool readCalVal(int size, QString &valStr);
    void initReadCalVal(sRtuItem &it , int size);
    bool analysDevCalibrationValue(uchar *buf, int len, int size, QString &valStr);

private:
    Sn_DevType *mTypeDef;
};

#endif // DEV_TYPEID_H
