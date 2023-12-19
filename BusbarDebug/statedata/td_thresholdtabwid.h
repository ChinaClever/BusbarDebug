#ifndef HOME_THRESHOLDTABWID_H
#define HOME_THRESHOLDTABWID_H

#include "td_linetabwid.h"

class Td_ThresholdTabWid : public ComTableWid
{
    Q_OBJECT
public:
    explicit Td_ThresholdTabWid(int index, QWidget *parent = nullptr);

signals:

protected:
    void initWid();
    void setDataUnit(int size, sDataValue *unit);
    void setDataUnit(int size , sDataPowUnit *unit);
    void setDataUnit(sBoxData *unit);
    void appendItem(sObjectData *dev);
    void appendItem(sBoxData *dev);

protected slots:
    void timeoutDone();

protected:
    void change(double &rate , QString &suffix , QString &str , int &r);
    void addList(int idx , sRtuUshortUnit *unit ,double rate ,QString suffix ,QString str ,int r );

private:
    sCfgItem *mItem;
    sObjData *mData;
    sDevType *mSceen;
    sBusData *mBusData;
    int m_index;
};

#endif // HOME_THRESHOLDTABWID_H
