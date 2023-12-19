#ifndef DATATABLEWID_H
#define DATATABLEWID_H

#include "comtablewid.h"
#include "baseobject.h"
#include "datapacket.h"

class Td_DataTabWid : public ComTableWid
{
    Q_OBJECT
public:
    explicit Td_DataTabWid(QWidget *parent = nullptr);

protected:
    void initWid();
    void appendItem(sObjectData *unit);

signals:

protected slots:
    void timeoutDone();

private:
    sCfgItem *mItem;
    sObjData *mData;
    sDevType *mSceen;
    sBusData *mBusData;
};

#endif // DATATABLEWID_H
