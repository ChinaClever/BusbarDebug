/*
 *
 *  Created on: 2021年1月1日
 *      Author: Lzy
 */
#include "test_corethread.h"

Test_CoreThread::Test_CoreThread(QObject *parent) : BaseThread(parent)
{

}

void Test_CoreThread::initFunSlot()
{
    Dev_SiRtu::bulid(this);
    Dev_IpRtu::bulid(this);
    Dev_IpSnmp::bulid(this);
    Test_NetWork::bulid(this);
    mYc = Yc_Obj::bulid(this);

    mCtrl = Test_SiThread::bulid(this);
    mAd = Ad_CoreThread::bulid(this);
    mSn = Sn_SerialNum::bulid(this);
}

bool Test_CoreThread::setDev()
{
    bool ret = true;
    // if(mItem->modeId) {
    //     if(mPro->step > Test_Seting) ret = delay(6);
    //     else ret = Test_NetWork::bulid()->startProcess();
    // } else {
    //     ret = mCtrl->setDev();
    //     if(mCfg->si_led) return ret;
    // }
    if(ret) ret = mSn->snEnter();
    return ret;
}

bool Test_CoreThread::readDev()
{
    bool ret = true;
    Dev_Object *dev = Dev_IpRtu::bulid();
    if(mItem->modeId) {
        QString str = tr("Modbus RTU通讯 ");
        ret = dev->readPduData();
        if(ret) str += tr("正常"); else str += tr("错误");
        updatePro(str, ret);

        if(mCfg->ip_modbus) {
        } else {
            if(mPro->step >= Test_Seting) return ret;
            ret = Test_NetWork::bulid()->checkNet(); if(!ret) return ret;
            //str = tr("SNMP通讯 ");
            //Dev_IpSnmp *snmp = Dev_IpSnmp::bulid(); ret = snmp->readPduData();
            //if(!ret) {snmp->resetSnmp(); ret = snmp->readPduData();}
            //if(ret) str += tr("正常"); else str += tr("错误");
            //updatePro(str, ret, 1);
        }
    }

    return ret;
}

bool Test_CoreThread::checkDev()
{
    QString str = tr("设备类型验证");
    bool ret = mDt->devType == mItem->modeId ? true:false;
    if(ret) str += tr("正常"); else str += tr("错误");
    ret = updatePro(str, ret);
    if(ret) {
        if(mDt->devType) {
            if(mCfg->ip_ac == AC) {
                str = tr("设备相数验证");
                int lines = mCfg->ip_lines;
                ret = mDt->lines == lines ? true:false;
                if(ret) str += tr("正常"); else str += tr("错误");
                updatePro(str, ret);
            }

            str = tr("设备版本验证");
            if(mDt->version == mCfg->ip_version)
                ret = true;
            else if( IP_PDUV1_HUADA  == mCfg->ip_version)
                ret = true;
            else if( IP_PDUV3_EATON - 2  == mCfg->ip_version)
                ret = false;
            if(ret) str += tr("正常"); else str += tr("错误");
            updatePro(str, ret);
        }
    }

    return ret;
}

void Test_CoreThread::workResult()
{
    BaseLogs::bulid()->start();
    bool res = mYc->powerDown();
    // sleep(2);
    // Json_Pack::bulid()->http_post("debugdata/add","192.168.1.12");//全流程才发送记录(http)

    QString str = tr("最终结果 ");
    if(mPro->result != Test_Fail) {
        str += tr("通过");
        mPro->uploadPassResult = 1;
        // mPro->result = Test_Over;
    } else {
        res = false;
        str += tr("失败");
        mPro->uploadPassResult = 0;
        // mPro->result = Test_Fail;
    }

    updatePro(str, res, 2);
    mPro->step = Test_Over;
}


bool Test_CoreThread::initFun()
{
    updatePro(tr("即将开始"));
    bool ret = false;
    if(mItem->modeId == 0){
        ret = mYc->powerOn(50);
        sleep(25);
    }else if(mItem->modeId == 1){//插接箱
        ret = mYc->powerOn();
        sleep(5);
        if(ret) ret = setDev();//设置序列号
        // if(ret) ret = readDev();
    }
    else if(mItem->modeId == 2){//温度传感器
        ret = mYc->powerOn(0);
        sleep(5);
        if(ret) ret = setDev();//设置序列号
        // if(ret) ret = readDev();
    }

    return ret;
}


bool Test_CoreThread::workDown()
{
    bool ret = Ad_Resulting::bulid(this)->initRtuThread();

    ret = mAd->startAdjust();
    return ret;
}

void Test_CoreThread::collectData()
{
    int cnt = 0;
    Dev_Object *dev = Dev_SiRtu::bulid();
    if(mItem->modeId == START_BUSBAR) dev = Dev_IpSnmp::bulid();

    while(mPro->step == Test_Collect) {
        bool ret = dev->readPduData();
        if(ret && (++cnt%30)) continue;
        QString str = tr("正在读取设备数据 %1").arg(cnt);
        if(!ret && (cnt>5)) str= tr("读取设备数据错误！");
        updatePro(str, ret, 2);
    }

    QString str = tr("读取设备数据停止！");
    updatePro(str);
}

void Test_CoreThread::allTest()
{
    bool ret = initFun();
    if(ret) {
        ret = workDown();
    } else mPro->result = Test_Fail;

    workResult();
}

void Test_CoreThread::run()
{
    if(mItem->modeId == START_BUSBAR)mDt->devType = START_BUSBAR;
    else if(mItem->modeId == INSERT_BUSBAR)mDt->devType = INSERT_BUSBAR;
    else if(mItem->modeId == TEMPER_BUSBAR)mDt->devType = TEMPER_BUSBAR;
    if(isRun) return; else isRun = true;
    //    Dev_IpSnmp *devip = Dev_IpSnmp::bulid();
    //    devip->readPduData();
    //    Ad_Resulting::bulid()->setStartValue();
    //    devip->readPduData();
    //    Ad_Resulting::bulid()->compareStartValue();
    //    Dev_SiRtu *devip = Dev_SiRtu::bulid();//rtu
    //    devip->readPduData();
    //    Ad_Resulting::bulid()->setInsertValue();
    //    devip->readPduData();
    //    Ad_Resulting::bulid()->compareInsertValue();
    //    BaseLogs::bulid()->start();
    mPro->recordstep = mPro->step;
    switch (mPro->step) {
    case Test_Start: allTest(); break;
    case Test_Collect: collectData(); break;
    }

    isRun = false;
}
