/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.4.15
 *Email: 2117610943@qq.com
 */

#include "LZCoin.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSData.h"
#include "Scenes/GameScenes/Adventure/GameScene/LZAGSInformationLayer.h"
#include "Scenes/GameScenes/BigMap/GameScene/LZBigMapGameScene.h"
#include "Based/LZPlayMusic.h"

Coin::Coin(Node* node):
    _node(node)
,   _loaclZOrder(0)
,   _coin(nullptr)
,   _isEnable(true)
,   _global(Global::getInstance())
{
}

void Coin::createCoin()
{
    coinInit();
}

void Coin::setPosition(const Vec2& position)
{
    _position = position;
}

void Coin::setEnable(bool isUsed)
{
    _isEnable = isUsed;
}

void Coin::setCoinLocalZOrder(const int loaclZOrder)
{
    _loaclZOrder = loaclZOrder;
}

SkeletonAnimation* Coin::getCoin()
{
    return _coin;
}

bool Coin::getEnable() const
{
    return _isEnable;
}

void Coin::releaseFunction()
{
    _coin->removeFromParent();
    _coin = nullptr;
}

void Coin::coinRecoveryAction(Coin* coin)
{
    auto temporary = coin->getCoin();
    auto actionMoveTo = EaseSineOut::create(MoveTo::create(0.5f, Vec2(1655, 38)));
    auto actionScaleFade = Spawn::create(ScaleTo::create(0.2f, 0.3f), FadeOut::create(0.2f), nullptr);
    auto actionCallFunc = CallFunc::create([=]()
        {
            PlayMusic::playMusic("moneyfalls");
            Global::getInstance()->userInformation->setCoinNumbers(Global::getInstance()->userInformation->getCoinNumbers() + 1);
            informationLayerInformation->updateCoinNumbers();
        });
    auto actionCallFunc2 = CallFunc::create([temporary]()
        {
            temporary->setVisible(false);
        });

    if (BigMapGameScene::scrollView)
    {
        auto offset = BigMapGameScene::scrollView->getContentOffset();
        auto point = temporary->getPosition();

        temporary->retain();
        temporary->removeFromParent();
        informationLayerInformation->addChild(temporary);
        temporary->setPosition(Vec2(point.x - fabs(offset.x), point.y - fabs(offset.y)));
    }

    coin->setEnable(false);
    temporary->stopAllActions();
    temporary->runAction(Sequence::create(actionMoveTo, actionScaleFade, actionCallFunc, DelayTime::create(5.0f), actionCallFunc2, nullptr));

    PlayMusic::playMusic("coin");
}

void Coin::deleteCoin(list<Coin*>::iterator& coin)
{
    if (!(*coin)->getCoin()->isVisible())
    {
        (*coin)->releaseFunction();
        delete* coin;
        *coin = nullptr;
        CoinsGroup.erase(coin++);
    }
    else
    {
        ++coin;
    }
}

void Coin::coinInit()
{
    _coin = SkeletonAnimation::createWithData( _global->userInformation->getAnimationData().find("coin")->second);
    _coin->addAnimation(0, "rotate", true);
    _coin->setScale(0.05f);
    _coin->setLocalZOrder(_loaclZOrder);
    _coin->setPosition(_position);
    _coin->update(0);
    _node->addChild(_coin);
    _coin->runAction(Spawn::create(FadeIn::create(0.5f), ScaleTo::create(0.5f, 0.8f),
        JumpBy::create(0.5f, Vec2(150 - rand() % 300, 100 - rand() % 100), rand() % 100 + 200, 1),
        Sequence::create(DelayTime::create(0.25f), nullptr), nullptr));
    _coin->runAction(Sequence::create(DelayTime::create(15.f), CallFunc::create([=]() {_coin->setVisible(false); }), nullptr));

    PlayMusic::playMusic("moneyfalls");
}
