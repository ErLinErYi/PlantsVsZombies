﻿/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.6
 *Email: 2117610943@qq.com
 */

#pragma once
#include "../LZPlants.h"
#include "Scenes/GameScenes/Adventure/SelectPlantsScene/LZASPSSpriteLayer.h"

class Zombies;
class Bullet;

class EmissionPlants :public Plants
{
CC_CONSTRUCTOR_ACCESS:
    EmissionPlants();
    ~EmissionPlants();

protected:
    virtual void createBullet() = 0;
    virtual void plantAttack(Zombies* zombie) = 0;
    virtual void createListener(std::string animationName, std::string actionName = "shoot", std::string musicName = "") override;
	virtual void cavePlantInformation(rapidjson::Value& object, rapidjson::Document::AllocatorType& allocator) override;
	virtual void readPlantInforamtion(rapidjson::Document* levelDataDocument, char* key, int i) override;

protected:
    bool _isChanged;           // 是否改变为攻击形态
    bool _isHaveZombies;       // 是否有僵尸
    bool _isReadFileData;      // 是否读取文件数据
    Bullet* _bulletAnimation;  // 子弹动画
};