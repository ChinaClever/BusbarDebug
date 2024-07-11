/*
 *
 *  Created on: 2020年10月1日
 *      Author: Lzy
 */
#include "ad_corethread.h"
#include "dblogs.h"
#include "cores/test_network.h"
extern QString user_land_name();

Ad_CoreThread::Ad_CoreThread(QObject *parent) : BaseThread(parent)
{
    mLedSi = Ad_LedSi::bulid(this);
    mAdjust = Ad_Adjusting::bulid(this);
    mResult = Ad_Resulting::bulid(this);
}

Ad_CoreThread *Ad_CoreThread::bulid(QObject *parent)
{
    static Ad_CoreThread* sington = nullptr;
    if(sington == nullptr)
        sington = new Ad_CoreThread(parent);
    return sington;
}


bool Ad_CoreThread::workDown()
{
    bool ret = true; QString str;

    Dev_Object *dev = Dev_SiRtu::bulid();
    if(mItem->modeId == START_BUSBAR) dev = Dev_IpSnmp::bulid();
    if(mItem->modeId == START_BUSBAR){
        ret = mResult->setStartCurtype();
        ret = mAdjust->startAdjust();
        ret = Test_NetWork::bulid()->checkNet();
        if(!ret) mPro->step = Test_Fail;
        ret = mResult->resEnter();
        if(ret) mPro->step = Test_Seting;
        else mPro->step = Test_Fail;
        if(mPro->step == Test_Seting){
            ret = dev->readPduData();
            if(!ret) mPro->step = Test_Fail;
            else{
                mResult->setStartValue();
                dev->readPduData();
                mResult->compareStartValue();
            }
        }
    }else if(mItem->modeId == INSERT_BUSBAR){
        ret = mAdjust->startAdjust();
        if(mPro->step == Test_Seting){
            ret = dev->readPduData();
            if(!ret) mPro->step = Test_Fail;
            else{
                mResult->setInsertValue();
                int n = 3;
                while(n--)
                    dev->readPduData();
                mResult->compareInsertValue();
            }
        }
        if(mPro->step == Test_vert) {
            ret = mResult->resEnter();
        } else if(mPro->step == Test_Over) {
            ret = mResult->initRtuThread();
        }
    }else if(mItem->modeId == TEMPER_BUSBAR){
        ret = dev->readPduData();
        //设置温度基本信息（蜂鸣器、告警滤波）
        mResult->setEnvInfo();
        //温度模块设置阈值
        mResult->setEnvValue();

        //对比阈值信息
        dev->readPduData();
        mResult->compareEnvInfo();
        mResult->compareEnvValue();//温度阈值
    }

    //恢复出厂设置
    if(ret) {
        if(mItem->modeId == START_BUSBAR){//始端箱
            str = tr("始端箱恢复出厂设置");
            ret = Dev_SiCtrl::bulid()->setBusbarStartRestore(12);
        }else {
            str = tr("插接箱恢复出厂设置");
            ret = Dev_SiCtrl::bulid()->setBusbarInsertRestore(12);
        }
        updatePro(str);
    }

    return ret;
}

bool Ad_CoreThread::settingInsert()
{
    bool ret = false;
    Dev_Object *dev = Dev_SiRtu::bulid();
    ret = dev->readPduData();
    if(!ret) mPro->step = Test_Fail;
    else{
        mResult->setInsertValue();
        int n = 3;
        while(n--)
            dev->readPduData();
        mResult->compareInsertValue();
    }
    if(mPro->step == Test_vert) mPro->result = Test_Pass;
    else mPro->result = Test_Fail;
    mPro->step = Test_Over;
    return ret;
}

bool Ad_CoreThread::settingStart()
{
    bool ret = false;
    Dev_Object *dev = Dev_IpSnmp::bulid();
    ret = dev->readPduData();
    if(!ret) mPro->step = Test_Fail;
    else{
        mResult->setStartValue();
        dev->readPduData();
        mResult->compareStartValue();
    }
    if(mPro->step == Test_vert) mPro->result = Test_Pass;
    else mPro->result = Test_Fail;
    mPro->step = Test_Over;
    return ret;
}

