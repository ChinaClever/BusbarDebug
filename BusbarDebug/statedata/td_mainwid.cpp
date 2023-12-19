/*
 *
 *  Created on: 2021年1月1日
 *      Author: Lzy
 */
#include "td_mainwid.h"
#include "ui_td_mainwid.h"

Td_MainWid::Td_MainWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Td_MainWid)
{
    ui->setupUi(this);
    QGridLayout *gridLayout = new QGridLayout(parent);
    gridLayout->setContentsMargins(0, 0, 0, 0);
    gridLayout->addWidget(this);
    initWid();
}

Td_MainWid::~Td_MainWid()
{
    delete ui;
}

void Td_MainWid::initWid()
{
    mTableWid = new Td_DataTabWid(ui->tabWidget);
    ui->tabWidget->addTab(mTableWid, tr("校准数据列表"));

    mLineTabWid = new Td_LineTabWid(ui->tabWidget);
    ui->tabWidget->addTab(mLineTabWid, tr("相数据列表"));

    mThTabWid1 = new Td_ThresholdTabWid(0 ,ui->tabWidget);
    ui->tabWidget->addTab(mThTabWid1, tr("电流报警阈值"));

    mThTabWid2 = new Td_ThresholdTabWid(1 ,ui->tabWidget);
    ui->tabWidget->addTab(mThTabWid2, tr("电压报警阈值"));

    mThTabWid3 = new Td_ThresholdTabWid(2 ,ui->tabWidget);
    ui->tabWidget->addTab(mThTabWid3, tr("线电压报警阈值"));

    mThTabWid4 = new Td_ThresholdTabWid(3 ,ui->tabWidget);
    ui->tabWidget->addTab(mThTabWid4, tr("功率报警阈值"));

    mThTabWid5 = new Td_ThresholdTabWid(4 ,ui->tabWidget);
    ui->tabWidget->addTab(mThTabWid5, tr("其他报警阈值"));

    mEnvTabWid = new Td_EnvTabWid(ui->tabWidget);
    ui->tabWidget->addTab(mEnvTabWid, tr("传感器状态"));
}
