/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.11
 *Email: 2117610943@qq.com
 */

#include "GSData.h"
#include "GSBackgroundLayer.h"
#include "GSInformationLayer.h"
#include "GSButtonLayer.h"
#include "GSControlLayer.h"
#include "GSAnimationLayer.h"

#include "Zombies/Zombies.h"

GSBackgroundLayer* backgroundLayerInformation;   // 背景层实例化
GSInformationLayer* informationLayerInformation; // 信息层实例化
GSButtonLayer* buttonLayerInformation;           // 按钮层实例化
GSControlLayer* controlLayerInformation;         // 控制层实例化
GSAnimationLayer* animationLayerInformation;     // 动画层示例化

multimap<int, Plants*> PlantsGroup;
list<Zombies*> ZombiesGroup;
list<Bullet*> BulletGroup;
list<Sun*> SunsGroup;
list<Car*> CarsGroup;
list<Coin*> CoinsGroup;
