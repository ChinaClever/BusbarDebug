/*
 *
 *  Created on: 2020年10月1日
 *      Author: Lzy
 */
#include "ctrl_zpduthread.h"

Ctrl_ZpduThread::Ctrl_ZpduThread(QObject *parent)
{
    mItem = Cfg::bulid()->item;
    mModbus = Ad_Modbus::bulid(this);
}


Ctrl_ZpduThread *Ctrl_ZpduThread::bulid(QObject *parent)
{
    Ctrl_ZpduThread* sington = nullptr;
    if(sington == nullptr)
        sington = new Ctrl_ZpduThread(parent);
    return sington;
}


void Ctrl_ZpduThread::funSwitch(uchar *on, uchar *off)
{
    int k = 5, id = mItem->addr-1;
    static uchar cmd[68] = {0x7B, 0xC1, 0x00, 0xA2, 0xB2};

    for(int i=0; i<6; i++)  cmd[k++] = on[i];  //打开有效位
    for(int i=0; i<6; i++)  cmd[k++] = off[i];  //关闭有效位

    //预留位
    for(int i=0; i<3; i++)  cmd[k++] = 0xC7 + i;
    for(int i=0; i<3; i++)  cmd[k++] = 0xD7 + i;
    for(int i=0; i<39; i++)  cmd[k++] = 0x00;    
    if((on[id] == 0xFF) || (off[id] == 0xFF)) cmd[k++] = 1; // 统一开关为1
    else cmd[k++] = 0;

    for(int i=0; i<3; i++)  cmd[k++] = 0x0E;
    cmd[k++] = 0x44;
    cmd[k] = mModbus->getXorNumber(cmd,sizeof(cmd)-1);
    mModbus->writeSerial(cmd, sizeof(cmd));
}

void Ctrl_ZpduThread::openOnlySwitch(int id)
{
    openOutputSwitch(id);
    closeOtherSwitch(id);
}


void Ctrl_ZpduThread::openOutputSwitch(int id)
{
    uchar on[8], off[8];
    for(int i=0; i<6; i++)  on[i] = 0x00;  //打开有效位
    for(int i=0; i<6; i++)  off[i] = 0x00;  //关闭有效位

    setBitControl(id, on);
    funSwitch(on, off);
}

void Ctrl_ZpduThread::closeOtherSwitch(int id)
{
    uchar on[8], off[8];
    for(int i=0; i<6; i++)  on[i] = 0x00;  //打开有效位
    for(int i=0; i<6; i++)  off[i] = 0x00;  //关闭有效位

    setBitControl(id, off);
    for(int i=0; i<6; i++)  off[i] = ~off[i];
    funSwitch(on, off);
}

void Ctrl_ZpduThread::openAllSwitch()
{
    uchar on[8], off[8];
    for(int i=0; i<6; i++)  on[i] = 0xFF;  //打开有效位
    for(int i=0; i<6; i++)  off[i] = 0x00;  //关闭有效位

    funSwitch(on, off);
}

void Ctrl_ZpduThread::setBitControl(int id, uchar *buf)
{
    int index = (8 * (3-1) +id) / 8;
    int key = (8 * (3-1) +id) % 8 ;
    buf[index] |= 0x80 >> key;
}

bool Ctrl_ZpduThread::delay(int s)
{
    bool ret = true;
    for(int i=0; i<7*s; ++i) {
            msleep(100);
    }

    return ret;
}
