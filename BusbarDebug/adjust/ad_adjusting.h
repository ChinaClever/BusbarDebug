#ifndef AD_ADJUSTING_H
#define AD_ADJUSTING_H

#include "ad_ledsi.h"
#include "ctrl_zpduthread.h"
class Ad_Adjusting : public BaseThread
{
    Q_OBJECT
    explicit Ad_Adjusting(QObject *parent = nullptr);
public:
    static Ad_Adjusting *bulid(QObject *parent = nullptr);
    bool startAdjust();
//    bool startAdjustOneByOne(int lines );
    bool sentCmdkPhase(int k);
    bool readData();


protected:
    bool sentCmd();

    bool writeCmd(uchar fn, uchar line, int addr = 0);
    bool transmit(uchar *buf, int len);
    bool transmit10(uchar *buf, int len);

    bool writePhase();
    bool writeOffset();


    bool waitDcRecv();
    int readSerial(uchar *recv, int sec=25);
    bool recvStatus(uchar *recv, int len);
    bool updateStatus(ushort status);
    bool overWork(const QString &str);

private:
    Ctrl_ZpduThread *mControlOp;
};

#endif // AD_ADJUSTING_H
