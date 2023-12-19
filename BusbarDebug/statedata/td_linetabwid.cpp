/*
 *
 *  Created on: 2021年1月1日
 *      Author: Lzy
 */
#include "td_linetabwid.h"

Td_LineTabWid::Td_LineTabWid(QWidget *parent) : ComTableWid(parent)
{
    initWid();
    mItem = Cfg::bulid()->item;
}

void Td_LineTabWid::initWid()
{
    QStringList header;
    QString title = tr("相数据列表");
    header << tr("开关") << tr("电流") << tr("电压") << tr("有功功率")  << tr("功率因数") << tr("电能");
    initTableWid(header, 3, title);

    sDevData *dd = sDataPacket::bulid()->getDev();
    mSceen = &(dd->dt);
    mBusData = get_share_mem();
}


void Td_LineTabWid::appendItem(sObjectData *unit)
{
    for(int i=0; i<unit->lineNum; ++i) {
        QStringList listStr;

        if( mSceen->devType == START_BUSBAR){
            if(1 == unit->sw[i]) listStr << tr("开"); else  listStr << tr("关");
        }
        else{
            if(2 == unit->sw[i]) listStr << tr("开"); else if(1 == unit->sw[i])  listStr << tr("关");
        }
        listStr << QString::number(unit->cur.value[i]/COM_RATE_CUR,'f',2)+"A";
        listStr << QString::number(unit->vol.value[i]/COM_RATE_VOL,'f',1)+"V";
        listStr << QString::number(unit->pow.value[i]/COM_RATE_POW,'f',3)+"kW";
        listStr << QString::number(unit->pf[i]/COM_RATE_PF,'f',2);
        listStr << QString::number(unit->ele[i]/COM_RATE_ELE,'f',1)+"kWh";
        setTableRow(i, listStr);

        setItemColor(i, 1, unit->cur.alarm[i]);
        setItemColor(i, 2, unit->vol.alarm[i]);
        setItemColor(i, 3, unit->pow.alarm[i]);
    }
}

void Td_LineTabWid::timeoutDone()
{
    clearTable();
    if(mBusData->box[mItem->addr-1].offLine && mBusData->box[mItem->addr-1].loopNum) {
        appendItem(&(mBusData->box[mItem->addr-1].data));
    }
}
