#ifndef DEV_IPCFG_H
#define DEV_IPCFG_H

#include "dev_iprtu.h"

class Dev_IpCfg : public Dev_Object
{
    Q_OBJECT
    explicit Dev_IpCfg(QObject *parent = nullptr);
public:
    static Dev_IpCfg *bulid(QObject *parent = nullptr);
    void save();

protected:
    QString cfgGroup(){return "busbarStartCfg";}

    void initType();
    void initData();
    void initCfg();

    void writeType();
    void writeData();

};

#endif // DEV_IPCFG_H
