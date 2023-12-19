#ifndef AD_LEDSI_H
#define AD_LEDSI_H

#include "yc_obj.h"
#include "dev_ipcfg.h"

class Ad_LedSi : public BaseThread
{
    Q_OBJECT
    explicit Ad_LedSi(QObject *parent = nullptr);
public:
    static Ad_LedSi *bulid(QObject *parent = nullptr);
    bool startAdjust();

protected:
    bool sentCmd();
    bool writeCmd(uchar fn, uchar line);
    bool transmit(uchar *buf, int len);

    bool writeDc();
    bool writeAc();
    bool readData();
    bool waitDcRecv();
    int readSerial(uchar *recv, int sec=25);
    bool recvStatus(uchar *recv, int len);
    bool updateStatus(ushort status);
    bool overWork(const QString &str);
    bool writeCmd1(uchar fn, uchar line);

private:
    Dev_SiRtu *mRtu;
};

#endif // AD_LEDSI_H
