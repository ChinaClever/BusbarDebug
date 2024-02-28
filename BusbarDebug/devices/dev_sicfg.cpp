/*
 *
 *  Created on: 2021年1月1日
 *      Author: Lzy
 */
#include "dev_sicfg.h"

Dev_SiCfg::Dev_SiCfg(QObject *parent) : Dev_Object(parent)
{
    initCfg();
}

Dev_SiCfg *Dev_SiCfg::bulid(QObject *parent)
{
    static Dev_SiCfg* sington = nullptr;
    if(sington == nullptr)
        sington = new Dev_SiCfg(parent);
    return sington;
}

void Dev_SiCfg::initType()
{
    sTypeCfg *ptr = mCfg;
    ptr->si_baud = read("si_baud", 3).toInt();
    ptr->si_iOF = read("si_iOF", 0).toInt();
    ptr->si_buzzer = read("si_buzzer", 0).toInt();
    ptr->si_filter = read("si_filter", 5).toInt();

    //温度阈值
    ptr->tem_buzzer = read("tem_buzzer",0).toInt();
    ptr->tem_filter = read("tem_filter",5).toInt();
    ptr->temMin[0] = read("temMin_1",0).toDouble();
    ptr->temMin[1] = read("temMin_2",0).toDouble();
    ptr->temMin[2] = read("temMin_3",0).toDouble();
    ptr->temMin[3] = read("temMin_4",0).toDouble();
    ptr->temMax[0] = read("temMax_1",0).toDouble();
    ptr->temMax[1] = read("temMax_2",0).toDouble();
    ptr->temMax[2] = read("temMax_3",0).toDouble();
    ptr->temMax[3] = read("temMax_4",0).toDouble();
}

void Dev_SiCfg::writeType()
{
    sTypeCfg *ptr = mCfg;
    write("si_baud", ptr->si_baud);
    write("si_iOF", ptr->si_iOF);
    write("si_buzzer", ptr->si_buzzer);
    write("si_filter", ptr->si_filter);

    //温度阈值
    write("tem_buzzer", ptr->tem_buzzer);
    write("tem_filter", ptr->tem_filter);
    write("temMin_1", ptr->temMin[0]);
    write("temMin_2", ptr->temMin[1]);
    write("temMin_3", ptr->temMin[2]);
    write("temMin_4", ptr->temMin[3]);
    write("temMax_1", ptr->temMax[0]);
    write("temMax_2", ptr->temMax[1]);
    write("temMax_3", ptr->temMax[2]);
    write("temMax_4", ptr->temMax[3]);
}

void Dev_SiCfg::initData()
{
    sObjCfg *ptr = &(mCfg->si_cfg);
    initUnit("vol", ptr->vol);
    initUnit("cur", ptr->cur);

    initUnit("tem", ptr->tem);
    initUnit("pow", ptr->pow);

    //initUnit("zerocur", ptr->zerocur);////////////////////////////////
}

void Dev_SiCfg::writeData()
{
    sObjCfg *ptr = &(mCfg->si_cfg);
    writeUnit("vol", ptr->vol);
    writeUnit("cur", ptr->cur);

    writeUnit("tem", ptr->tem);
    writeUnit("pow", ptr->pow);

    //writeUnit("zerocur", ptr->zerocur);/////////////////////////////////////
}

void Dev_SiCfg::initCfg()
{
    initType();
    initData();
}

void Dev_SiCfg::save()
{
    writeType();
    writeData();
}
