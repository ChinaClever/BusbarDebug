/*
 *
 *  Created on: 2020年10月1日
 *      Author: Lzy
 */
#include "td_datatabwid.h"

Td_DataTabWid::Td_DataTabWid(QWidget *parent) : ComTableWid(parent)
{
    initWid();
    mItem = Cfg::bulid()->item;
}

void Td_DataTabWid::initWid()
{
    QStringList header;
    QString title = tr("状态列表");
    header<<tr("开关") << tr("电流") << tr("电压") << tr("功率") << tr("电能") << tr("最终电流") << tr("最终功率")<< tr("结果");
    initTableWid(header, 3, title);

    sDevData *dd = sDataPacket::bulid()->getDev();
    mSceen = &(dd->dt);
    mBusData = get_share_mem();
}


void Td_DataTabWid::appendItem(sObjectData *unit)
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
        listStr << QString::number(unit->ele[i]/COM_RATE_ELE,'f',1)+"kWh";
        if(unit->cur.valued[i])
            listStr << QString::number(unit->cur.valued[i]/COM_RATE_CUR,'f',2)+"A";
        else
            listStr << "---";
        if(unit->pow.valued[i])
            listStr << QString::number(unit->pow.valued[i]/COM_RATE_POW,'f',3)+"kW";
        else
            listStr << "---";

        switch(unit->cur.status[i]){
            case Test_Fail: listStr << "X";break;
            case Test_Pass: listStr << "√";break;
            default: listStr << "";break;
        }
        setTableRow(i, listStr);
        if(unit->cur.status[i] == Test_Fail){
            setAlarmBackgroundColor(i);
        }else{
            setNormalBackgroundColor(i);
        }
    }
}

void Td_DataTabWid::timeoutDone()
{
    if(mBusData->box[mItem->addr-1].offLine && mBusData->box[mItem->addr-1].loopNum) {
        appendItem(&(mBusData->box[mItem->addr-1].data));
    } else {
        clearTable();
    }
}
