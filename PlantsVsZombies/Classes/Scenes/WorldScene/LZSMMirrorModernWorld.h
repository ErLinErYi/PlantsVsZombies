/**
 *Copyright (c) 2019 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.4.17
 *Email: 2117610943@qq.com
 */

#pragma once
#include "LZSMModernWorld.h"

class MirrorModernWorld :public ModernWorld
{
public:
    CREATE_FUNC(MirrorModernWorld);
    static Scene* createScene();

CC_CONSTRUCTOR_ACCESS:
    MirrorModernWorld();
    ~MirrorModernWorld();
    virtual void onEnter() override;

protected:
    virtual void createScrollView() override;
    virtual void readWorldLevel() override;
    virtual ui::Button* createButton(Node* node, const std::string& name, const Vec2& position) override;
    virtual void createButtonListener(ui::Button* button, const int& ID) override;
    virtual void setLevelVisible(Node* node) override;
    virtual void createGoBack() override;

private:
    static bool _isPopEnter;
};