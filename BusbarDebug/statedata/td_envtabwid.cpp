/*
 *
 *  Created on: 2021年1月1日
 *      Author: Lzy
 */
#include "td_envtabwid.h"

Td_EnvTabWid::Td_EnvTabWid(QWidget *parent) : ComTableWid(parent)
{
    mItem = Cfg::bulid()->item;
    initWid();
}

void Td_EnvTabWid::initWid()
{
    QStringList header;
    QString title = tr("传感器信息");
    header << tr("传感器") << tr("监测值") << tr("最小值") << tr("最大值");
    initTableWid(header, 2, title);

    mBusData = get_share_mem();
}


void Td_EnvTabWid::setDataUnit(sDataValue &unit)
{
    QString suffix = "℃";
    QString str = tr("温度");


    for(int i=0; i<4; ++i) {
        QStringList listStr;

        listStr << str;
        listStr << QString::number(unit.value[i]/COM_RATE_TEM,'f',1)+suffix;
        listStr << QString::number(unit.min[i]/COM_RATE_TEM,'f',1)+suffix;
        listStr << QString::number(unit.max[i]/COM_RATE_TEM,'f',1)+suffix;
        qDebug()<<mItem->addr<<unit.max[i];
        setTableRow(i, listStr);

        if((unit.value[i] < unit.min[i]) || (unit.value[i] > unit.max[i])) {
            unit.alarm[i] = 2;
            setAlarmBackgroundColor(i);
        } else {
            unit.alarm[i] = 0;
            setNormalBackgroundColor(i);
            setItemColor(i, 1, unit.alarm[i]);
        }
    }
}


void Td_EnvTabWid::appendItem()
{
    setDataUnit(mBusData->box[mItem->addr-1].env.tem);
}

void Td_EnvTabWid::timeoutDone()
{
    clearTable();
    if(mBusData->box[mItem->addr-1].offLine && mBusData->box[mItem->addr-1].loopNum) {
        appendItem();
    }
}
