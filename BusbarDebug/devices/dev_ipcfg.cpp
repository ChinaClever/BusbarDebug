/*
 *
 *  Created on: 2021年1月1日
 *      Author: Lzy
 */
#include "dev_ipcfg.h"

Dev_IpCfg::Dev_IpCfg(QObject *parent) : Dev_Object(parent)
{
    initCfg();
}


Dev_IpCfg *Dev_IpCfg::bulid(QObject *parent)
{
    static Dev_IpCfg* sington = nullptr;
    if(sington == nullptr)
        sington = new Dev_IpCfg(parent);
    return sington;
}


void Dev_IpCfg::initType()
{
    sTypeCfg *ptr = mCfg;
    ptr->ip_curtype = read("ip_curtype", 2).toInt();
    ptr->ip_buzzer = read("ip_buzzer", 0).toInt();
    ptr->ip_filter = read("ip_filter", 5).toInt();

    ptr->ip_mode = read("ip_mode", 1).toInt();
    ptr->ip_num = read("ip_num", 6).toInt();
    ptr->ip_ISD = read("ip_ISD", 0).toInt();
    ptr->ip_iOF = read("ip_iOF", 0).toInt();
    ptr->ip_shunt = read("ip_shunt", 0).toInt();
    ptr->ip_residual = read("ip_residual", 0).toInt();
    ptr->ip_lightning = read("ip_lightning", 0).toInt();



}

void Dev_IpCfg::writeType()
{
    sTypeCfg *ptr = mCfg;
    write("ip_curtype", ptr->ip_curtype);
    write("ip_buzzer", ptr->ip_buzzer);
    write("ip_filter", ptr->ip_filter);

    write("ip_mode", ptr->ip_mode);
    write("ip_num", ptr->ip_num);
    write("ip_ISD", ptr->ip_ISD);
    write("ip_iOF", ptr->ip_iOF);
    write("ip_shunt", ptr->ip_shunt);
    write("ip_residual", ptr->ip_residual);
    write("ip_lightning", ptr->ip_lightning);
}

void Dev_IpCfg::initData()
{
    sObjCfg *ptr = &(mCfg->ip_cfg);
    initUnit("vol", ptr->vol);
    initUnit("cur", ptr->cur);
    initUnit("tem", ptr->tem);
    initUnit("pow", ptr->pow);


    initUnit("totalpow", ptr->totalpow);
    initUnit("linevol", ptr->linevol);
    initUnit("hz", ptr->hz);
    initUnit("zerocur", ptr->zerocur);
    initUnit("recur", ptr->recur);
}

void Dev_IpCfg::writeData()
{
    sObjCfg *ptr = &(mCfg->ip_cfg);
    writeUnit("vol", ptr->vol);
    writeUnit("cur", ptr->cur);
    writeUnit("tem", ptr->tem);
    writeUnit("pow", ptr->pow);

    writeUnit("totalpow", ptr->totalpow);
    writeUnit("linevol", ptr->linevol);
    writeUnit("hz", ptr->hz);
    writeUnit("zerocur", ptr->zerocur);
    writeUnit("recur", ptr->recur);
}

void Dev_IpCfg::initCfg()
{
    initType();
    initData();
}

void Dev_IpCfg::save()
{
    writeType();
    writeData();
}
