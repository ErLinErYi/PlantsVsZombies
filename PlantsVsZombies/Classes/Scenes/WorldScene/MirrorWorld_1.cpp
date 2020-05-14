/**
 *Copyright (c) 2019 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.4.17
 *Email: 2117610943@qq.com
 */

#include "MirrorWorld_1.h"
#include "SelectWorldScene.h"
#include "Based/LevelData.h"
#include "Based/UserData.h"
#include "Based/PlayMusic.h"
#include "Scenes/GameScene/OpenCaveGameScene.h"
#include "../SelectPlantsScene/MirrorSelectPlantsScene.h"

bool MirrorWorld_1::_isPopEnter = false;

MirrorWorld_1::MirrorWorld_1()
{
    _worldPosition = UserData::getInstance()->openDoubleUserData(
        _global->userInformation->getGameDifficulty() ? "WORLD_2_POSITION_DIF" : "WORLD_2_POSITION");

    /* 播放音乐 */
    PlayMusic::changeBgMusic("mainmusic2", true);
}

MirrorWorld_1::~MirrorWorld_1()
{
    _isPopEnter = false;
}

Scene* MirrorWorld_1::createScene()
{
    return MirrorWorld_1::create();
}

void MirrorWorld_1::onEnter()
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
                    Director::getInstance()->replaceScene(TransitionFade::create(0.5f, MirrorWorld_1::createScene()));
                }), nullptr));
    }
}

void MirrorWorld_1::createScrollView()
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

void MirrorWorld_1::readWorldLevel()
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
        snprintf(worldFile, 128, _global->userInformation->getSystemDifCaveFileName(
            _global->userInformation->getUserCaveFileNumber()).c_str(), 2);
    }
    else
    {
        snprintf(worldFile, 128, _global->userInformation->getSystemCaveFileName(
            _global->userInformation->getUserCaveFileNumber()).c_str(), 2);
    }
    _global->userInformation->getUserSelectWorldData().at(1)->levels =
        UserData::getInstance()->openIntUserData(worldFile);


    if (_global->userInformation->getUserSelectWorldData().at(1)->levels == 0)
    {
        _global->userInformation->getUserSelectWorldData().at(1)->levels = 1;
    }
}

ui::Button* MirrorWorld_1::createButton(Node* node, const std::string& name, const Vec2& position)
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

void MirrorWorld_1::createButtonListener(ui::Button* button, const int& ID)
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

void MirrorWorld_1::setLevelVisible(Node* node)
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

void MirrorWorld_1::createGoBack()
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
