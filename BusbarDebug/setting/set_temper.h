#ifndef SET_TEMPER_H
#define SET_TEMPER_H

#include <QWidget>
#include "common.h"
#include "dev_ipcfg.h"

namespace Ui {
class Set_temper;
}

class Set_temper : public QWidget
{
    Q_OBJECT

public:
    explicit Set_temper(QWidget *parent = nullptr);
    ~Set_temper();

signals:
    void errSig();
public slots:
    void initFunSlot();
    void enabledSlot(bool en);
protected:
    void initType();
    void updateType();
    bool dataSave();
    bool inputCheck();
private:
    Ui::Set_temper *ui;

    sCfgItem *mItem;
    Dev_SiCfg *mObj;
    sDevData *mDev;
};

#endif // SET_TEMPER_H
