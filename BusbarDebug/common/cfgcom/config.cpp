/*
 * 配置文件公共基类
 *
 *  Created on: 2018年1月1日
 *      Author: Lzy
 */
#include "config.h"

Cfg::Cfg()
{
    mCfg = CfgCom::bulid();
    item = new sCfgItem();
    item->com = nullptr;
    item->source = nullptr;
    item->control = nullptr;
    item->vol = 200;

    initMac();
    initCnt();
    initCfgDev();
    initErrData();
    initCurrentNum();
}

Cfg *Cfg::bulid()
{
    static Cfg* sington = nullptr;
    if(sington == nullptr)
        sington = new Cfg();
    return sington;
}


void Cfg::setDate()
{
    QString value = QDate::currentDate().toString("yyyy-MM-dd");
    write("date", value, "Date");
}

bool Cfg::getDate()
{
    bool ret = false;

    QString str = read("date","","Date").toString();
    if(!str.isEmpty()) {
        QDate date = QDate::fromString(str, "yyyy-MM-dd");
        if(QDate::currentDate() > date) {
            ret = true;
        }
    }

    return ret;
}

void Cfg::setCurrentNum()
{
    setDate();
    write("num", item->currentNum, "Date");
}

void Cfg::initCurrentNum()
{
    bool ret = getDate();
    if(ret) {
        item->currentNum = 0;
        setCurrentNum();
    } else {
        int value = read("num", 0,"Date").toInt();
        item->currentNum = value;
    }
}


void Cfg::initCfgDev()
{
    item->addr = read("addr", 1,"BusbarSys").toInt();
    item->modeId = read("modeId", 1,"BusbarSys").toInt();
    item->user = read("user", "", "BusbarUser").toString();
    //item->aiMode = read("ai_mode", 0, "Sys").toInt();
    if(item->modeId) item->modeId = 1;
}

void Cfg::writeCfgDev()
{
    writeCnt();
    write("addr", item->addr, "BusbarSys");
    write("modeId", item->modeId, "BusbarSys");
    //write("ai_mode", item->aiMode, "Sys");
    write("user", item->user, "BusbarUser");
}

void Cfg::initCnt()
{
    item->cnt.cnt = read("cnt", 0, "BusbarCount").toInt();
    item->cnt.all = read("all", 0, "BusbarCount").toInt();
    item->cnt.ok = read("ok", 0, "BusbarCount").toInt();
    item->cnt.err = read("err", 0, "BusbarCount").toInt();
}

void Cfg::writeCnt()
{
    write("cnt", item->cnt.cnt, "BusbarCount");
    write("all", item->cnt.all, "BusbarCount");
    write("ok", item->cnt.ok, "BusbarCount");
    write("err", item->cnt.err, "BusbarCount");
    write("user", item->user, "BusbarUser");
    wirteMac();
}

void Cfg::wirteMac()
{
    write("mac", item->mac, "Mac");
    write("cnt", item->cntMac, "Mac");
    write("start", item->startMac, "Mac");
    write("end", item->endMac, "Mac");
}

void Cfg::initMac()
{
    QString str = "2C:26:5F:38:00:00";
    item->mac = read("mac", str, "Mac").toString();
    item->cntMac = read("cnt", 5*1000, "Mac").toInt();
    item->startMac = read("start", str, "Mac").toString();
    item->endMac = read("end", str, "Mac").toString();
}


void Cfg::initErrData()
{
    item->volErr = read("vol", 1,"BusbarErr").toInt();
    item->curErr = read("cur", 1,"BusbarErr").toInt();
    item->powErr = read("pow", 45,"BusbarErr").toInt();
}

void Cfg::writeErrData()
{
    write("vol", item->volErr, "BusbarErr");
    write("cur", item->curErr, "BusbarErr");
    write("pow", item->powErr, "BusbarErr");
}

/**
 * @brief 获取串口名称
 * @return 串口名
 */
QString Cfg::getSerialName(const QString &key)
{
    return mCfg->read(key, "", "Serial").toString();
}

/**
 * @brief 设置串口名
 * @param name
 */
void Cfg::setSerialName(const QString &key, const QString &v)
{
    mCfg->write(key, v, "Serial");
}

QString Cfg::getSerialBr(const QString &com)
{
    QString key = QString("BR_%1").arg(com);
    return mCfg->read(key, "", "Serial").toString();
}

void Cfg::setSerialBr(const QString &com, const QString &br)
{
    QString key = QString("BR_%1").arg(com);
    mCfg->write(key, br, "Serial");
}


/**
 * @brief 根据名称获取配置文件数值
 * @return 对应的配置文件数值
 */
QVariant Cfg::read(const QString &key, const QVariant &v, const QString &g)
{
    return mCfg->read(key, v, g);
}

/**
 * @brief 设置对应名称的配置文件数值
 * @param value
 * @param name
 */
void Cfg::write(const QString &key, const QVariant& v, const QString &g)
{
    mCfg->write(key, v, g);
}

/**
 * @brief 获取当前用户名称
 * @return 用户名
 */
QString Cfg::getLoginName()
{
    return mCfg->read("name", "admin", "Login").toString();
}

/**
 * @brief 设置当前用户名
 * @param name
 */
void Cfg::setLoginName(const QString &name)
{
    mCfg->write("name", name, "Login");
}
