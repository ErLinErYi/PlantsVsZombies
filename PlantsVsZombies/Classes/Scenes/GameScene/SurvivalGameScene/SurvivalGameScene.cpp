/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.5.08
 *Email: 2117610943@qq.com
 */

#include "SurvivalGameScene.h"
#include "SGSSurvivalControlLayer.h"
#include "../GSData.h"

Scene* SurvivalGameScene::createScene()
{
    return SurvivalGameScene::create();
}

void SurvivalGameScene::controlLayer()
{
    controlLayerInformation = SGSSurvivalControlLayer::create();
    controlLayerInformation->addLayer(this, 1, "SurvivalControlLayer");
}
