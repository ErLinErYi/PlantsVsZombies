/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.4.17
 *Email: 2117610943@qq.com
 */

#include "LZBigMapWorld.h"
#include "Based/LZLevelData.h"
#include "Based/LZUserData.h"
#include "Based/LZPlayMusic.h"
#include "Based/LZMouseEventControl.h"
#include "Scenes/SelectWorldScene/LZSelectWorldScene.h"
#include "Scenes/GameScenes/BigMap/GameScene/LZBMGSOpenCaveGameScene.h"
#include "Scenes/GameScenes/BigMap/SelectPlantsScene/LZBMSelectPlantsScene.h"

BigMapWorld::BigMapWorld()
{
    PlayMusic::changeBgMusic("mainmusic2", true);

    _global->userInformation->setCurrentPlayWorldTag(1);
    _global->userInformation->setCurrentPlayWorldName(_global->userInformation->getGameText().find("- 大地图模式 -")->second->text);
    _worldPosition = UserData::getInstance()->openDoubleUserData(const_cast<char*>(getScrollViewPositionString().c_str()));

    _isPopEnter = false;
}

BigMapWorld::~BigMapWorld()
{
    _isPopEnter = false;
}

void BigMapWorld::onEnter()
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
                    Director::getInstance()->replaceScene(TransitionFade::create(0.5f, BigMapWorld::create()));
                }), nullptr));
    }
}

void BigMapWorld::createScrollView()
{
    auto size = Director::getInstance()->getVisibleSize();
    _scrollView = ui::ScrollView::create();
    _scrollView->setBounceEnabled(true);
    _scrollView->setDirection(ui::ScrollView::Direction::HORIZONTAL);
    _scrollView->setContentSize(size);
    _scrollView->setInnerContainerSize(Size(1920 + 600 *
        _global->userInformation->getUserSelectWorldData().at(1)->levels, size.height));//1920+600*52
    _scrollView->setPosition(Vec2(0, 0));
    _scrollView->jumpToPercentHorizontal(_worldPosition);
    this->addChild(_scrollView);

    _scrollView->addChild(_parallax);

    addScrollView(1);
}

void BigMapWorld::readWorldLevel()
{
    /* 读取该世界关卡数据 */
    if (!_global->userInformation->getUserSelectWorldData().at(1)->isReadWoldInformation)
    {
        OpenLevelData::getInstance()->openLevelsData(
            _global->userInformation->getTextPath().find(
                _global->userInformation->getGameDifficulty() ?
                "GameDataModernWorldDIF" : "GameDataModernWorld")->second);
        _global->userInformation->getUserSelectWorldData().at(1)->isReadWoldInformation = true;
    }

    string worldFile;
    if (_global->userInformation->getGameDifficulty())
        worldFile = StringUtils::format(_global->userInformation->getSystemDifCaveFileName().c_str(), 2);
    else
        worldFile = StringUtils::format(_global->userInformation->getSystemCaveFileName().c_str(), 2);
    _global->userInformation->getUserSelectWorldData().at(1)->levels =
        UserData::getInstance()->openIntUserData(const_cast<char*>(worldFile.c_str()));


    if (_global->userInformation->getUserSelectWorldData().at(1)->levels == 0)
    {
        _global->userInformation->getUserSelectWorldData().at(1)->levels = 1;
    }
}

ui::Button* BigMapWorld::createButton(Node* node, const std::string& name, const Vec2& position)
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

void BigMapWorld::createButtonListener(ui::Button* button, const int& ID)
{
    button->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type)
        {
            switch (type)
            {
            case ui::Widget::TouchEventType::BEGAN:
                PlayMusic::playMusic("tap");
                break;
            case ui::Widget::TouchEventType::ENDED:

                //读取关卡信息
                OpenLevelData::getInstance()->createLevelData(ID, StringUtils::format("Level_%d", ID).c_str());
                OpenLevelData::getInstance()->setLevelNumber(ID);

                _global->userInformation->setCurrentPlayLevels(ID);

                UserData::getInstance()->caveUserData(const_cast<char*>(getScrollViewPositionString().c_str()), 
                    _scrollView->getScrolledPercentHorizontal()); /* 记录位置 */
                UserData::getInstance()->createNewLevelDataDocument();
                if (UserData::getInstance()->isHaveLevelData(_global->userInformation->getCurrentCaveFileLevelWorldName()))
                {
                    auto layer = LayerColor::create(Color4B(0, 0, 0, 0));
                    layer->setGlobalZOrder(2);
                    this->addChild(layer);
                    layer->runAction(Sequence::create(FadeIn::create(0.5f),
                        CallFunc::create([=]()
                            {
                                layer->removeFromParent();
                                Director::getInstance()->pushScene(BMOpenCaveGameScene::create());
                            }), nullptr));
                }
                else
                    Director::getInstance()->pushScene(TransitionFade::create(1.0f, BMSelectPlantsScene::create()));

                break;
            }
        });
}

void BigMapWorld::setLevelVisible(Node* node)
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

void BigMapWorld::createGoBack()
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

                UserData::getInstance()->caveUserData(const_cast<char*>(getScrollViewPositionString().c_str()),
                    _scrollView->getScrolledPercentHorizontal()); /* 记录位置 */

                _global->userInformation->setMainToWorld(false);
                Director::getInstance()->replaceScene(TransitionFade::create(0.5f, SelectWorldScene::createScene()));
                break;
            }
        });
}

void BigMapWorld::createMouseListener()
{
    auto mouse = EventListenerMouse::create();
    mouse->onMouseScroll = [&](Event* event)
    {
        auto mouseEvent = static_cast<EventMouse*>(event);
        float movex = mouseEvent->getScrollY() * 99360.f / (1920 + 600 * _global->userInformation->getUserSelectWorldData().at(1)->levels);

        MouseEventControl::mouseScrollControlListener(_scrollView, movex, ui::ScrollView::Direction::HORIZONTAL);
    };
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(mouse, _scrollView);
}
