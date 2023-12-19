#ifndef AD_MODBUS_H
#define AD_MODBUS_H

#include "datapacket.h"
#include "dbstates.h"

//#define MODBUS_RTU_SIZE 255

class Ad_Modbus : public QThread
{
    Q_OBJECT
    explicit Ad_Modbus(QObject *parent = nullptr);
public:
    static Ad_Modbus *bulid(QObject *parent = nullptr);

    bool writeSerial(quint8 *cmd, int len);
    uchar getXorNumber(uchar *buf, int len);

protected:
    void initSerial();

private:
    sCfgItem *mItem;
    SerialPort *mSerial;
};

#endif // AD_MODBUS_H
