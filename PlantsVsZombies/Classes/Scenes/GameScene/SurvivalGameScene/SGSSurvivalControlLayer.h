/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.5.08
 *Email: 2117610943@qq.com
 */

#pragma once
#include "../GSControlLayer.h"

class SGSSurvivalControlLayer :public GSControlLayer
{
public:
    CREATE_FUNC(SGSSurvivalControlLayer);
    void addLayer(Node* node, const int order, const string& name) { node->addChild(this, order, name); }
    void  setZombiesAppearFrequency(const unsigned int frequency);
    unsigned int getZombiesAppearFrequency() const;

CC_CONSTRUCTOR_ACCESS:
    //SGSSurvivalControlLayer();

protected:
    virtual void judgeLevelIsFinished()override;                                                /* 判断关卡是否结束 */
    virtual void createZombies()override;                                                       /* 创建僵尸 */
    virtual bool controlRefurbishZombies()override;                                             /* 控制僵尸刷新 */
    virtual void controlRefurbishMusicAndText()override;                                        /* 控制音乐与文字的刷新*/

private:
    int getZombiesNumbers();

private:
    static unsigned int _frequencyNumbers;
};