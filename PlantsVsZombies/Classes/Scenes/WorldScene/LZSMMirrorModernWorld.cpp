/**
 *Copyright (c) 2019 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.4.17
 *Email: 2117610943@qq.com
 */

#include "LZSMMirrorModernWorld.h"
#include "LZSMSelectWorldScene.h"
#include "Based/LZBLevelData.h"
#include "Based/LZBUserData.h"
#include "Based/LZBPlayMusic.h"
#include "Scenes/GameScene/LZSGOpenCaveGameScene.h"
#include "../SelectPlantsScene/LZSSMirrorSelectPlantsScene.h"

bool MirrorModernWorld::_isPopEnter = false;

MirrorModernWorld::MirrorModernWorld()
{
    _worldPosition = UserData::getInstance()->openDoubleUserData(
        _global->userInformation->getGameDifficulty() ? "WORLD_2_POSITION_DIF" : "WORLD_2_POSITION");

    /* 播放音乐 */
    PlayMusic::changeBgMusic("mainmusic2", true);
}

MirrorModernWorld::~MirrorModernWorld()
{
    _isPopEnter = false;
}

Scene* MirrorModernWorld::createScene()
{
    return MirrorModernWorld::create();
}

void MirrorModernWorld::onEnter()
{
    Scene::onEnter();
    if (_isPopEnter)
    {
        _isPopEnter = false;
        auto layer = LayerColor::create(Color4B(0, 0, 0, 255));
        this->addChild(layer);
        layer->runAction(Sequence::create(DelayTime::create(0.1f),
            CallFunc::create([=]()
                {
                    Director::getInstance()->replaceScene(TransitionFade::create(0.5f, MirrorModernWorld::createScene()));
                }), nullptr));
    }
}

void MirrorModernWorld::createScrollView()
{
    auto size = Director::getInstance()->getVisibleSize();
    _scrollView = ui::ScrollView::create();
    _scrollView->setBounceEnabled(true);
    _scrollView->setDirection(ui::ScrollView::Direction::HORIZONTAL);
    _scrollView->setContentSize(size);
    _scrollView->setInnerContainerSize(Size(33100, size.height));
    _scrollView->setPosition(Vec2(0, 0));
    _scrollView->jumpToPercentHorizontal(_worldPosition);
    this->addChild(_scrollView);

    _scrollView->addChild(_parallax);

    addScrollView(1);
}

void MirrorModernWorld::readWorldLevel()
{
    /* 读取该世界关卡数据 */
    if (!_global->userInformation->getUserSelectWorldData().at(1)->isReadWoldInformation)
    {
        OpenLevelData::getInstance()->openLevelsData(
            _global->userInformation->getTextPath().find(
                _global->userInformation->getGameDifficulty() ?
                "GAMEWORLD_1DATAS_DIF" : "GAMEWORLD_1DATAS")->second);
        _global->userInformation->getUserSelectWorldData().at(1)->isReadWoldInformation = true;
    }

    char worldFile[128];
    if (_global->userInformation->getGameDifficulty())
    {
        snprintf(worldFile, 128, _global->userInformation->getSystemDifCaveFileName().c_str(), 2);
    }
    else
    {
        snprintf(worldFile, 128, _global->userInformation->getSystemCaveFileName().c_str(), 2);
    }
    _global->userInformation->getUserSelectWorldData().at(1)->levels =
        UserData::getInstance()->openIntUserData(worldFile);


    if (_global->userInformation->getUserSelectWorldData().at(1)->levels == 0)
    {
        _global->userInformation->getUserSelectWorldData().at(1)->levels = 1;
    }
}

ui::Button* MirrorModernWorld::createButton(Node* node, const std::string& name, const Vec2& position)
{
    auto sprite4 = ui::Button::create(name + ".png", "", "", TextureResType::PLIST);
    sprite4->setPosition(position);
    sprite4->setGlobalZOrder(1);
    sprite4->setScale(2.0f);
    node->addChild(sprite4);

    auto text = ui::Text::create();
    text->setFontName(GAME_FONT_NAME_2);
    text->setFontSize(30);
    text->setString(to_string(++_level));
    text->setGlobalZOrder(1);
    text->setTextColor(Color4B::RED);
    text->setPosition(Vec2(40, 35));
    setLevelVisible(text);
    sprite4->addChild(text);

    if (_global->userInformation->getUserSelectWorldData().at(1)->levels < _level)
    {
        sprite4->setEnabled(false);
    }
    if (_global->userInformation->getUserSelectWorldData().at(1)->levels == _level)
    {
        auto wgfd = Sprite::createWithSpriteFrameName("wgfd.png");
        wgfd->setPosition(Vec2(40, 15));
        wgfd->setScale(0.8f);
        wgfd->setGlobalZOrder(1);
        sprite4->addChild(wgfd, -1);

        wgfd->runAction(RepeatForever::create(Sequence::create(FadeOut::create(0.2f), FadeIn::create(0.2f), nullptr)));
        sprite4->runAction(RepeatForever::create(Sequence::create(TintTo::create(0.1f, Color3B(100, 100, 100)), TintTo::create(0.1f, Color3B(255, 255, 255)), nullptr)));
    }
    if (_global->userInformation->getUserSelectWorldData().at(1)->levels > _level)
    {
        auto wgfd = Sprite::createWithSpriteFrameName("wgfd.png");
        wgfd->setPosition(Vec2(40, 15));
        wgfd->setScale(0.8f);
        wgfd->setGlobalZOrder(1);
        sprite4->addChild(wgfd, -1);

        sprite4->setColor(Color3B(0, 255, 255));
    }

    createButtonListener(sprite4, _level);

    return sprite4;
}

void MirrorModernWorld::createButtonListener(ui::Button* button, const int& ID)
{
    char LevelName[20] = {};
    snprintf(LevelName, 20, "Level_%d", ID);

    button->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type)
        {
            switch (type)
            {
            case ui::Widget::TouchEventType::BEGAN:
                PlayMusic::playMusic("tap");
                break;
            case ui::Widget::TouchEventType::ENDED:

                UserData::getInstance()->caveUserData(
                    _global->userInformation->getGameDifficulty() ? "WORLD_2_POSITION_DIF" : "WORLD_2_POSITION",
                    _scrollView->getScrolledPercentHorizontal()); /* 记录位置 */

                //读取关卡信息
                OpenLevelData::getInstance()->createLevelData(ID, LevelName);
                OpenLevelData::getInstance()->setLevelNumber(ID);

                _global->userInformation->setCurrentPlayLevels(ID);
                _global->userInformation->setCurrentPlayWorldTag(1);
                _global->userInformation->setCurrentPlayWorldName(" - 镜像世界 - ");

                Director::getInstance()->pushScene(TransitionFade::create(1.0f, MirrorSelectPlantsScene::createScene()));

                break;
            }
        });
}

void MirrorModernWorld::setLevelVisible(Node* node)
{
    if (_global->userInformation->getUserSelectWorldData().at(1)->levels < _level - 1)
    {
        node->setVisible(false);
    }
    if (_global->userInformation->getUserSelectWorldData().at(1)->levels == _level)
    {
        node->setOpacity(100);
        node->setCascadeOpacityEnabled(true);
    }
    if (_global->userInformation->getUserSelectWorldData().at(1)->levels == _level - 1)
    {
        node->setOpacity(30);
        node->setCascadeOpacityEnabled(true);
    }
}

void MirrorModernWorld::createGoBack()
{
    auto back = ui::Button::create("back.png", "back1.png", "", TextureResType::PLIST);
    back->setScale(0.7f);
    back->setAnchorPoint(Vec2(0, 1));
    back->setPosition(Vec2(0, 1080));
    this->addChild(back);

    back->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type)
        {
            switch (type)
            {
            case ui::Widget::TouchEventType::BEGAN:
                PlayMusic::playMusic("gravebutton");
                break;
            case ui::Widget::TouchEventType::ENDED:

                UserData::getInstance()->caveUserData(
                    _global->userInformation->getGameDifficulty() ? "WORLD_2_POSITION_DIF" : "WORLD_2_POSITION",
                    _scrollView->getScrolledPercentHorizontal()); /* 记录位置 */

                _global->userInformation->setMainToWorld(false);
                Director::getInstance()->replaceScene(TransitionFade::create(0.5f, SelectWorldScene::createScene()));
                break;
            }
        });
}
