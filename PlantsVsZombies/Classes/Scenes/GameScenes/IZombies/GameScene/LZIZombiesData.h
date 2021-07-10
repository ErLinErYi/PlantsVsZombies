/**
 *Copyright (c) 2019 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.7.04
 *Emal: 2117610943@qq.com
 */

#pragma once
#include "Based/LZUserData.h"

class IZombiesData :public UserData
{
public:
    static IZombiesData* getInstance();
    
    virtual void caveLevelData(char* key) override;
    virtual void openLevelData(char* key) override;
    virtual void openLevelPlantsData(char* key) override;
    virtual void openLevelOtherData(char* key) override;
    virtual void openLevelZombiesData(char* key) override;
    virtual void openLevelBrainData(char* key);

protected:
    IZombiesData();
    ~IZombiesData();

    virtual string getLevelDataFileName() override;
    virtual void caveLevelOtherData(char* key) override;
    virtual void caveLevelBrainData(char* key);
    virtual void replaceScene() override;

protected:
    static IZombiesData* _instance;
};