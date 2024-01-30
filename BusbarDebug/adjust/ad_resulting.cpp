/*
 *
 *  Created on: 2020年10月1日
 *      Author: Lzy
 */
#include "ad_resulting.h"
#define AD_CUR_RATE 100
#include <QDateTime>
#include "cores/test_network.h"

Ad_Resulting::Ad_Resulting(QObject *parent) : BaseThread(parent)
{
    mObj = &(mDataBusbar->box[mItem->addr-1].data);
}

Ad_Resulting *Ad_Resulting::bulid(QObject *parent)
{
    static Ad_Resulting* sington = nullptr;
    if(sington == nullptr)
        sington = new Ad_Resulting(parent);
    return sington;
}

bool Ad_Resulting::curErrRange(int exValue, int cur)
{
    bool ret = false;

    int err = exValue * mItem->curErr/1000.0;

    //    int min = (exValue - mItem->curErr * 10);
    //    int max = (exValue + mItem->curErr * 10);
    int min = (exValue - err);
    int max = (exValue + err);
    qDebug()<<"cur "<<cur<<"min "<<min<<"max "<<max;
    if((cur >= min) && (cur <= max )) {
        ret =  true;
    }

    return ret;
}

bool Ad_Resulting::powErrRange(int exValue, int pow)
{
    bool ret = false;
    int err = exValue * mItem->powErr/1000.0;
    int min = exValue - err;
    int max = exValue + err;
    if((pow >= min) && (pow <= max )) {
        ret =  true;
    } else {
        qDebug() << "pow Err Range" << pow << exValue << err;
    }

    return ret;
}

bool Ad_Resulting::powRangeByID(int i, int exValue, int cnt)
{
    mObj = &(mDataBusbar->box[mItem->addr-1].data);
    exValue = mItem->vol * exValue/AD_CUR_RATE; //mData->cur.value[i]/COM_RATE_CUR;
    exValue *= 0.5;
    QString str = tr("期望功率%1kW 第%2位 功率 ").arg(exValue/1000.0).arg(i+1);
    //    int crate = 1;
    //    if(mItem->modeId == START_BUSBAR) crate = mCfg->ip_cfg.pow.rate;
    //    else crate = mCfg->si_cfg.pow.rate;
    //    mObj->pow.value[i] *= crate;
    bool ret = powErrRange(exValue, mObj->pow.value[i]);
    mObj->pow.valued[i] = mObj->pow.value[i];
    if(ret) {
        str += tr("正常");
        updatePro(str);
        mObj->cur.status[i] = Test_Pass;
    } else {
        ret = false;
        if(cnt > 3) {
            str += tr("错误");
            updatePro(str, ret, 1);
            mObj->cur.status[i] = Test_Fail;
        }
    }

    return ret;
}

bool Ad_Resulting::curRangeByID(int i, int exValue, int cnt)
{
    mObj = &(mDataBusbar->box[mItem->addr-1].data);
    int cur = mObj->cur.value[i] ;
    //    if(mDt->lines == 2 &&i == 0) exValue *= 2;
    QString str = tr("期望电流%1A: 实际电流 %2A 第%3位 电流 ").arg(exValue/AD_CUR_RATE).arg(mObj->cur.value[i]/COM_RATE_CUR).arg(i+1);
    bool ret = curErrRange(exValue, cur);
    mObj->cur.valued[i] = mObj->cur.value[i];
    if(ret) {
        //if(mCfg->si_led) return ret;   ////===========
        ret = powRangeByID(i, exValue, cnt);
        if(ret){str += tr("正常"); updatePro(str);}
    } else {
        ret = false;
        if(cnt > 3) {
            str += tr("错误");
            updatePro(str, ret, 0);
            mObj->cur.status[i] = Test_Fail;
        }
    }

    return ret;
}

bool Ad_Resulting::volErrRangeByID(int i)
{
    mObj = &(mDataBusbar->box[mItem->addr-1].data);
    bool ret = true;
    int err = mItem->vol * mItem->curErr/1000.0;
    int vol = mObj->vol.value[i];
    int min = (mItem->vol - err);
    int max = (mItem->vol + err);
    QString str = tr("期望电压200V，实际电压%1V 第%2位 电压 ").arg(vol).arg(i+1);
    if((vol >= min) && (vol <= max)) {
        str += tr("正常");
        updatePro(str);
        mData->status[i] = Test_Pass;
    } else {
        ret = false;
    }

    return ret;
}

bool Ad_Resulting::volErrRange()
{
    int k = 0;
    bool ret = mCollect->readPduData();
    for(int i=0; i<mData->size; ++i) {
        ret = volErrRangeByID(i);
        if(!ret) {
            if(k++ < 5){
                i = -1; delay(2);
                mCollect->readPduData();
            } else {
                ret = false;
                mData->status[i] = Test_Fail;
                QString str = tr("检测到电压 %1 错误").arg(i+1);
                updatePro(str, ret, 1); break;
            }
        }
    }

    return ret;
}

bool Ad_Resulting::eachCurCheck(int k, int exValue)
{
    bool ret = true;
    double value = mItem->vol*exValue/AD_CUR_RATE/1000.0;
    if(AC == mDt->ac) value = value*0.5;
    QString str = tr("校验数据: 期望电流%1A 功率%2kW").arg(exValue/AD_CUR_RATE).arg(value);
    updatePro(str);
    for(int i=0; i<5; ++i) {
        if(i) str += tr(" 第%1次").arg(i+1); //else delay(4);
        ret = curRangeByID(k, exValue, i);
        if(ret) break; else if(!delay(i+5)) break;
        mCollect->readPduData();
    }

    return ret;
}

bool Ad_Resulting::eachCurCheckInAdjusting(int k, int exValue)
{
    bool ret = false;
    //if(AC == mDt->ac) value = value*0.5;

    ret = curRangeByIDInAdjusting(k, exValue);
    QString str = tr("检查数据: 期望电流%1A").arg(exValue/AD_CUR_RATE);
    updatePro(str);
    mCollect->readPduData();

    return ret;
}

bool Ad_Resulting::curRangeByIDInAdjusting(int i, int exValue)
{
    int cur = mObj->cur.value[i] * 10 ;
    if(mDt->lines == 2 &&i == 0) exValue *= 2;
    QString str = tr("期望电流%1A 第%2位 电流 ").arg(exValue/AD_CUR_RATE).arg(i+1);
    bool ret = curErrRange(exValue, cur);
    mObj->cur.valued[i] = mObj->cur.value[i];
    if(ret) {
        //if(mCfg->si_led) return ret;   ////===========
        ret = volErrRangeByIDInAdjusting(i);
        if(ret) str += tr("正常");
        updatePro(str);
    } else {
        ret = false;
    }

    return ret;
}

bool Ad_Resulting::volErrRangeByIDInAdjusting(int i)
{
    mObj = &(mDataBusbar->box[mItem->addr-1].data);
    bool ret = true;
    int vol = mObj->vol.value[i];

    int crate = 1;
    if(mItem->modeId == START_BUSBAR) crate = mCfg->ip_cfg.vol.rate;
    else crate = mCfg->si_cfg.vol.rate;
    int min = (220 - mItem->volErr)*crate;
    int max = (220 + mItem->volErr)*crate;
    QString str = tr("期望电压220V，实际电压%1V 第%2位 电压 ").arg(vol/(crate*1.0)).arg(i+1);
    if((vol >= min) && (vol <= max)) {
        str += tr("正常");
        updatePro(str);
    } else {
        ret = false;
    }

    return ret;
}

bool Ad_Resulting::eachCurEnter(int exValue)
{
    bool res = true;
    mCollect->readPduData();
    for(int k=0; k<mObj->lineNum; ++k) {
        bool ret = eachCurCheck(k, exValue);
        if(!ret) res = false;
    }

    return res;
}

bool Ad_Resulting::eachCurEnterOneByOne(int exValue)
{
    bool res = true;
    mCollect->readPduData();
    mControlOp = Ctrl_ZpduThread::bulid();
    int lines = mDt->lines;
    for(int k=0; k<lines; ++k) {
        if( mControlOp ){
            mControlOp->openOnlySwitch(k);
            mControlOp->delay(5);
            mControlOp->openOnlySwitch(k);
        }
        else
            return false;
        bool ret = eachCurCheck(lines==2 ?k+1:k, exValue);
        if(!ret) res = false;
    }

    return res;
}

bool Ad_Resulting::initRtuThread()
{
    switch (mItem->modeId) {
    case START_BUSBAR: mCollect = Dev_IpSnmp::bulid(this); break;
    case INSERT_BUSBAR: mCollect = Dev_SiRtu::bulid(this); break;
    default: mCollect = nullptr; break;
    }

    return mCollect->readPduData();
}

bool Ad_Resulting::workDown(int exValue)
{
    return eachCurEnter(exValue);
}

bool Ad_Resulting::noLoadCurCheck(int cnt)
{
    bool res = true;
    for(int k=0; k<mObj->lineNum; ++k) {
        mObj->pow.valued[k] = mObj->pow.value[k];
        mObj->cur.valued[k] = mObj->cur.value[k];
        QString str = tr("空载校验: 第%1相 ").arg(k+1);
        if(mObj->cur.value[k] || mObj->pow.value[k]) {
            res = false;
            if(cnt > 3) {
                mObj->cur.status[k] = Test_Fail;
                if(mObj->cur.value[k]) str += tr("电流有底数");
                if(mObj->pow.value[k]) str += tr("功率有底数");
                updatePro(str, res, 1);
            }
        } else {
            mObj->cur.status[k] = Test_Pass;
            str += tr("通过");
            updatePro(str);
        }
    }

    return res;
}

bool Ad_Resulting::noLoadCurFun()
{
    bool ret = true;
    for(int i=0; i<5; ++i) {
        QString str = tr("空载校验: 第%1次检查").arg(i+1);
        if(i)updatePro(str, true, 5); else delay(6);
        mCollect->readPduData();
        ret = noLoadCurCheck(i);
        if(ret) break; else if(!delay(i+4)) break;
    }

    return ret;
}

bool Ad_Resulting::noLoadEnter()
{
    bool ret = mSource->setCur(0, 0);
    if(ret) ret = volErrRange();

    QString str = tr("空载验证：设置空载电流");
    updatePro(str, ret, 5);
    if(ret) ret = noLoadCurFun();
    return ret;
}

bool Ad_Resulting::powerOn()
{
    initRtuThread();
    updatePro(tr("自动验证开始"),true, 2);
    mSource = Yc_Obj::bulid()->get();
    updatePro(tr("等待设备重启"));
    mSource->setVol(0,3);
    mSource->setCur(0,0);

    updatePro(tr("等待设备稳定"));
    mPro->step = Test_vert;
    mSource->setVol(200, 15);
    bool ret = mSource->setCur(40, 10);
    QString str = tr("验证电流：期望电流4A");
    return updatePro(str, ret, 3);
}

bool Ad_Resulting::resEnter()
{
    bool ret = powerOn();
    if(ret) {
        ret = workDown(4*AD_CUR_RATE);
        if(ret) ret = noLoadEnter();
    }

    return ret;
}

bool Ad_Resulting::workDownOneByOne(int exValue)
{
    return eachCurEnterOneByOne(exValue);
}

bool Ad_Resulting::resEnterOneByOne()
{
    bool ret = powerOn();
    if(ret) {
        ret = workDownOneByOne(4*AD_CUR_RATE);
        if(ret) ret = noLoadEnter();
    }

    return ret;
}


void Ad_Resulting::setStartInfo()
{
    QString str = tr("配置始端箱基本信息！");
    updatePro(str);
    Dev_IpSnmp *devip = Dev_IpSnmp::bulid();
    devip->SetInfo(devip->getCurTpyeOid(), QString::number(mCfg->ip_curtype));
    if(mCfg->ip_mode == 1)
        devip->SetInfo(devip->getModeOid(), QString::number(mCfg->ip_mode));
    devip->SetInfo(devip->getNumOid(), QString::number(mCfg->ip_num));
    devip->SetInfo(devip->getButterOid(), QString::number(mCfg->ip_buzzer));
    devip->SetInfo(devip->getFilterOid(), QString::number(mCfg->ip_filter));
    devip->SetInfo(devip->getLightningOid(), QString::number(mCfg->ip_lightning));
    devip->SetInfo(devip->getiOFOid(), QString::number(mCfg->ip_iOF));
    devip->SetInfo(devip->getISDOid(), QString::number(mCfg->ip_ISD));
    devip->SetInfo(devip->getShuntOid(), QString::number(mCfg->ip_shunt));
    devip->SetInfo(devip->getResidualOid(), QString::number(mCfg->ip_residual));
}

bool Ad_Resulting::setStartCurtype()
{
    QString str = tr("更改始端箱电流规格为---标准！");
    updatePro(str);
    Dev_IpSnmp *devip = Dev_IpSnmp::bulid();
    return devip->SetInfo(devip->getCurTpyeOid(), QString::number(0));
}

void Ad_Resulting::setStartLineValue()
{
    Dev_IpSnmp *devip = Dev_IpSnmp::bulid();
    QString info = tr("配置始端箱电压等电气阈值信息！");
    updatePro(info);

    QVector<int> ans = getStartLineValue();
    for(int i = 0 ; i < START_LINE_NUM ; i++){
        for(int j = 0 ; j < ans.size(); j++){
            QString str = devip->getLineVolMinOid(i , j);
            devip->SetInfo(str, QString::number(ans.at(j)));
        }
    }
}

void Ad_Resulting::setStartOtherValue()
{
    Dev_IpSnmp *devip = Dev_IpSnmp::bulid();

    QString info = tr("配置始端箱其他阈值信息！");
    updatePro(info);
    QVector<int> ans = getStartOtherValue();

    for(int j = 0 ; j < 19 - 13 + 1; j++){
        QString str = devip->getOtherMinOid(j);
        devip->SetInfo(str, QString::number(ans.at(j)));
    }

}

void Ad_Resulting::setStartEnvValue()
{
    Dev_IpSnmp *devip = Dev_IpSnmp::bulid();
    sObjCfg *it = &(mCfg->ip_cfg);
    QString info = tr("配置始端箱温度阈值信息！");
    updatePro(info);
    QVector<int> ans;
    ans.append(it->tem.min*it->tem.rate);
    ans.append(it->tem.max*it->tem.rate);

    for(int j = 0 ; j < RTU_TH_NUM*2; j+=2){
        QString str = devip->getTemMinOid(j);
        devip->SetInfo(str, QString::number(ans.at(0)));
        str = devip->getTemMinOid(j+1);
        devip->SetInfo(str, QString::number(ans.at(1)));
    }
}

void Ad_Resulting::setStartMACAddress()
{
    Dev_IpSnmp *devip = Dev_IpSnmp::bulid();
    if( mDataBusbar->box[mItem->addr-1].mac.contains("ff:ff:ff:ff:ff:ff")
        || !mDataBusbar->box[mItem->addr-1].mac.contains("2c:26:5f:")){
        Test_NetWork::bulid()->addMacAddr1();
        devip->SetInfo(devip->getMACAddress(), mItem->mac);
    }
}

void Ad_Resulting::setStartValue()
{
    setStartInfo();
    setStartLineValue();
    setStartOtherValue();
    setStartEnvValue();
    setStartMACAddress();
}

QString Ad_Resulting::changeCurType(int index)
{
    QString str = tr("标准");
    switch(index){
    case 0:str = tr("标准");break;
    case 1:str = tr("250 A");break;
    case 2:str = tr("400 A");break;
    case 3:str = tr("650 A");break;
    default:str = tr("标准");break;
    }
    return str;
}

QString Ad_Resulting::changeMode(int index)
{
    QString str = tr("主机地址 1 / Modbus模式");
    switch(index){
    case 0:str = tr("主机地址 0 / SNMP模式");break;
    case 1:str = tr("主机地址 1 / Modbus模式");break;
    default:str = tr("主机地址 1 / Modbus模式");break;
    }
    return str;
}

QString Ad_Resulting::changeBaudType(int index)
{
    QString str = tr("9600");
    switch(index){
    case 1:str = tr("9600");break;
    case 2:str = tr("19200");break;
    case 3:str = tr("38400");break;
    case 4:str = tr("57600");break;
    default:str = tr("9600");break;
    }
    return str;
}

void Ad_Resulting::compareStartInfo()
{
    QString str = tr("对比始端箱基本信息！");
    updatePro(str);
    sBoxData* b = &(mPacket->share_mem_get()->box[mItem->addr-1]);
    bool ret = false;
    int curValue = b->curSpecification;
    int expect = mCfg->ip_curtype;
    if(curValue == expect) ret = true;
    str = tr("始端箱电流规格实际值：%1 , 期待值：%2！").arg(changeCurType(curValue)).arg(changeCurType(expect));
    updatePro(str,ret);ret = false;

    curValue = b->workMode;
    expect = mCfg->ip_mode;
    if(curValue == expect) ret = true;
    str = tr("始端箱通讯模式实际值：%1 , 期待值：%2！").arg(changeMode(curValue)).arg(changeMode(expect));
    updatePro(str,ret);ret = false;

    curValue = mPacket->share_mem_get()->boxNum;
    expect = mCfg->ip_num;
    if(curValue == expect) ret = true;
    str = tr("始端箱级联数目实际值：%1 , 期待值：%2！").arg(curValue).arg(expect);
    updatePro(str,ret);ret = false;

    curValue = b->buzzerStatus;
    expect = mCfg->ip_buzzer;
    if(curValue == expect) ret = true;
    str = tr("始端箱蜂鸣器实际值：%1 , 期待值：%2！").arg(curValue?tr("关闭"):tr("开启")).arg(expect?tr("关闭"):tr("开启"));
    updatePro(str,ret);ret = false;

    curValue = b->alarmTime;
    expect = mCfg->ip_filter;
    if(curValue == expect) ret = true;
    str = tr("始端箱过滤次数实际值：%1 , 期待值：%2！").arg(curValue).arg(expect);
    updatePro(str,ret);ret = false;

    curValue = b->lightning;
    expect = mCfg->ip_lightning;
    if(curValue == expect) ret = true;
    str = tr("始端箱防雷模块实际值：%1 , 期待值：%2！").arg(curValue?tr("有"):tr("无")).arg(expect?tr("有"):tr("无"));
    updatePro(str,ret);ret = false;

    curValue = b->iOF;
    expect = mCfg->ip_iOF;
    if(curValue == expect) ret = true;
    str = tr("始端箱iOF辅助触点实际值：%1 , 期待值：%2！").arg(curValue?tr("有"):tr("无")).arg(expect?tr("有"):tr("无"));
    updatePro(str,ret);ret = false;

    curValue = b->isd;
    expect = mCfg->ip_ISD;
    if(curValue == expect) ret = true;
    str = tr("始端箱ISD报警触点实际值：%1 , 期待值：%2！").arg(curValue?tr("有"):tr("无")).arg(expect?tr("有"):tr("无"));
    updatePro(str,ret);ret = false;

    curValue = b->shuntRelease;
    expect = mCfg->ip_shunt;
    if(curValue == expect) ret = true;
    str = tr("始端箱分励脱扣实际值：%1 , 期待值：%2！").arg(curValue?tr("有"):tr("无")).arg(expect?tr("有"):tr("无"));
    updatePro(str,ret);ret = false;

    curValue = b->reState;
    expect = mCfg->ip_residual;
    if(curValue == expect) ret = true;
    str = tr("始端箱剩余电流实际值：%1 , 期待值：%2！").arg(curValue?tr("有"):tr("无")).arg(expect?tr("有"):tr("无"));
    updatePro(str,ret);ret = false;
}

QVector<int> Ad_Resulting::getStartLineValue()
{
    sObjCfg *it = &(mCfg->ip_cfg);
    QVector<int> ans;
    ans.append(it->linevol.min*it->linevol.rate);
    ans.append(it->linevol.max*it->linevol.rate);
    ans.append(it->vol.min*it->vol.rate);
    ans.append(it->vol.max*it->vol.rate);
    ans.append(it->cur.min*it->cur.rate);
    ans.append(it->cur.max*it->cur.rate);
    ans.append(it->pow.min*it->pow.rate);
    ans.append(it->pow.max*it->pow.rate);
    return ans;
}

QVector<int> Ad_Resulting::getStartOtherValue()
{
    sObjCfg *it = &(mCfg->ip_cfg);
    QVector<int> ans;
    ans.append(it->recur.max*it->recur.rate);
    ans.append(it->zerocur.min*it->zerocur.rate);
    ans.append(it->zerocur.max*it->zerocur.rate);
    ans.append(it->totalpow.min*it->totalpow.rate);
    ans.append(it->totalpow.max*it->totalpow.rate);
    ans.append(it->hz.min*it->hz.rate);
    ans.append(it->hz.max*it->hz.rate);
    return ans;
}

void Ad_Resulting::compareStartLineValue()
{
    sObjCfg *it = &(mCfg->ip_cfg);
    sObjectData* b = &(mPacket->share_mem_get()->box[mItem->addr-1]).data;
    QString info = tr("对比始端箱电压等电气阈值信息！");
    updatePro(info);
    bool ret = false;
    int curValue = 0 , expect = 0;
    QVector<int> ans = getStartLineValue();

    QString str = tr("最小值");
    QString v = tr("线电压");
    QString r = tr("V");
    double rate = it->linevol.rate;
    for(int i = 0 ; i < START_LINE_NUM ; i++){
        for(int j = 0 ; j < ans.size() ; j++){
            if( j <= 1){
                curValue = (j % 2 == 0)?b->lineVol.min[i]:b->lineVol.max[i];
                v = tr("线电压");r = tr("V");rate = it->linevol.rate;
            }else if( j <= 3){
                curValue = (j % 2 == 0)?b->vol.min[i]:b->vol.max[i];
                v = tr("电压");r = tr("V");rate = it->vol.rate;
            }else if( j <= 5){
                curValue = (j % 2 == 0)?b->cur.min[i]:b->cur.max[i];
                v = tr("电流");r = tr("A");rate = it->cur.rate;
            }else if( j <= 7){
                curValue = (j % 2 == 0)?b->pow.min[i]:b->pow.max[i];
                v = tr("功率");r = tr("kW");rate = it->pow.rate;
            }
            str = (j % 2 == 0)?tr("最小值"):tr("最大值");;
            expect = ans.at(j);
            if(curValue == expect) ret = true;
            info = tr("始端箱L%1 %2 %3实际值：%4 %6, 期待值：%5 %6！")
                    .arg(i+1).arg(v).arg(str).arg(curValue/rate).arg(expect/rate).arg(r);
            updatePro(info,ret);ret = false;
        }
    }
}

bool Ad_Resulting::check(QString name , QString str , QString r, double rate , int curValue , int expect)
{
    bool ret = false;
    if(curValue == expect) ret = true;
    QString info = tr("始端箱%1 %2实际值：%3 %5, 期待值：%4 %5！")
            .arg(name).arg(str).arg(curValue/rate).arg(expect/rate).arg(r);
    updatePro(info,ret);ret = false;
    return ret;
}

void Ad_Resulting::compareStartOtherValue()
{
    sBoxData* b = &(mPacket->share_mem_get()->box[mItem->addr-1]);
    sObjCfg *it = &(mCfg->ip_cfg);

    QString info = tr("对比始端箱其他阈值信息！");
    updatePro(info);
    int curValue = 0 , expect = 0;
    double rate = it->recur.rate;
    QVector<int> ans = getStartOtherValue();
    QString name , str, r;

    for(int i = 0 ; i < ans.size(); i++){
        str = (i % 2)?tr("最小值"):tr("最大值");
        if(i == 0){
            curValue = b->reCur.smax;
            name = tr("剩余电流") ; str = tr("最大值") ; r = tr("A");
            rate = it->recur.rate;
        }else if( i <= 2){
            curValue = (i % 2)?b->zeroLineCur.smin:b->zeroLineCur.smax;
            name = tr("零线电流") ; r = tr("A");
            rate = it->zerocur.rate;
        }else if( i <= 4){
            curValue = (i % 2)?b->totalPow.imin:b->totalPow.imax;
            name = tr("功率") ; r = tr("kW");
            rate = it->totalpow.rate;
        }else{
            curValue = (i % 2)?b->rate.smin:b->rate.smax;
            name = tr("频率") ; r = tr("Hz");
            rate = it->hz.rate;
        }
        expect = ans.at(i);
        check(name , str , r , rate , curValue , expect);
    }
}

void Ad_Resulting::compareStartEnvValue()
{
    sEnvData *b = &(mPacket->share_mem_get()->box[mItem->addr-1].env);
    sObjCfg *it = &(mCfg->ip_cfg);
    int expect = 0 ,curValue = 0;
    double rate = it->tem.rate;
    bool ret = false;
    int idx = 1;
    QString info = tr("对比始端箱温度阈值信息！");
    updatePro(info);
    QVector<int> ans;
    ans.append(it->tem.min*it->tem.rate);
    ans.append(it->tem.max*it->tem.rate);

    QString str = tr("最小值");
    for(int j = 0 ; j < RTU_TH_NUM*2; j++){
        expect = (j % 2==0)?ans.at(0):ans.at(1);
        curValue = (j % 2==0)?b->tem.min[j/2]:b->tem.max[j/2];
        str = (j % 2==0)?tr("最小值"):tr("最大值");
        idx = j/2 + 1;
        if(curValue == expect) ret = true;
        info = tr("始端箱温度 %1 %2实际值：%3 ℃, 期待值：%4 ℃！")
                .arg(idx).arg(str).arg(curValue/rate).arg(expect/rate);
        updatePro(info,ret);ret = false;
    }
}

void Ad_Resulting::compareStartMac()
{
    bool ret = false;
    if( mDataBusbar->box[mItem->addr-1].mac.contains("2c:26:5f:") ){
        ret = true;
        QString info = tr("始端箱MAC Address :%1 设置成功！")
                .arg(mDataBusbar->box[mItem->addr-1].mac);
        updatePro(info,ret);
    }else{
        QString info = tr("始端箱MAC Address :%1 设置失败！")
                .arg(mDataBusbar->box[mItem->addr-1].mac);
        updatePro(info,ret);
    }
}

void Ad_Resulting::compareStartValue()
{
    compareStartInfo();
    compareStartLineValue();
    compareStartOtherValue();
    compareStartEnvValue();
    compareStartMac();
    mPro->step = Test_vert;
}

void Ad_Resulting::setInsertInfo()
{
    QString info = tr("设置插接箱基本信息！");
    updatePro(info);
    Dev_SiCtrl::bulid()->setBusbarInsertiOF(mCfg->si_iOF);
    //Dev_SiCtrl::bulid()->setBusbarInsertBaud(mCfg->si_baud+1);
    Dev_SiCtrl::bulid()->setBusbarInsertBuzzer(mCfg->si_buzzer);
    Dev_SiCtrl::bulid()->setBusbarInsertFilter(mCfg->si_filter);
}

void Ad_Resulting::setInsertLineValue()
{
    QString info = tr("设置插接箱回路阈值信息！");
    updatePro(info);
    sBoxData *b = &(mPacket->share_mem_get()->box[mItem->addr-1]);
    sObjCfg *it = &(mCfg->si_cfg);
    int minVal = it->cur.min*it->cur.rate;
    int maxVal = it->cur.max*it->cur.rate;
    int loop = b->loopNum;
    for(int i = 0; i < loop ; i++){
        Dev_SiCtrl::bulid()->setBusbarInsertCur(i+1 ,minVal ,maxVal);
    }
    minVal = it->vol.min*it->vol.rate;
    maxVal = it->vol.max*it->vol.rate;
    for(int i = 0; i < loop ; i++){
        Dev_SiCtrl::bulid()->setBusbarInsertVol(i+1 ,minVal ,maxVal);
    }
    minVal = it->pow.min*it->pow.rate;
    maxVal = it->pow.max*it->pow.rate;
    for(int i = 0; i < loop ; i++){
        Dev_SiCtrl::bulid()->setBusbarInsertPow(i+1 ,minVal ,maxVal);
    }
}

void Ad_Resulting::setInsertEnvValue()
{
    QString info = tr("设置插接箱温度阈值信息！");
    updatePro(info);
    sObjCfg *it = &(mCfg->si_cfg);
    int minVal = it->tem.min*it->tem.rate;
    int maxVal = it->tem.max*it->tem.rate;
    for(int i = 0; i < 4 ; i++){
        Dev_SiCtrl::bulid()->setBusbarInsertTem(i+1 ,minVal ,maxVal);
    }
}

void Ad_Resulting::setInsertZeroLineValue()
{
    QString info = tr("设置插接箱零线电流阈值信息！");
    updatePro(info);
    sObjCfg *it = &(mCfg->si_cfg);
    int minVal = it->zerocur.min*it->zerocur.rate;
    int maxVal = it->zerocur.max*it->zerocur.rate;
    Dev_SiCtrl::bulid()->setBusbarInsertZeroLine(minVal ,maxVal);
}

void Ad_Resulting::setInsertValue()
{
    setInsertInfo();
    setInsertLineValue();
    setInsertEnvValue();
    //setInsertZeroLineValue();///////////////////////////////////////////////////

}

void Ad_Resulting::compareInsertInfo()
{
    QString str = tr("对比插接箱基本信息！");
    updatePro(str);
    sBoxData* b = &(mPacket->share_mem_get()->box[mItem->addr-1]);
    bool ret = false;
    int curValue = b->baudRate;
    int expect = mCfg->si_baud+1;
    if(curValue == expect) ret = true;
    str = tr("插接箱波特率实际值：%1 , 期待值：%2！").arg(changeBaudType(curValue)).arg(changeBaudType(expect));
    updatePro(str,ret);ret = false;

    curValue = b->buzzerStatus;
    expect = mCfg->si_buzzer;
    if(curValue == expect) ret = true;
    str = tr("插接箱蜂鸣器实际值：%1 , 期待值：%2！").arg(curValue?tr("关闭"):tr("开启")).arg(expect?tr("关闭"):tr("开启"));
    updatePro(str,ret);ret = false;

    curValue = b->alarmTime;
    expect = mCfg->si_filter;
    if(curValue == expect) ret = true;
    str = tr("插接箱过滤次数实际值：%1 , 期待值：%2！").arg(curValue).arg(expect);
    updatePro(str,ret);ret = false;

    curValue = b->iOF;
    expect = mCfg->si_iOF;
    if(curValue == expect) ret = true;
    str = tr("插接箱iOF触点实际值：%1 , 期待值：%2！").arg(curValue?tr("有"):tr("无")).arg(expect?tr("有"):tr("无"));
    updatePro(str,ret);ret = false;
}

void Ad_Resulting::compareInsertLineValue()
{
    QString info = tr("对比插接箱电压等电气阈值信息！");
    updatePro(info);
    sObjectData* b = &(mPacket->share_mem_get()->box[mItem->addr-1].data);
    sObjCfg *it = &(mCfg->si_cfg);
    bool ret = false;

    int curValue = 0 , expect = 0;
    QVector<int> ans;
    ans.append(it->cur.min*it->cur.rate);
    ans.append(it->cur.max*it->cur.rate);
    ans.append(it->vol.min*it->vol.rate);
    ans.append(it->vol.max*it->vol.rate);
    ans.append(it->pow.min*it->pow.rate);
    ans.append(it->pow.max*it->pow.rate);

    QString str = tr("最小值");
    QString v = tr("电流");
    QString r = tr("A");
    double rate = it->cur.rate;
    for(int i = 0 ; i < b->lineNum; i++){
        for(int j = 0 ; j < 6 ; j++){
            if( j <= 1){
                curValue = (j % 2 == 0)?b->cur.min[i]:b->cur.max[i];
                str = (j % 2 == 0)?tr("最小值"):tr("最大值");
                v = tr("电流");r = tr("A");rate = it->cur.rate;
            }else if( j <= 3 ){
                curValue = (j % 2 == 0)?b->vol.min[i]:b->vol.max[i];
                str = (j % 2 == 0)?tr("最小值"):tr("最大值");
                v = tr("电压");r = tr("V");rate = it->vol.rate;
            }else{
                curValue = (j % 2 == 0)?b->pow.min[i]:b->pow.max[i];
                str = (j % 2 == 0)?tr("最小值"):tr("最大值");
                v = tr("功率");r = tr("kW");rate = it->pow.rate;
            }
            expect = ans.at(j);
            if(curValue == expect) ret = true;
            info = tr("插接箱L%1 %2 %3实际值：%4 %6, 期待值：%5 %6！")
                    .arg(i+1).arg(v).arg(str).arg(curValue/rate).arg(expect/rate).arg(r);
            updatePro(info,ret);ret = false;
        }
    }
}

void Ad_Resulting::compareInsertEnvValue()
{
    sEnvData *b = &(mPacket->share_mem_get()->box[mItem->addr-1].env);
    sObjCfg *it = &(mCfg->si_cfg);
    int expect = 0 ,curValue = 0;
    double rate = it->tem.rate;
    bool ret = false;
    int idx = 1;
    QString info = tr("对比插接箱温度阈值信息！");
    updatePro(info);
    QVector<int> ans;
    ans.append(it->tem.min*it->tem.rate);
    ans.append(it->tem.max*it->tem.rate);

    QString str = tr("最小值");
    for(int j = 0 ; j < RTU_TH_NUM*2; j++){
        expect = (j % 2==0)?ans.at(0):ans.at(1);
        curValue = (j % 2==0)?b->tem.min[j/2]:b->tem.max[j/2];
        str = (j % 2==0)?tr("最小值"):tr("最大值");
        idx = j/2 + 1;
        if(curValue == expect) ret = true;
        info = tr("插接箱温度 %1 %2实际值：%3 ℃, 期待值：%4 ℃！")
                .arg(idx).arg(str).arg(curValue/rate).arg(expect/rate);
        updatePro(info,ret);ret = false;
    }
}

void Ad_Resulting::compareInsertZeroLineValue()
{
    sRtuUshortUnit *b = &(mPacket->share_mem_get()->box[mItem->addr-1].zeroLineCur);
    sObjCfg *it = &(mCfg->si_cfg);
    int expect = 0 ,curValue = 0;
    double rate = it->zerocur.rate;
    bool ret = false;
    int idx = 1;
    QString info = tr("对比插接箱零线电流阈值信息！");
    updatePro(info);
    QVector<int> ans;
    ans.append(it->zerocur.min*it->zerocur.rate);
    ans.append(it->zerocur.max*it->zerocur.rate);

    QString str = tr("最小值");
    for(int j = 0 ; j < 2; j++){
        expect = (j % 2==0)?ans.at(0):ans.at(1);
        curValue = (j % 2==0)?b->smin:b->smax;
        str = (j % 2==0)?tr("最小值"):tr("最大值");
        idx = j/2 + 1;
        if(curValue == expect) ret = true;
        info = tr("插接箱零线电流 %1 %2实际值：%3 ℃, 期待值：%4 ℃！")
                .arg(idx).arg(str).arg(curValue/rate).arg(expect/rate);
        updatePro(info,ret);ret = false;
    }
}

void Ad_Resulting::compareInsertValue()
{
    compareInsertInfo();
    compareInsertLineValue();
    compareInsertEnvValue();

    //compareInsertZeroLineValue();///////////////////////////////////////////////////

    mPro->step = Test_vert;
}
