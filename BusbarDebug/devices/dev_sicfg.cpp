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
}

void Dev_SiCfg::writeType()
{
    sTypeCfg *ptr = mCfg;
    write("si_baud", ptr->si_baud);
    write("si_iOF", ptr->si_iOF);
    write("si_buzzer", ptr->si_buzzer);
    write("si_filter", ptr->si_filter);
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
