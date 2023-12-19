#ifndef HOME_LINETABWID_H
#define HOME_LINETABWID_H

#include "td_datatabwid.h"

class Td_LineTabWid : public ComTableWid
{
    Q_OBJECT
public:
    explicit Td_LineTabWid(QWidget *parent = nullptr);

signals:

protected:
    void initWid();
    void appendItem(sObjectData *dev);

protected slots:
    void timeoutDone();

private:
    sCfgItem *mItem;
    sObjData *mData;
    sDevType *mSceen;
    sBusData *mBusData;
};

#endif // HOME_LINETABWID_H
