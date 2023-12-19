#ifndef CTRL_ZPDUTHREAD_H
#define CTRL_ZPDUTHREAD_H

#include "ad_modbus.h"

class Ctrl_ZpduThread : public QThread
{
    Q_OBJECT
    explicit Ctrl_ZpduThread(QObject *parent = nullptr);
public:
    static Ctrl_ZpduThread *bulid(QObject *parent = nullptr);
    void openAllSwitch();
    void openOnlySwitch(int id);
    bool delay(int s);

protected:
    void funSwitch(uchar *on, uchar *off);
    void setBitControl(int id, uchar *buf);
    void closeOtherSwitch(int id);
    void openOutputSwitch(int id);

    sCfgItem *mItem;
    Ad_Modbus *mModbus;
};

#endif // CTRL_ZPDUTHREAD_H
