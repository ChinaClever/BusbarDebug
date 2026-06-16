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
    case 0:{rate = COM_RATE_CUR;suffix = "A";str = tr("电流");r = 3;
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

void Td_ThresholdTabWid::addList(int idx , sRtuULLintUnit *unit ,double rate ,QString suffix ,QString str ,int r )
{
    QStringList listStr;
    listStr << str;
    listStr << QString::number(unit->ivalue/rate,'f',r)+suffix;
    listStr << QString::number(unit->imin/rate,'f',r)+suffix;
    listStr << QString::number(unit->imax/rate,'f',r)+suffix;
    setTableRow(idx, listStr);

    if((unit->ivalue < unit->imin) || (unit->ivalue > unit->imax)) {
        unit->ialarm = 2;
        setAlarmBackgroundColor(idx);
    } else {
        unit->ialarm = 0;
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

    rate = COM_RATE_CUR; suffix = "A"; str = tr("零线电流"); r = 3;
    // addList(2 , &(unit->zeroLineCur) ,  rate , suffix , str , r );
    QStringList listStr1;
    listStr1 << str;
    listStr1 << QString::number(unit->zeroLineCur.svalue/rate,'f',r)+suffix;
    listStr1 << QString::number(unit->zeroLineCur.smin/rate,'f',r)+suffix;
    // listStr1 << "---";
    listStr1 << QString::number(unit->zeroLineCur.smax/rate,'f',r)+suffix;
    setTableRow(2, listStr1);

    if((unit->zeroLineCur.svalue < unit->zeroLineCur.smin) || (unit->zeroLineCur.svalue > unit->zeroLineCur.smax)) {
        unit->zeroLineCur.salarm = 2;
        setAlarmBackgroundColor(2);
    } else {
        unit->zeroLineCur.salarm = 0;
        setNormalBackgroundColor(2);
    }

//    rate = COM_RATE_POW; suffix = "kW"; str = tr("总功率"); r = 3;
//    QStringList listStr;
//    listStr << str;
//    listStr << QString::number(unit->totalPow.ivalue/rate,'f',r)+suffix;
//    listStr << QString::number(unit->totalPow.imin/rate,'f',r)+suffix;
//    listStr << QString::number(unit->totalPow.imax/rate,'f',r)+suffix;
//    setTableRow(3, listStr);

//    if((unit->totalPow.ivalue < unit->totalPow.imin) || (unit->totalPow.ivalue > unit->totalPow.imax)) {
//        unit->totalPow.ialarm = 2;
//        setAlarmBackgroundColor(3);
//    } else {
//        unit->totalPow.ialarm = 0;
//        setNormalBackgroundColor(3);
//    }
    rate = COM_RATE_POW; suffix = "kW"; str = tr("总有功功率"); r = 3;
    addList(3 , &(unit->totalPow) ,  rate , suffix , str , r );

    rate = COM_RATE_POW; suffix = "kVA";str = tr("总视在功率");r = 3;
    listStr1.clear();
    listStr1 << str;
    listStr1 << QString::number(unit->totalApPow/rate,'f',r)+suffix;
    listStr1 << "---"<< "---";
    setTableRow(4, listStr1);
    rate = COM_RATE_ELE; suffix = "kWh";str = tr("总电能");r = 1;
    listStr1.clear();
    listStr1 << str;
    listStr1 << QString::number(unit->totalEle/rate,'f',r)+suffix;
    listStr1 << "---"<< "---";
    setTableRow(5, listStr1);

    rate = COM_RATE_POW;suffix = "kW";r = 3;
    int index = 3, outputNum = 3;
    for(int i = 0 ; i < 3 ; i++){
        str = tr("Output%1 有功功率").arg(i+1);
        addList(index*2+i , &(unit->outputXBox.outputXPow[i]) ,  rate , suffix , str , r );
    }

    rate = COM_RATE_POW; suffix = "kVA";r = 3;
    for(int i = 0 ; i < 3 ; i++){
        str = tr("Output%1 视在功率").arg(i+1);
        listStr1.clear();
        listStr1 << str;
        listStr1 << QString::number(unit->outputXBox.outputXApPow[i].ivalue/rate,'f',r)+suffix;
        listStr1 << "---"<< "---";
        setTableRow(index*2+outputNum+i, listStr1);
    }

    rate = COM_RATE_ELE; suffix = "kWh";r = 1;
    for(int i = 0 ; i < 3 ; i++){
        str = tr("Output%1 电能").arg(i+1);
        listStr1.clear();
        listStr1 << str;
        listStr1 << QString::number(unit->outputXBox.outputXEle[i]/rate,'f',r)+suffix;
        listStr1 << "---"<< "---";
        setTableRow(index*2+outputNum*2+i, listStr1);
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
