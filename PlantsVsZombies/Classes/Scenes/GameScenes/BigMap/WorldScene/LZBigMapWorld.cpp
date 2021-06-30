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

void BigMapWorld::createBackground()
{
    auto layerColor = LayerColor::create(Color4B(30, 180, 190, 200));
    this->addChild(layerColor);

    auto rotate = Sprite::createWithSpriteFrameName("RunBackground.png");
    rotate->setScale(7.0f);
    rotate->setPosition(_backgroundSize / 2.0f);
    this->addChild(rotate);
    rotate->runAction(RepeatForever::create(RotateBy::create(1.0f, -10)));

    auto World1_34 = Sprite::createWithSpriteFrameName("World2_3.png");
    World1_34->setScale(1.5f);
    World1_34->setPosition(_backgroundSize / 2.0f);
    this->addChild(World1_34);

    auto World1_33 = Sprite::createWithSpriteFrameName("World1_33.png");
    World1_33->setPosition(Vec2(400, 300));
    World1_33->setScale(0.5f);
    World1_34->addChild(World1_33);

    _parallax = ParallaxNode::create();

    auto WorldBackgroundBlack = Sprite::createWithSpriteFrameName("WorldBackgroundBlack.png");
    auto WorldBackgroundBlack1 = Sprite::createWithSpriteFrameName("WorldBackgroundBlack.png");
    WorldBackgroundBlack->setAnchorPoint(Vec2(0, 1));
    WorldBackgroundBlack1->setAnchorPoint(Vec2(0, 0));
    WorldBackgroundBlack1->setFlippedY(true);
    _parallax->addChild(WorldBackgroundBlack, 2, Vec2(0, 0), Vec2(0, 1140));
    _parallax->addChild(WorldBackgroundBlack1, 2, Vec2(0, 0), Vec2(0, -60));

    for (int i = 0; i < 8; i++)
    {
        switch (rand() % 2)
        {
        case 0:
        {
            auto World1_28 = Sprite::createWithSpriteFrameName("World1_28.png");
            World1_28->setScale(2.0f);
            _parallax->addChild(World1_28, 1, Vec2(0.1f, 0), Vec2(700 * i, rand() % 400 + 300));
        }
        break;
        case 1:
        {
            auto World1_29 = Sprite::createWithSpriteFrameName("World1_29.png");
            World1_29->setScale(2.0f);
            _parallax->addChild(World1_29, 1, Vec2(0.1f, 0), Vec2(700 * i, rand() % 400 + 300));
        }
        break;
        }
    }
}

void BigMapWorld::createScrollView()
{
    const auto size = Director::getInstance()->getVisibleSize();
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
                "GameDataBigMapWorldDif" : "GameDataBigMapWorld")->second);
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

void BigMapWorld::addScrollView(const int id)
{
    /* 创建背景物品 */
    const string name[] = { {"World2_2"},{"World2_3"},{"World2_4"}};
    const string name1[] = { {"World2_5"},{"World2_17"} };

    for (int i = 0; i < 10; i++)
    {
        const auto world = createSprite(name[i % 3], Vec2(500 + 1000 * i, i % 2 == 0 ? 850 : 350), rand() % 4 / 10.0f + 1.5f, 1, 0.2f, true);
        world->setScale(0.7f);
        world->runAction(RepeatForever::create(RotateBy::create(10, 30)));
    }

    for (int i = 0; i < 8; i++)
    {
        const auto world = createSprite(name1[i % 2], Vec2(500 + 2000 * i, 180), rand() % 3 / 10.0f + 1.5f, 3, 1.f, true);
        world->setAnchorPoint(Vec2(0.5f, 0.5f));
        world->setScale(0.7f);
        if (i % 2 != 0)
        {
            world->runAction(RepeatForever::create(RotateBy::create(25, 30)));
        }
    }

    /* 创建线条 */
    auto draw = DrawNode::create();
    draw->setOpacity(0);
    Vec2 BeginPoint[53] =
    {
        Vec2(575, 450) ,Vec2(1050, 550) ,Vec2(1370, 640) ,Vec2(1650, 490) ,Vec2(2050, 420) ,Vec2(2300, 370) ,Vec2(2650, 520) ,Vec2(2940,700),
        Vec2(3230, 560) ,Vec2(3550, 460) ,Vec2(3920, 540) ,Vec2(4250, 710) ,Vec2(4620, 590) ,Vec2(4960, 540) ,Vec2(5300, 400) ,Vec2(5620,570),
        Vec2(5950, 665) ,Vec2(6300, 800) ,Vec2(6600, 590) ,Vec2(6900, 490) ,Vec2(7200, 440) ,Vec2(7500, 520) ,Vec2(7810, 680) ,Vec2(8100, 760),
        Vec2(8400, 725) ,Vec2(8700, 600) ,Vec2(9000, 510) ,Vec2(9300, 450) ,Vec2(9580, 540) ,Vec2(9950, 670) ,Vec2(10300, 750) ,Vec2(10590, 690),
        Vec2(10900, 620) ,Vec2(11200, 510) ,Vec2(11500, 400) ,Vec2(11900, 500) ,Vec2(12100, 390) ,Vec2(12300, 500) ,Vec2(12700, 430) ,Vec2(13000, 520),
        Vec2(13300, 620) ,Vec2(13600, 730) ,Vec2(13900, 660) ,Vec2(14200, 570) ,Vec2(14500, 470) ,Vec2(14800, 370) ,Vec2(15130, 540) ,Vec2(15500, 680),
        Vec2(15800, 730) ,Vec2(16200, 680) ,Vec2(16520, 480) ,Vec2(16780, 390),Vec2(16900,480)
    };
    Vec2 EndPoint[53] =
    {
        Vec2(1000, 560) ,Vec2(1300, 640) ,Vec2(1600, 500) ,Vec2(2000, 420) ,Vec2(2250, 380) ,Vec2(2600, 510) ,Vec2(2900, 670) ,Vec2(3190,560),
        Vec2(3530, 460) ,Vec2(3900, 540) ,Vec2(4250, 710) ,Vec2(4600, 590) ,Vec2(4960, 540) ,Vec2(5300, 400) ,Vec2(5600, 570) ,Vec2(5950,670),
        Vec2(6300, 800) ,Vec2(6600, 590) ,Vec2(6900, 490) ,Vec2(7200, 440) ,Vec2(7500, 520) ,Vec2(7810, 680) ,Vec2(8070, 760) ,Vec2(8400, 720),
        Vec2(8700, 600) ,Vec2(9000, 510) ,Vec2(9300, 450) ,Vec2(9580, 540) ,Vec2(9950, 670) ,Vec2(10300, 750) ,Vec2(10590, 690) ,Vec2(10900, 620),
        Vec2(11200, 510) ,Vec2(11500, 400) ,Vec2(11900, 500) ,Vec2(12100, 390) ,Vec2(12300, 500) ,Vec2(12700, 430) ,Vec2(13000, 520) ,Vec2(13300, 620),
        Vec2(13600, 730) ,Vec2(13900, 660) ,Vec2(14200, 570) ,Vec2(14500, 470) ,Vec2(14800, 370) ,Vec2(15130, 540) ,Vec2(15500, 680) ,Vec2(15800, 730),
        Vec2(16200, 680) ,Vec2(16520, 480) ,Vec2(16700, 380) ,Vec2(16900, 450),Vec2(16950,480)
    };
    for (int i = 0; i < _global->userInformation->getUserSelectWorldData().at(id)->levels; ++i)
    {
        draw->drawSegment(BeginPoint[i], EndPoint[i], 4, Color4F(0, 1, 1, 0.6f));
        draw->runAction(RepeatForever::create(Sequence::create(MoveBy::create(0.05f, Vec2(0.1f, 0.1f)), MoveBy::create(0.05f, Vec2(-0.1f, -0.1f)), nullptr)));
        auto LineAction = Sprite::createWithSpriteFrameName("LineAction.png");
        LineAction->setPosition(EndPoint[i]);
        draw->addChild(LineAction);
        LineAction->runAction(RepeatForever::create(Sequence::create(MoveTo::create(1.0f, EndPoint[i]),
            CallFunc::create([=]()
                {
                    LineAction->setPosition(BeginPoint[i]);
                }), nullptr)));
    }
    draw->runAction(RepeatForever::create(Sequence::create(FadeIn::create(0.7f), FadeOut::create(0.7f), nullptr)));
    _parallax->addChild(draw, 10, Vec2(0.5f, 0), Vec2(0, 0));

    showLevels();
}

void BigMapWorld::showLevels()
{
    /* 创建关卡 */
    auto world1 = createSprite("World2_1", Vec2(10, _backgroundSize.height / 2.0f), 1.f, 2, 0.5f, false);
    auto sprite = Button::create("begingame.png", "", "",cocos2d::ui::Widget::TextureResType::PLIST);
    sprite->setPosition(Vec2(480, 410));
    sprite->setAnchorPoint(Vec2(0, 0.5f));
    sprite->setGlobalZOrder(1);
    world1->addChild(sprite);

    if (_global->userInformation->getUserSelectWorldData().at(1)->levels <= 10)
    {
        auto prohibit = Sprite::createWithSpriteFrameName("Prohibit.png");
        prohibit->setPosition(Vec2(40, 30));
        prohibit->setScale(0.5f);
        prohibit->setGlobalZOrder(2);
        sprite->addChild(prohibit);
    }
    else
    {
        playProhibitMusic(sprite);
    }
    auto wgfd = Sprite::createWithSpriteFrameName("wgfd.png");
    wgfd->setPosition(Vec2(40, 15));
    wgfd->setScale(0.8f);
    wgfd->setGlobalZOrder(1);
    sprite->addChild(wgfd, -1);

    auto Level_1 = createSprite("World2_27", Vec2(900, 500), 0.6f, 2, 0.5f, true);
    createSprite(Level_1, "World2_8", Vec2(100, 380), 1.f, 0, true);
    createButton(Level_1, "begingame", Vec2(180, 290));

    auto Level_2 = createSprite("World2_24", Vec2(1200, 580), 0.8f, 2, 0.5f, true);
    createSprite(Level_2, "World2_6", Vec2(100, 470), 1.f, 2, true);
    createButton(Level_2, "begingame", Vec2(180, 290));

    auto Level_3 = createSprite("World2_24", Vec2(1500, 450), 0.6f, 2, 0.5f, true);
    createSprite(Level_3, "World2_14", Vec2(-200, 180), 1.0f, -1, false);
    createSprite(Level_3, "World2_19", Vec2(170, 360), 1.f, 1, true);
    createButton(Level_3, "begingame", Vec2(180, 270));

    auto Level_4 = createSprite("World2_28", Vec2(1900, 350), 0.8f, 2, 0.5f, true);
    createSprite(Level_4, "World2_14", Vec2(-200, 180), 1.0f, -1, false);
    createSprite(Level_4, "World2_7", Vec2(80, 380), 0.8f, 2, true);
    createButton(Level_4, "begingame", Vec2(150, 280));

    auto Level_5 = createSprite("World2_27", Vec2(2200, 300), 0.6f, 2, 0.5f, true);
    createSprite(Level_5, "World2_18", Vec2(40, 410), 0.7f, 1, true);
    createButton(Level_5, "begingame", Vec2(150, 300));

    auto Level_6 = createSprite("World2_25", Vec2(2500, 450), 0.7f, 2, 0.5f, true);
    createSprite(Level_6, "World2_16", Vec2(-200, 140), 1.0f, 2, false);
    createSprite(Level_6, "World2_19", Vec2(170, 360), 0.7f, 1, true);
    createButton(Level_6, "begingame", Vec2(180, 280));

    auto Level_7 = createSprite("World2_25", Vec2(2800, 620), 0.8f, 2, 0.5f, true);
    createSprite(Level_7, "World2_16", Vec2(-100, 160), 0.7f, 1, false);
    createSprite(Level_7, "World2_8", Vec2(80, 370), 1.3f, 0, true);
    createButton(Level_7, "begingame", Vec2(180, 280));

    auto Level_8 = createSprite("World2_24", Vec2(3100, 500), 0.6f, 2, 0.5f, true);
    createSprite(Level_8, "World2_9", Vec2(60, 360), 0.9f, 2, true);
    createButton(Level_8, "begingame", Vec2(170, 300));

    auto Level_9 = createSprite("World2_24", Vec2(3400, 400), 0.8f, 2, 0.5f, true);
    createSprite(Level_9, "World2_14", Vec2(-200, 180), 1.0f, -1, false);
    createSprite(Level_9, "World2_23", Vec2(40, 350), 0.9f, 4, true);
    createButton(Level_9, "begingame", Vec2(200, 300));

    auto Level_10 = createSprite("World2_26", Vec2(3800, 500), 0.5f, 2, 0.5f, true);
    createSprite(Level_10, "World2_22", Vec2(20, 370), 0.8f, 1, true);
    createButton(Level_10, "begingame", Vec2(200, 280));

    auto Level_11 = createSprite("World2_27", Vec2(4100, 650), 0.7f, 2, 0.5f, true);
    createSprite(Level_11, "World2_16", Vec2(-200, 140), 1.0f, 2, false);
    createSprite(Level_11, "World2_13", Vec2(200, 140), 1.f, 3, false);
    createSprite(Level_11, "World2_20", Vec2(70, 330), 0.8f, 3, true);
    createButton(Level_11, "begingame", Vec2(200, 270));

    auto Level_12 = createSprite("World2_27", Vec2(4500, 550), 0.65f, 2, 0.5f, true);
    createSprite(Level_12, "World2_6", Vec2(80, 380), 0.8f, 3, true);
    createButton(Level_12, "begingame", Vec2(180, 280));

    auto Level_13 = createSprite("World2_24", Vec2(4900, 500), 0.5f, 2, 0.5f, true);
    createSprite(Level_13, "World2_14", Vec2(-200, 260), 0.9f, -1, false);
    createSprite(Level_13, "World2_18", Vec2(50, 420), 0.9f, 2, true);
    createButton(Level_13, "begingame", Vec2(180, 280));

    auto Level_14 = createSprite("World2_24", Vec2(5120, 360), 0.8f, 3, 0.5f, true);
    createSprite(Level_14, "World2_14", Vec2(-200, 180), 1.0f, -1, false);
    createSprite(Level_14, "World2_19", Vec2(0, 400), 1.f, 3, true);
    createButton(Level_14, "begingame", Vec2(200, 250));

    auto Level_15 = createSprite("World2_25", Vec2(5500, 540), 0.7f, 2, 0.5f, true);
    createSprite(Level_15, "World2_11", Vec2(-250, 30), 1.f, 1, false);
    createSprite(Level_15, "World2_7", Vec2(40, 400), 1.0f, 2, true);
    createButton(Level_15, "begingame", Vec2(190, 240));

    auto Level_16 = createSprite("World2_27", Vec2(5900, 620), 0.6f, 2, 0.5f, true);
    createSprite(Level_16, "World2_16", Vec2(-200, 140), 1.0f, 2, false);
    createSprite(Level_16, "World2_9", Vec2(50, 340), 0.8f, 2, true);
    createButton(Level_16, "begingame", Vec2(160, 290));

    auto Level_17 = createSprite("World2_28", Vec2(6220, 740), 0.5f, 2, 0.5f, true);
    createSprite(Level_17, "World2_10", Vec2(30, 300), 0.8f, 1, true);
    createSprite(Level_17, "World2_13", Vec2(160, 120), 1.f, 2, false);
    createButton(Level_17, "begingame", Vec2(160, 270));

    auto Level_18 = createSprite("World2_24", Vec2(6500, 550), 0.55f, 2, 0.5f, true);
    createSprite(Level_18, "World2_14", Vec2(-200, 180), 1.0f, -1, false);
    createSprite(Level_18, "World2_21", Vec2(60, 430), 1.0f, 3, true);
    createButton(Level_18, "begingame", Vec2(170, 300));

    auto Level_19 = createSprite("World2_26", Vec2(6830, 430), 0.6f, 2, 0.5f, true);
    createSprite(Level_19, "World2_14", Vec2(-200, 180), 1.0f, -1, false);
    createSprite(Level_19, "World2_22", Vec2(50, 360), 1.0f, 2, true);
    createButton(Level_19, "begingame", Vec2(230, 280));

    auto Level_20 = createSprite("World2_28", Vec2(7130, 400), 0.65f, 2, 0.5f, true);
    createSprite(Level_20, "World2_18", Vec2(-10, 500), 1.5f, 1, true);
    createButton(Level_20, "begingame", Vec2(140, 240));

    auto Level_21 = createSprite("World2_24", Vec2(7400, 480), 0.6f, 3, 0.5f, true);
    createSprite(Level_21, "World2_11", Vec2(240, 330), 1.0f, -1, false);
    createSprite(Level_21, "World2_6", Vec2(50, 430), 1.0f, 2, true);
    createButton(Level_21, "begingame", Vec2(160, 260));

    auto Level_22 = createSprite("World2_28", Vec2(7700, 630), 0.5f, 2, 0.5f, true);
    createSprite(Level_22, "World2_9", Vec2(20, 370), 1.5f, 1, true);
    createButton(Level_22, "begingame", Vec2(180, 270));

    auto Level_23 = createSprite("World2_25", Vec2(8000, 750), 0.4f, 2, 0.5f, true);
    createSprite(Level_23, "World2_22", Vec2(100, 380), 1.0f, 2, true);
    createButton(Level_23, "begingame", Vec2(190, 240));

    auto Level_24 = createSprite("World2_27", Vec2(8300, 700), 0.6f, 2, 0.5f, true);
    createSprite(Level_24, "World2_14", Vec2(-200, 180), 1.0f, -1, false);
    createSprite(Level_24, "World2_12", Vec2(190, 70), 1.2f, 1, false);
    createSprite(Level_24, "World2_19", Vec2(0, 350), 1.2f, 1, true);
    createButton(Level_24, "begingame", Vec2(190, 230));

    auto Level_25 = createSprite("World2_24", Vec2(8600, 570), 0.5f, 2, 0.5f, true);
    createSprite(Level_25, "World2_20", Vec2(0, 320), 1.5f, 1, true);
    createButton(Level_25, "begingame", Vec2(190, 260));

    auto Level_26 = createSprite("World2_28", Vec2(8900, 490), 0.5f, 2, 0.5f, true);
    createSprite(Level_26, "World2_14", Vec2(-150, 200), 1.0f, -1, false);
    createSprite(Level_26, "World2_23", Vec2(120, 380), 1.0f, 3, true);
    createButton(Level_26, "begingame", Vec2(170, 240));

    auto Level_27 = createSprite("World2_24", Vec2(9200, 420), 0.6f, 2, 0.5f, true);
    createSprite(Level_27, "World2_14", Vec2(-200, 180), 1.0f, -1, false);
    createSprite(Level_27, "World2_6", Vec2(50, 490), 1.5f, 1, true);
    createButton(Level_27, "begingame", Vec2(160, 250));

    auto Level_28 = createSprite("World2_25", Vec2(9500, 520), 0.4f, 2, 0.5f, true);
    createSprite(Level_28, "World2_7", Vec2(100, 380), 1.0f, 2, true);
    createButton(Level_28, "begingame", Vec2(210, 240));

    auto Level_29 = createSprite("World2_27", Vec2(9840, 640), 0.6f, 2, 0.5f, true);
    createSprite(Level_29, "World2_16", Vec2(-200, 140), 1.0f, 2, false);
    createSprite(Level_29, "World2_18", Vec2(0, 420), 1.0f, 2, true);
    createButton(Level_29, "begingame", Vec2(200, 260));

    auto Level_30 = createSprite("World2_26", Vec2(10200, 700), 0.5f, 2, 0.5f, true);
    createSprite(Level_30, "World2_13", Vec2(220, 120), 1.f, 1, false);
    createSprite(Level_30, "World2_21", Vec2(0, 410), 1.5f, 1, true);
    createButton(Level_30, "begingame", Vec2(190, 260));

    auto Level_31 = createSprite("World2_28", Vec2(10530, 670), 0.4f, 2, 0.5f, true);
    createSprite(Level_31, "World2_9", Vec2(80, 380), 1.0f, 2, false);
    createButton(Level_31, "begingame", Vec2(170, 230));

    auto Level_32 = createSprite("World2_24", Vec2(10800, 580), 0.6f, 2, 0.5f, true);
    createSprite(Level_32, "World2_14", Vec2(-200, 180), 1.0f, -1, false);
    createSprite(Level_32, "World2_10", Vec2(100, 380), 1.0f, 2, true);
    createButton(Level_32, "begingame", Vec2(160, 250));

    auto Level_33 = createSprite("World2_25", Vec2(11100, 500), 0.5f, 2, 0.5f, true);
    createSprite(Level_33, "World2_14", Vec2(-200, 180), 1.0f, -1, false);
    createSprite(Level_33, "World2_19", Vec2(150, 360), 1.0f, 3, true);
    createButton(Level_33, "begingame", Vec2(170, 250));

    auto Level_34 = createSprite("World2_27", Vec2(11450, 370), 0.4f, 2, 0.5f, true);
    createSprite(Level_34, "World2_14", Vec2(-200, 180), 1.0f, -1, false);
    createSprite(Level_34, "World2_20", Vec2(30, 300), 1.0f, 2, true);
    createButton(Level_34, "begingame", Vec2(190, 250));

    auto Level_35 = createSprite("World1_2", Vec2(11790, 520), 1.4f, 2, 0.5f, false);
    createButton(Level_35, "begingame", Vec2(85, 240))->setScale(1.2f);
    createButton(Level_35, "begingame", Vec2(245, 175))->setScale(1.4f);
    createButton(Level_35, "begingame", Vec2(400, 240))->setScale(1.2f);

    auto Level_38 = createSprite("World2_24", Vec2(12600, 380), 0.6f, 2, 0.5f, true);
    createSprite(Level_38, "World2_14", Vec2(-200, 180), 1.0f, -1, false);
    createSprite(Level_38, "World2_23", Vec2(100, 380), 1.0f, 1, true);
    createButton(Level_38, "begingame", Vec2(190, 280));

    auto Level_39 = createSprite("World2_27", Vec2(12900, 470), 0.6f, 4, 0.5f, true);
    createSprite(Level_39, "World2_7", Vec2(50, 440), 1.5f, 1, true);
    createButton(Level_39, "begingame", Vec2(180, 280));

    auto Level_40 = createSprite("World2_25", Vec2(13180, 570), 0.6f, 3, 0.5f, true);
    createSprite(Level_40, "World2_16", Vec2(-200, 140), 1.0f, 2, false);
    createSprite(Level_40, "World2_9", Vec2(50, 350), 1.0f, 2, true);
    createButton(Level_40, "begingame", Vec2(170, 270));

    auto Level_41 = createSprite("World2_25", Vec2(13500, 680), 0.6f, 2, 0.5f, true);
    createSprite(Level_41, "World2_16", Vec2(-180, 110), 1.0f, 2, false);
    createSprite(Level_41, "World2_8", Vec2(50, 400), 1.5f, 1, true);
    createButton(Level_41, "begingame", Vec2(170, 260));

    auto Level_42 = createSprite("World2_27", Vec2(13800, 620), 0.6f, 2, 0.5f, true);
    createSprite(Level_42, "World2_14", Vec2(-200, 180), 1.0f, -1, false);
    createSprite(Level_42, "World2_13", Vec2(200, 120), 1.f, 4, false);
    createSprite(Level_42, "World2_18", Vec2(50, 420), 0.8f, 4, true);
    createButton(Level_42, "begingame", Vec2(170, 270));

    auto Level_43 = createSprite("World2_28", Vec2(14100, 530), 0.6f, 2, 0.5f, true);
    createSprite(Level_43, "World2_14", Vec2(-200, 180), 1.0f, -1, false);
    createSprite(Level_43, "World2_19", Vec2(50, 370), 1.2f, 1, true);
    createButton(Level_43, "begingame", Vec2(170, 250));

    auto Level_44 = createSprite("World2_24", Vec2(14400, 420), 0.6f, 2, 0.5f, true);
    createSprite(Level_44, "World2_14", Vec2(-200, 180), 1.0f, -1, false);
    createSprite(Level_44, "World2_20", Vec2(50, 370), 1.2f, 1, true);
    createButton(Level_44, "begingame", Vec2(170, 280));

    auto Level_45 = createSprite("World2_24", Vec2(14700, 330), 0.6f, 3, 0.5f, true);
    createSprite(Level_45, "World2_21", Vec2(50, 380), 0.8f, 3, true);
    createButton(Level_45, "begingame", Vec2(170, 280));

    auto Level_46 = createSprite("World2_25", Vec2(15000, 500), 0.7f, 2, 0.5f, true);
    createSprite(Level_46, "World2_16", Vec2(-200, 120), 1.0f, 2, false);
    createSprite(Level_46, "World2_22", Vec2(70, 360), 0.8f, 1, true);
    createButton(Level_46, "begingame", Vec2(200, 270));

    auto Level_47 = createSprite("World2_28", Vec2(15350, 650), 0.8f, 2, 0.5f, true);
    createSprite(Level_47, "World2_6", Vec2(40, 380), 1.2f, 4, true);
    createButton(Level_47, "begingame", Vec2(170, 230));

    auto Level_48 = createSprite("World2_25", Vec2(15700, 700), 0.6f, 2, 0.5f, true);
    createSprite(Level_48, "World2_17", Vec2(100, 380), 0.8f, 0, true);
    createButton(Level_48, "begingame", Vec2(200, 250));

    auto Level_49 = createSprite("World2_27", Vec2(16110, 650), 0.6f, 2, 0.5f, true);
    createSprite(Level_49, "World2_14", Vec2(-200, 180), 1.0f, -1, false);
    createSprite(Level_49, "World2_13", Vec2(200, 140), 1.f, 3, false);
    createSprite(Level_49, "World2_5", Vec2(50, 400), 1.0f, 2, true);
    createButton(Level_49, "begingame", Vec2(150, 250));

    auto Level_50 = createSprite("World1_2", Vec2(16400, 500), 1.4f, 2, 0.5f, true);
    createButton(Level_50, "begingame", Vec2(85, 240))->setScale(1.2f);
    createButton(Level_50, "begingame", Vec2(245, 175))->setScale(1.4f);
    createButton(Level_50, "begingame", Vec2(400, 240))->setScale(1.2f);
}

ui::Button* BigMapWorld::createButton(Node* node, const std::string& name, const Vec2& position)
{
    auto sprite4 = ui::Button::create(name + ".png", "", "",cocos2d::ui::Widget::TextureResType::PLIST);
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
                playLevelGameAndCaveThings(ID, 2);
                button->setTouchEnabled(false);
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
    auto back = ui::Button::create("back.png", "back1.png", "",cocos2d::ui::Widget::TextureResType::PLIST);
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
        const auto mouseEvent = static_cast<EventMouse*>(event);
        const float movex = mouseEvent->getScrollY() * 99360.f / (1920 + 600 * _global->userInformation->getUserSelectWorldData().at(1)->levels);

        MouseEventControl::mouseScrollControlListener(_scrollView, movex, ui::ScrollView::Direction::HORIZONTAL);
    };
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(mouse, _scrollView);
}
