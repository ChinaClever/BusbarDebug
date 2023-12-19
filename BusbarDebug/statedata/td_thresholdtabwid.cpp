/*
 *
 *  Created on: 2021年1月1日
 *      Author: Lzy
 */
#include "td_thresholdtabwid.h"

Td_ThresholdTabWid::Td_ThresholdTabWid(int index , QWidget *parent) : ComTableWid(parent)
{
    m_index = index;
    initWid();
    mItem = Cfg::bulid()->item;
}

void Td_ThresholdTabWid::initWid()
{
    QStringList header;
    QString title = tr("报警阈值");
    header << tr("项目") << tr("当前值") << tr("最小值") << tr("最大值");
    initTableWid(header, 6, title);
    sDevData *dd = sDataPacket::bulid()->getDev();
    mSceen = &(dd->dt);
    mBusData = get_share_mem();
}

void Td_ThresholdTabWid::change(double &rate , QString &suffix , QString &str , int &r)
{
    switch (m_index) {
    case 0:{rate = COM_RATE_CUR;suffix = "A";str = tr("电流");r = 2;
    }break;
    case 1:{rate = COM_RATE_VOL;suffix = "V";str = tr("电压");r = 1;
    }break;
    case 2:{rate = COM_RATE_VOL;suffix = "V";str = tr("线电压");r = 1;
    }break;
    }
}

void Td_ThresholdTabWid::setDataUnit(int size , sDataValue *unit)
{
    double rate = COM_RATE_CUR;
    QString suffix = "A";
    QString str = tr("电流");
    int r = 2;
    change(rate , suffix , str , r);
    for(int i=0; i<size; ++i) {
        QStringList listStr;
        listStr << str;
        listStr << QString::number(unit->value[i]/rate,'f',r)+suffix;
        listStr << QString::number(unit->min[i]/rate,'f',r)+suffix;
        listStr << QString::number(unit->max[i]/rate,'f',r)+suffix;
        setTableRow(i, listStr);

        if((unit->value[i] < unit->min[i]) || (unit->value[i] > unit->max[i])) {
            unit->alarm[i] = 2;
            setAlarmBackgroundColor(i);
        } else {
            unit->alarm[i] = 0;
            setNormalBackgroundColor(i);
        }
    }
}

void Td_ThresholdTabWid::setDataUnit(int size , sDataPowUnit *unit)
{
    double rate = COM_RATE_POW;
    QString suffix = "kW";
    QString str = tr("功率");
    int r = 3;
    change(rate , suffix , str , r);
    for(int i=0; i<size; ++i) {
        QStringList listStr;
        listStr << str;
        listStr << QString::number(unit->value[i]/rate,'f',r)+suffix;
        listStr << QString::number(unit->min[i]/rate,'f',r)+suffix;
        listStr << QString::number(unit->max[i]/rate,'f',r)+suffix;
        setTableRow(i, listStr);

        if((unit->value[i] < unit->min[i]) || (unit->value[i] > unit->max[i])) {
            unit->alarm[i] = 2;
            setAlarmBackgroundColor(i);
        } else {
            unit->alarm[i] = 0;
            setNormalBackgroundColor(i);
        }
    }
}

void Td_ThresholdTabWid::addList(int idx , sRtuUshortUnit *unit ,double rate ,QString suffix ,QString str ,int r )
{
    QStringList listStr;
    listStr << str;
    listStr << QString::number(unit->svalue/rate,'f',r)+suffix;
    listStr << QString::number(unit->smin/rate,'f',r)+suffix;
    listStr << QString::number(unit->smax/rate,'f',r)+suffix;
    setTableRow(idx, listStr);

    if((unit->svalue < unit->smin) || (unit->svalue > unit->smax)) {
        unit->salarm = 2;
        setAlarmBackgroundColor(idx);
    } else {
        unit->salarm = 0;
        setNormalBackgroundColor(idx);
    }
}

void Td_ThresholdTabWid::setDataUnit(sBoxData *unit)
{
    double rate = COM_RATE_VOL;
    QString suffix = "Hz";
    QString str = tr("频率");
    int r = 1;
//    sRtuUshortUnit rate;//新频率
//    sRtuUshortUnit reCur;//剩余电流
//    sRtuUshortUnit zeroLineCur;//零线电流
//    sRtuULLintUnit totalPow; //总有功功率
    addList(0 , &(unit->rate) ,  rate , suffix , str , r );
    rate = COM_RATE_CUR; suffix = "A"; str = tr("剩余电流"); r = 2;
    addList(1 , &(unit->reCur) ,  rate , suffix , str , r );
    rate = COM_RATE_CUR; suffix = "A"; str = tr("零线电流"); r = 2;
    addList(2 , &(unit->zeroLineCur) ,  rate , suffix , str , r );
    rate = COM_RATE_POW; suffix = "kW"; str = tr("总功率"); r = 3;
    QStringList listStr;
    listStr << str;
    listStr << QString::number(unit->totalPow.ivalue/rate,'f',r)+suffix;
    listStr << QString::number(unit->totalPow.imin/rate,'f',r)+suffix;
    listStr << QString::number(unit->totalPow.imax/rate,'f',r)+suffix;
    setTableRow(3, listStr);

    if((unit->totalPow.ivalue < unit->totalPow.imin) || (unit->totalPow.ivalue > unit->totalPow.imax)) {
        unit->totalPow.ialarm = 2;
        setAlarmBackgroundColor(3);
    } else {
        unit->totalPow.ialarm = 0;
        setNormalBackgroundColor(3);
    }

}

void Td_ThresholdTabWid::appendItem(sObjectData *dev)
{
    switch (m_index) {
    case 0:setDataUnit(dev->lineNum , &(dev->cur));break;
    case 1:setDataUnit(dev->lineNum , &(dev->vol));break;
    case 2:setDataUnit(dev->lineNum , &(dev->lineVol));break;
    case 3:setDataUnit(dev->lineNum , &(dev->pow));break;
    default:break;
    }
}

void Td_ThresholdTabWid::appendItem(sBoxData *dev)
{
    setDataUnit(dev);
}

void Td_ThresholdTabWid::timeoutDone()
{
    clearTable();
    if(mBusData->box[mItem->addr-1].offLine && mBusData->box[mItem->addr-1].loopNum) {
        if(m_index <= 3)
            appendItem(&(mBusData->box[mItem->addr-1].data));
        else
            appendItem(&(mBusData->box[mItem->addr-1]));
    }
}
