/**
 *Copyright (c) 2019 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.4.17
 *Email: 2117610943@qq.com
 */

#pragma once
#include "SelectPlantsScene.h"

class MirrorSelectPlantsScene :public SelectPlantsScene
{
public:
    static Scene* createScene();
    CREATE_FUNC(MirrorSelectPlantsScene);

protected:
    void readyTextCallBack(Node* node, const std::string& name, const int& id) override;
};