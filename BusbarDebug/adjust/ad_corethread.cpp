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
    bool ret = true;
    ret = mResult->setStartCurtype();
    ret = mAdjust->startAdjust();

    Dev_Object *dev = Dev_SiRtu::bulid();
    if(mItem->modeId == START_BUSBAR) dev = Dev_IpSnmp::bulid();
    if(mItem->modeId == START_BUSBAR){
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
    ret = mResult->resEnter();
    if(mPro->step == Test_Over) {
        ret = mResult->initRtuThread();
    }
    return ret;
}

