/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.4.15
 *Email: 2117610943@qq.com
 */

#pragma once
#include "cocos2d.h"
#include "spine/spine-cocos2dx.h"
#include "GlobalVariable.h"

using namespace cocos2d;
using namespace spine;

class Coin
{
public:
    void createCoin();
    void setPosition(const Vec2& position);
    void setEnable(bool isUsed);
    void setCoinLocalZOrder(const int loaclZOrder);
    SkeletonAnimation* getCoin();
    bool getEnable() const;
    void releaseCoin() const;
    static void coinRecoveryAction(Coin* coin);
    static void deleteCoin(list<Coin*>::iterator& coin);

CC_CONSTRUCTOR_ACCESS:
    Coin(Node* node = nullptr);
    ~Coin() {}

private:
    void coinInit();

private:
    bool _isEnable;
    bool _canDelete;
    int _loaclZOrder;
    Vec2 _position;
    SkeletonAnimation* _coin;
    Node* _node;
    Global* _global;
};