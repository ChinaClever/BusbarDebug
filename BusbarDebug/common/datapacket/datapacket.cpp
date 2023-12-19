/*
 *
 *  Created on: 2020年10月1日
 *      Author: Lzy
 */
#include "datapacket.h"
#include "config.h"

sDataPacket::sDataPacket()
{
    dev = new sDevData;
    pro = new sProgress();
}


sDataPacket *sDataPacket::bulid()
{
    static sDataPacket* sington = nullptr;
    if(sington == nullptr)
        sington = new sDataPacket();
    return sington;
}

void sDataPacket::init()
{
    pro->step = 0;
    pro->result = 0;
    pro->pass.clear();
    pro->status.clear();
    dev->dt.sn.clear();
    dev->dt.dev_type.clear();

    pro->result = Test_Info;
    pro->startTime = QTime::currentTime();
    memset(&(dev->data),0,sizeof(sObjData));
}

/**
 * 获取共享内存
 */

sBusData *sDataPacket::share_mem_get()
{
    static sBusData *ptr = nullptr;
    if(!ptr) {
        ptr = new sBusData();
    }

    return ptr;
}

/**
 * 共享内存初始化
 */
void sDataPacket::share_mem_init()
{
    sBusData *shared = share_mem_get();//指向shm
}

/**
 * 把共享内存从当前进程中分离
 */
void sDataPacket::share_mem_free()
{
    sBusData *shm = share_mem_get();
    delete shm;
}

/**
 * 删除共享内存
 */
void sDataPacket::share_mem_del()
{
   share_mem_free();
}


/***
  * 获取共享内存
  */
sBusData *get_share_mem()
{
    static sBusData *shm = NULL;
    if(shm == NULL) {
        shm = sDataPacket::bulid()->share_mem_get();
        sDataPacket::bulid()->share_mem_init();
    }

    return shm;
}
