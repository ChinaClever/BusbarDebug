#ifndef DEV_IPSNMP_H
#define DEV_IPSNMP_H

#include "dev_sictrl.h"
#include "snmpclient.h"
#include <libsnmp.h>
#include "snmp/snmp_pp/snmp_pp.h"

using namespace Snmp_pp;


class Dev_IpSnmp : public Dev_Object
{
    Q_OBJECT
    explicit Dev_IpSnmp(QObject *parent = nullptr);
public:
    static Dev_IpSnmp *bulid(QObject *parent = nullptr);
    bool readPduData();
    void resetSnmp();
    bool SetInfo(QString o , QString val);
    sBusData *mBusData;
    QString getMACAddress(){return "1.3.6.1.4.1.30966.12.1.1.1.1.0";}
    QString getCurTpyeOid(){return "1.3.6.1.4.1.30966.12.1.1.2.3.0";}
    QString getModeOid(){return "1.3.6.1.4.1.30966.12.1.1.2.5.0";}
    QString getNumOid(){return "1.3.6.1.4.1.30966.12.1.1.2.6.0";}
    QString getButterOid(){return "1.3.6.1.4.1.30966.12.1.1.2.9.0";}
    QString getFilterOid(){return "1.3.6.1.4.1.30966.12.1.1.2.10.0";}
    QString getLightningOid(){return "1.3.6.1.4.1.30966.12.1.1.2.11.0";}
    QString getiOFOid(){return "1.3.6.1.4.1.30966.12.1.1.2.12.0";}
    QString getISDOid(){return "1.3.6.1.4.1.30966.12.1.1.2.13.0";}
    QString getShuntOid(){return "1.3.6.1.4.1.30966.12.1.1.2.14.0";}
    QString getResidualOid(){return "1.3.6.1.4.1.30966.12.1.1.2.15.0";}

    QString getLineVolMinOid(int line , int index){return QString("1.3.6.1.4.1.30966.12.1.2.%1.%2.0").arg(line+1).arg(index+16);}
    QString getOtherMinOid(int index){return QString("1.3.6.1.4.1.30966.12.1.1.3.%1.0").arg(index+13);}
    QString getTemMinOid(int index){return QString("1.3.6.1.4.1.30966.12.1.3.%1.0").arg(index+9);}

protected:
    bool lineNumV3();
    bool devDataV3();
    bool devDataV1();
    int getItemByOid(const QString &oid, int id);

    void praseMasterVal(QString id , QString value);
    void startBoxBaseInfo(QString id, QString val);
    void startBoxEleParaInfo(QString id, QString val);
    void startBoxNetInfo(QString id, QString val);
    void baseMsInformation(QString id, QString val);
    void parseVolHar(QString val, int line);
    void parseCurHar(QString val, int line);
    void loopMsInformation(QString id, QString val);
    void temMsInformation(QString id, QString val);

    bool determine_vb(SmiUINT32 val, Vb &vb, QString str);


private:
    SnmpClient *mSnmp;
};

#endif // DEV_IPSNMP_H
