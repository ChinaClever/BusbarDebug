#ifndef DEV_SICTRL_H
#define DEV_SICTRL_H

#include "dev_sirtu.h"

class Dev_SiCtrl : public Dev_Object
{
    Q_OBJECT
    explicit Dev_SiCtrl(QObject *parent = nullptr);
public:
    static Dev_SiCtrl *bulid(QObject *parent = nullptr);

    bool setDev();
    bool unClock();
    bool clearEle();
    bool factorySet();

    bool setCurTh(int i);
    bool setVolTh(int i);

    bool setTem();
    bool setHum();
    bool sentRtuCmd(ushort reg, ushort value, uchar fn=0x10);
    bool setHorizontaOrVertical();

    bool setBusbarStartEle(int index);//index [1,3]
    bool setBusbarStartPow(int val1 , int val2);
    bool setBusbarStartLineVol(int index , int val1 , int val2);

    bool setBusbarInsertEle(int index);//index [1,9]
    bool setBusbarInsertPow(int index , int val1 , int val2);
    bool setBusbarInsertVol(int index , int val1 , int val2);
    bool setBusbarInsertTem(int index , int val1 , int val2);
    bool setBusbarInsertCur(int index , int val1 , int val2);
    bool setBusbarInsertBaud(int val);
    bool setBusbarInsertiOF(int val);
    bool setBusbarInsertBuzzer(int val);
    bool setBusbarInsertFilter(int val);


    bool rtu_sent_ushortV3_buff(uchar addr, ushort reg, uint num,  uint val1, uint val2);
    bool rtu_sent_uintV3_buff(uchar addr, ushort reg, uint num,  uint val1, uint val2);

protected:
    bool writeReg(ushort reg, int i, sDataUnit &it, sUnitCfg &unit, int r=1);
};

#endif // DEV_SICTRL_H
