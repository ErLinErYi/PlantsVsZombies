/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.4.17
 *Email: 2117610943@qq.com
 */

#pragma once
#include "Scenes/GameScenes/Adventure/WorldScene/LZModernWorld.h"

class BigMapWorld :public ModernWorld
{
public:
    CREATE_FUNC(BigMapWorld);
   
CC_CONSTRUCTOR_ACCESS:
    BigMapWorld();
    ~BigMapWorld();
    virtual void onEnter() override;

protected:
    virtual void createScrollView() override;
    virtual void readWorldLevel() override;
    virtual ui::Button* createButton(Node* node, const std::string& name, const Vec2& position) override;
    virtual void createButtonListener(ui::Button* button, const int& ID) override;
    virtual void setLevelVisible(Node* node) override;
    virtual void createGoBack() override;
    virtual void createMouseListener() override;
    virtual void createBackground() override;
    virtual void addScrollView(const int id) override;
    virtual void showLevels() override;
    virtual Color4B getWorldColor(int id) override;
};