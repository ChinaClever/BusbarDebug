#ifndef DEV_SICFG_H
#define DEV_SICFG_H

#include "dev_object.h"

class Dev_SiCfg : public Dev_Object
{
    Q_OBJECT
    explicit Dev_SiCfg(QObject *parent = nullptr);
public:
    static Dev_SiCfg *bulid(QObject *parent = nullptr);

    void save();

protected:
    void initType();
    void initData();
    void initCfg();

    void writeType();
    void writeData();
};

#endif // DEV_SICFG_H
